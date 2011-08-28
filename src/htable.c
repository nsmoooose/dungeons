#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "htable.h"
#include "memory.h"

static int d_htable_next_2power (int n);
static void d_htable_node_destroy (struct d_htable *htable, struct d_htable_node * node);

struct d_htable *
d_htable_new (d_htable_hash_func_t hashfunc, d_htable_hash_cmp_func_t cmpfunc, int max_nodes) {
	struct d_htable *htable;
	int real_size = d_htable_next_2power (max_nodes);
	struct d_htable_node *node;
	int i;

	htable =
		d_calloc (1, sizeof (struct d_htable) +
				 real_size * (sizeof (struct d_htable_node *) + sizeof (struct d_htable_node)));
	htable->size = real_size;
	htable->nnodes = 0;
	htable->hash_func = hashfunc ? hashfunc : d_htable_hash_direct;
	htable->cmp_func = cmpfunc;
	htable->pool_start =
		((char *) htable) + sizeof (struct d_htable) +
		real_size * sizeof (struct d_htable_node *);
	htable->node_pool = htable->pool_start;

	for (i = 0, node = htable->node_pool; i < (real_size - 1); i++, node++) {
		node->next = node + 1;
	}

	return htable;
}

struct d_htable *
d_htable_new_str (int max_nodes) {
	return d_htable_new (d_htable_hash_str, d_htable_cmp_str, max_nodes);
}

struct d_htable *
d_htable_new_int (int max_nodes) {
	return d_htable_new (d_htable_hash_int, d_htable_cmp_int, max_nodes);
}

static int
d_htable_clear_foreach (void *key, void *value, void *data) {
	return 1;
}

void
d_htable_clear (struct d_htable *htable) {
	if (htable) {
		d_htable_foreach_remove (htable, d_htable_clear_foreach, 0);
	}
}

void
d_htable_destroy (struct d_htable *htable) {
	if (!htable) {
		return;
	}
	d_htable_clear (htable);
	d_free (htable);
}

int
d_htable_size (struct d_htable *htable) {
	if (!htable) {
		return 0;
	}
	return htable->nnodes;
}

static struct d_htable_node **
d_htable_lookup_node (struct d_htable *htable, const void *key) {
	struct d_htable_node **node;
	unsigned int hval = htable->hash_func (key);

	node = &htable->nodes[hval % htable->size];
	if (htable->cmp_func) {
		while (*node && !htable->cmp_func ((*node)->key, key))
			node = &(*node)->next;
	} else {
		while (*node && ((*node)->key != key))
			node = &(*node)->next;
	}
	return node;
}

void *
d_htable_lookup (struct d_htable *htable, const void *key) {
	struct d_htable_node *node;

	if ((!htable)) {
		return NULL;
	}

	node = *d_htable_lookup_node (htable, key);

	return node ? node->value : NULL;
}

int
d_htable_insert (struct d_htable *htable, void *key, void *value) {
	struct d_htable_node **node;

	assert (key);

	if ((!htable)) {
		return 0;
	}

	node = d_htable_lookup_node (htable, key);
	if (*node) {
		if ((*node)->value != value) {
			if (htable->unref) {
				htable->unref ((*node)->key, (*node)->value);
			}
			if (htable->remove) {
				htable->remove ((*node)->key, (*node)->value);
			}
			(*node)->value = value;
			if (htable->ref) {
				htable->ref (key, value);
			}
		}
	} else {
		*node  = htable->node_pool;
		assert (*node != NULL);
		htable->node_pool = htable->node_pool->next;
		(*node)->next = NULL;
		(*node)->key = key;
		(*node)->value = value;
		htable->nnodes++;

		if (htable->ref) {
			htable->ref (key, value);
		}
	}
	return 1;
}

