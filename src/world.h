#ifndef __DUNGEONS_WORLD_H__
#define __DUNGEONS_WORLD_H__

struct d_world {
	int width;
	int height;
	int depth;

	/*
	  struct d_heightmap hm;
	  struct d_htable tiles;
	*/
};

struct d_pos {
	short x, y, z;
};

struct d_tile {
	struct d_pos pos;
	/*
	   struct d_list* objects;
	   shortcuts for adjascent tiles?
	   
	*/
};

#endif
