#include "list.h"
#include "memory.h"

struct d_list *
d_list_new () {
	return d_calloc (1, sizeof (struct d_list));
}

void
d_list_destroy (struct d_list *list) {
	while (list->first) {
		d_list_remove (list, list->first);
	}
	d_free (list);
}

struct d_list_node *
d_list_prepend (struct d_list *list, void *data) {
	struct d_list_node *node = d_calloc (1, sizeof (struct d_list_node));
	node->prev = 0;
	node->data = data;
	node->next = list->first;
	if (list->first) {
		list->first->prev = node;
	}
	list->first = node;
	if (!list->last) {
		list->last = node;
	}
	list->nnodes++;

	if (list->ref) {
		list->ref (data);
	}

	return node;
}

struct d_list_node *
d_list_append (struct d_list *list, void *data) {
	struct d_list_node *node = d_calloc (1, sizeof (struct d_list_node));
	node->data = data;
	node->prev = list->last;
	node->next = 0;
	if (list->last) {
		list->last->next = node;
	}
	list->last = node;
	if (!list->first) {
		list->first = node;
	}
	list->nnodes++;

	if (list->ref) {
		list->ref (data);
	}
	return node;
}

void *
d_list_remove (struct d_list *list, struct d_list_node *node) {
	void *data = node->data;
	if (node->prev) {
		node->prev->next = node->next;
	}
	if (node->next) {
		node->next->prev = node->prev;
	}
	if (list->first == node) {
		list->first = node->next;
	}
	if (list->last == node) {
		list->last = node->prev;
	}
	d_free (node);
	list->nnodes--;

	if (list->unref) {
		list->unref (data);
	}
	if (list->remove) {
		list->remove (data);
	}

	return data;
}

void
d_list_copy (struct d_list *dst, struct d_list *src) {
	for (struct d_list_node *node = src->first;node;node=node->next) {
		d_list_append (dst, node->data);
	}
}

void
d_list_clear (struct d_list *list) {
	while (list->first) {
		d_list_remove (list, list->first);
	}
}