void *
d_htable_remove (struct d_htable *htable, const void *key) {
	struct d_htable_node **node, *dest;
	void *node_key = NULL;
	void *node_value = NULL;

	if ((!htable)) {
		return NULL;
	}

	node = d_htable_lookup_node (htable, key);
	if (*node) {
		node_key = (*node)->key;
		node_value = (*node)->value;
		dest = *node;
		*node = dest->next;
		d_htable_node_destroy (htable, dest);
		htable->nnodes--;
	}

	if (htable->unref) {
		htable->unref (node_key, node_value);
	}
	if (htable->remove) {
		htable->remove (node_key, node_value);
	}

	return node_value;
}

void
d_htable_foreach (struct d_htable *htable, d_htable_foreach_func_t func, void *data) {
	int i;
	struct d_htable_node *node;

	if (!htable || !func) {
		return;
	}

	for (i = 0; i < htable->size; i++) {
		for (node = htable->nodes[i]; node; node = node->next) {
			func (node->key, node->value, data);
		}
	}
}

int
d_htable_foreach_remove (struct d_htable *htable,
						 d_htable_foreach_remove_func_t func,
						 void *data)
{
	struct d_htable_node *node, *prev;
	unsigned int i, deleted = 0;

	for (i = 0; i < htable->size; i++) {
	restart:
		prev = NULL;

		for (node = htable->nodes[i]; node; prev = node, node = node->next) {
			if (func (node->key, node->value, data)) {
				if (htable->unref) {
					htable->unref (node->key, node->value);
				}
				if (htable->remove) {
					htable->remove (node->key, node->value);
				}

				deleted++;
				htable->nnodes--;
				if (prev) {
					prev->next = node->next;
					d_htable_node_destroy (htable, node);
					node = prev;
				} else {
					htable->nodes[i] = node->next;
					d_htable_node_destroy (htable, node);
					goto restart;
				}
			}
		}
	}
	return deleted;
}

int
d_htable_iterator_init (struct d_htable_iterator *it, struct d_htable *table) {
	if (!it) {
		return 0;
	}
	memset (it, 0, sizeof (struct d_htable_iterator));

	if (!table) {
		return 0;
	}

	it->htable = table;
	it->node = it->htable->nodes[it->i_node];
	while (!it->node && (it->i_node < (it->htable->size - 1))) {
		it->i_node++;
		it->node = it->htable->nodes[it->i_node];
	}

	return 1;
}

void
d_htable_iterator_next (struct d_htable_iterator *it) {
	if (!it || !it->node) {
		return;
	}
	it->node = it->node->next;
	while (!it->node && (it->i_node < (it->htable->size - 1))) {
		it->i_node++;
		it->node = it->htable->nodes[it->i_node];
	}
}

unsigned int
d_htable_hash_direct (const void *key) {
	return (intptr_t) key;
}

unsigned int
d_htable_hash_int (const void *key) {
	return *((unsigned int*)(key));
}

int
d_htable_cmp_int (const void *v, const void *v2) {
	return (*(unsigned int*)(v)) == *((unsigned int*)(v2));
}

int
d_htable_cmp_str (const void *v, const void *v2) {
	return !strcmp (v, v2);
}

unsigned int
d_htable_hash_str (const void *key) {
	const char *p = key;
	int h = *p;

	if (h) {
		for (p += 1; *p != '\0'; p++) {
			h = (h << 5) - h + *p;
		}
	}
	return h;
}

static void
d_htable_node_destroy (struct d_htable *htable, struct d_htable_node *node) {
	node->next = htable->node_pool;
	htable->node_pool = node;
}

static int
d_htable_next_2power (int n) {
	int i;

	for (i = 0; i < 31; i++) {
		if (n < (1 << i)) {
			return (1 << i);
		}
	}

	return n;
}

int
d_htable_at (struct d_htable* htable, int index,
				 void **key, void **value) {
	struct d_htable_iterator it;
	d_htable_iterator_init (&it, htable);
	int i=0;
	while (it.node) {
		if (i == index) {
			*key = it.node->key;
			*value = it.node->value;
			return 1;
		}
		d_htable_iterator_next (&it);
	}
	return 0;
}
