#ifndef __DUNGEONS_MATH_H__
#define __DUNGEONS_MATH_H__

struct d_point2 {
	int x, y;
};

struct d_point3 {
	int x,y,z;
};

struct d_aabb3 {
	struct d_point3 pos;
	int half_dimension;
};

int d_is_power_of_2 (int size);
int d_rand (int min, int max);

int d_aabb3_contains (struct d_aabb3 *aabb, struct d_point3 *pos);

#endif
