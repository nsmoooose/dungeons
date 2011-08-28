#ifndef __DUNGEONS_LIST_H__
#define __DUNGEONS_LIST_H__

typedef void (*d_list_cb)(void *data);

struct d_list_node {
	void *data;
	struct d_list_node *next, *prev;
};

struct d_list {
    struct d_list_node *first, *last;
	int nnodes;
	d_list_cb ref, unref, remove;
};

struct d_list *d_list_new ();
void d_list_destroy (struct d_list *list);
struct d_list_node *d_list_prepend (struct d_list *list, void *data);
struct d_list_node *d_list_append (struct d_list *list, void *data);
void *d_list_remove (struct d_list *list, struct d_list_node *node);

void d_list_copy (struct d_list *dst, struct d_list *src);

#endif
