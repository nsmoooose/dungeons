#ifndef __DUNGEONS_HTABLE_H__
#define __DUNGEONS_HTABLE_H__

typedef unsigned int (*d_htable_hash_func_t) (const void *key);
typedef int (*d_htable_hash_cmp_func_t) (const void *key1, const void *key2);
typedef void (*d_htable_foreach_func_t) (void *key, void *value, void *data);
typedef int (*d_htable_foreach_remove_func_t) (void *key,
											   void *value, void *data);
typedef void (*d_htable_cb) (void *key, void *value);


unsigned int d_htable_hash_direct (const void *key);
unsigned int d_htable_hash_str (const void *key);
int d_htable_cmp_str (const void *v, const void *v2);
unsigned int d_htable_hash_int (const void *key);
int d_htable_cmp_int (const void *v, const void *v2);

struct d_htable_node {
	struct d_htable_node *next;
	void *key;
	void *value;
};

struct d_htable {
	int size, nnodes;
	struct d_htable_node *node_pool;
	void *pool_start;
	d_htable_hash_func_t hash_func;
	d_htable_hash_cmp_func_t cmp_func;
	d_htable_cb ref, unref, remove;
	struct d_htable_node *nodes[];
};

struct d_htable_iterator {
	struct d_htable *htable;
	int i_node;
	struct d_htable_node *node;
};

struct d_htable *d_htable_new (
	d_htable_hash_func_t hashfunc,
	d_htable_hash_cmp_func_t cmpfunc, int max_nodes);
struct d_htable *d_htable_new_str (int max_nodes);
struct d_htable *d_htable_new_int (int max_nodes);

void d_htable_destroy (struct d_htable *htable);
int d_htable_size (struct d_htable *htable);
void *d_htable_lookup (struct d_htable *htable, const void *key);
int d_htable_lookup_extended (struct d_htable *htable,
							  const void *lookup_key,
							  void **origkey, void **value);
int d_htable_insert (struct d_htable *htable, void *key, void *value);
void *d_htable_remove (struct d_htable *htable, const void *key);
void d_htable_foreach (struct d_htable *htable,
					   d_htable_foreach_func_t func, void *data);
int d_htable_foreach_remove (struct d_htable *htable,
							 d_htable_foreach_remove_func_t func, void *data);
void d_htable_clear (struct d_htable *htable);

int d_htable_iterator_init (struct d_htable_iterator *it,
							struct d_htable *table);
void d_htable_iterator_next (struct d_htable_iterator *it);
int d_htable_at (struct d_htable* htable, int index,
				 void **key, void **value);

#endif
