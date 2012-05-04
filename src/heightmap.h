#ifndef __DUNGEONS_HEIGHTMAP_H__
#define __DUNGEONS_HEIGHTMAP_H__

struct d_heightmap {
	int width;
	int height;
	short *array;
};

struct d_heightmap* d_heightmap_new (int size);
void d_heightmap_destroy (struct d_heightmap *hm);

short d_heightmap_get (struct d_heightmap *hm, int x, int y);
void d_heightmap_set (struct d_heightmap *hm, int x, int y, short value);
void d_heightmap_get_highlow (struct d_heightmap *hm, int *high, int *low);

#endif
