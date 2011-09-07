#ifndef __DUNGEONS_MEMORY_H__
#define __DUNGEONS_MEMORY_H__

#include <stdlib.h>

void* d_calloc (size_t nmemb, size_t size);
void d_free (void *ptr);
void d_print_memory_leaks ();

extern unsigned int d_allocations;
extern unsigned int d_frees;

#endif
