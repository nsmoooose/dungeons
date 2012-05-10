#include <stdlib.h>
#include "math.h"

int
d_is_power_of_2 (int size) {
    int i, bitcount = 0;
    for (i=0; i<sizeof(int)*8; i++) {
		if (size & (1<<i)) {
			bitcount++;
		}
	}
    if (bitcount == 1) {
		return 1;
	}
    else {
		return 0;
	}
}

int
d_rand (int min, int max) {
	return rand () % (max-min) + min;
}

int
d_aabb_contains (struct d_aabb *aabb, struct d_point3 *pos) {
	if (pos->x >= aabb->pos.x - aabb->half_dimension && pos->x < aabb->pos.x + aabb->half_dimension &&
	    pos->y >= aabb->pos.y - aabb->half_dimension && pos->y < aabb->pos.y + aabb->half_dimension &&
	    pos->z >= aabb->pos.z - aabb->half_dimension && pos->z < aabb->pos.z + aabb->half_dimension) {
		return 1;
	}
	return 0;
}
