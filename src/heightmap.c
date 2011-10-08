#include "error.h"
#include "heightmap.h"
#include "memory.h"

struct d_heightmap*
d_heightmap_new (int size) {
    /* For a sizeXsize array, we need (size+1)X(size+1) space. For
       example, a 2x2 mesh needs 3x3=9 data points:

           *   *   *

           *   *   *

           *   *   *

       To account for this, increment 'size'. */
	struct d_heightmap *hm = d_calloc (1, sizeof (struct d_heightmap));
    hm->width = size;
	hm->height = size;
    hm->array = d_calloc (1, sizeof(float) * (hm->width + 1) * (hm->height + 1));
	return hm;
}

void
d_heightmap_destroy (struct d_heightmap *hm) {
	d_free (hm->array);
    d_free (hm);
}

short
d_heightmap_get (struct d_heightmap *hm, int x, int y) {
	if (x < 0 || y < 0 || x > hm->width || y > hm->height) {
		d_bug ("Coordinates outside heightmap.");
	}
	return hm->array[y * hm->width + x];
}

void
d_heightmap_set (struct d_heightmap *hm, int x, int y, short value) {
	if (x < 0 || y < 0 || x > hm->width || y > hm->height) {
		d_bug ("Coordinates outside heightmap.");
	}
	hm->array[y * hm->width + x] = value;
}
