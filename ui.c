#include "list.h"
#include "ui.h"

struct d_list *d_ui_stack = 0;

struct d_ui_handler*
d_ui_stack_pop () {
	struct d_ui_handler* handler = d_ui_stack->first->data;
	d_list_remove (d_ui_stack, d_ui_stack->first);
	if (!d_ui_stack->nnodes) {
		d_list_destroy (d_ui_stack);
		d_ui_stack = 0;
	}
	return handler;
}

void
d_ui_stack_push (struct d_ui_handler* handler) {
	if (!d_ui_stack) {
		d_ui_stack = d_list_new ();
	}
	d_list_prepend (d_ui_stack, handler);
}

struct d_ui_handler*
d_ui_stack_current () {
	if (!d_ui_stack) {
		return 0;
	}
	return d_ui_stack->first->data;
}

struct d_ui_handler*
d_ui_stack_prev (struct d_ui_handler *handler) {
	if (!d_ui_stack) {
		return 0;
	}
	for (struct d_list_node *node = d_ui_stack->first;node;node = node->next) {
		if (node->data == handler && node->next) {
			return node->next->data;
		}
	}
	return 0;
}
