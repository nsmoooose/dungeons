/*
  Written by: Paul E. Martz
  Copyright 1997 by Paul E. Martz, all right reserved
  Non-commercial use by individuals is permitted.

  Modified to fit this dungeons project.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "error.h"
#include "fractal_heightmap.h"
#include "memory.h"

/*
 * randNum - Return a random floating point number such that
 *      (min <= return-value <= max)
 * 32,767 values are possible for any given range.
 */
static float
randnum (float min, float max) {
	int r;
    float	x;
	r = random ();
    x = (float)(r & 0x7fff) /
		(float)0x7fff;
    return (x * (max - min) + min);
}

/*
 * avgDiamondVals - Given the i,j location as the center of a diamond,
 * average the data values at the four corners of the diamond and
 * return it. "Stride" represents the distance from the diamond center
 * to a diamond corner.
 */
static float
avgDiamondVals (int i, int j, int stride,
				int size, int subSize, short *fa) {
    /* In this diagram, our input stride is 1, the i,j location is
       indicated by "X", and the four value we want to average are
       "*"s:
           .   *   .

           *   X   *

           .   *   .
       */

    /* In order to support tiled surfaces which meet seamless at the
       edges (that is, they "wrap"), We need to be careful how we
       calculate averages when the i,j diamond center lies on an edge
       of the array. The first four 'if' clauses handle these
       cases. The final 'else' clause handles the general case (in
       which i,j is not on an edge).
     */
    if (i == 0) {
		return ((float) (fa[(i*size) + j-stride] +
						 fa[(i*size) + j+stride] +
						 fa[((subSize-stride)*size) + j] +
						 fa[((i+stride)*size) + j]) * .25f);
	}
    else if (i == size-1) {
		return ((float) (fa[(i*size) + j-stride] +
						 fa[(i*size) + j+stride] +
						 fa[((i-stride)*size) + j] +
						 fa[((0+stride)*size) + j]) * .25f);
	}
    else if (j == 0) {
		return ((float) (fa[((i-stride)*size) + j] +
						 fa[((i+stride)*size) + j] +
						 fa[(i*size) + j+stride] +
						 fa[(i*size) + subSize-stride]) * .25f);
	}
    else if (j == size-1) {
		return ((float) (fa[((i-stride)*size) + j] +
						 fa[((i+stride)*size) + j] +
						 fa[(i*size) + j-stride] +
						 fa[(i*size) + 0+stride]) * .25f);
	}
    else {
		return ((float) (fa[((i-stride)*size) + j] +
						 fa[((i+stride)*size) + j] +
						 fa[(i*size) + j-stride] +
						 fa[(i*size) + j+stride]) * .25f);
	}
}

/*
 * avgSquareVals - Given the i,j location as the center of a square,
 * average the data values at the four corners of the square and return
 * it. "Stride" represents half the length of one side of the square.
 */
static float
avgSquareVals (int i, int j, int stride, int size, short *fa) {
    /* In this diagram, our input stride is 1, the i,j location is
       indicated by "*", and the four value we want to average are
       "X"s:
           X   .   X

           .   *   .

           X   .   X
       */
    return ((float) (fa[((i-stride)*size) + j-stride] +
		     fa[((i-stride)*size) + j+stride] +
		     fa[((i+stride)*size) + j-stride] +
		     fa[((i+stride)*size) + j+stride]) * .25f);
}

/*
 * Returns 1 if size is a power of 2. Returns 0 if size is
 * not a power of 2, or is zero.
 */
static int
d_fractal_is_power_of_2 (int size) {
    int i, bitcount = 0;

    /* Note this code assumes that (sizeof(int)*8) will yield the
       number of bits in an int. Should be portable to most
       platforms. */
    for (i=0; i<sizeof(int)*8; i++) {
		if (size & (1<<i)) {
			bitcount++;
		}
	}
    if (bitcount == 1) {
		/* One bit. Must be a power of 2. */
		return (1);
	}
    else {
		/* either size==0, or size not a power of 2. Sorry, Charlie. */
		return (0);
	}
}

/*
 * Use the diamond-square algorithm to tessalate a
 * grid of float values into a fractal height map.
 */
void
d_fractal_heightmap_generate (struct d_heightmap *hm,
							  int seed, float heightScale, float h) {
	srandom (time (0));

    int	i, j;
    int	stride;
    int	oddline;
    int subSize;
	float ratio, scale;

	short *fa = hm->array;
	int size = hm->width;

    if (!d_fractal_is_power_of_2 (size) || (size==1)) {
		d_bug ("We can't tesselate the array if it is not a power of 2.");
    }

    /* subSize is the dimension of the array in terms of connected line
       segments, while size is the dimension in terms of number of
       vertices. */
    subSize = size;
    size++;

    /* initialize random number generator */
    srandom (seed);

	/* Set up our roughness constants.
	   Random numbers are always generated in the range 0.0 to 1.0.
	   'scale' is multiplied by the randum number.
	   'ratio' is multiplied by 'scale' after each iteration
	   to effectively reduce the randum number range.
	   */
	ratio = (float) pow (2., -h);
	scale = heightScale * ratio;

    /* Seed the first four values. For example, in a 4x4 array, we
       would initialize the data points indicated by '*':

           *   .   .   .   *

           .   .   .   .   .

           .   .   .   .   .

           .   .   .   .   .

           *   .   .   .   *

       In terms of the "diamond-square" algorithm, this gives us
       "squares".

       We want the four corners of the array to have the same
       point. This will allow us to tile the arrays next to each other
       such that they join seemlessly. */

    stride = subSize / 2;
    fa[(0*size)+0] =
      fa[(subSize*size)+0] =
        fa[(subSize*size)+subSize] =
          fa[(0*size)+subSize] = 0.f;

    /* Now we add ever-increasing detail based on the "diamond" seeded
       values. We loop over stride, which gets cut in half at the
       bottom of the loop. Since it's an int, eventually division by 2
       will produce a zero result, terminating the loop. */
    while (stride) {
		/* Take the existing "square" data and produce "diamond"
		   data. On the first pass through with a 4x4 matrix, the
		   existing data is shown as "X"s, and we need to generate the
	       "*" now:

               X   .   .   .   X

               .   .   .   .   .

               .   .   *   .   .

               .   .   .   .   .

               X   .   .   .   X

	      It doesn't look like diamonds. What it actually is, for the
	      first pass, is the corners of four diamonds meeting at the
	      center of the array. */
		for (i=stride; i<subSize; i+=stride) {
			for (j=stride; j<subSize; j+=stride) {
				fa[(i * size) + j] =
					(short)(scale * randnum (-0.5f, 0.5f) +
						  avgSquareVals (i, j, stride, size, fa));
				j += stride;
			}
			i += stride;
		}

		/* Take the existing "diamond" data and make it into
	       "squares". Back to our 4X4 example: The first time we
	       encounter this code, the existing values are represented by
	       "X"s, and the values we want to generate here are "*"s:

               X   .   *   .   X

               .   .   .   .   .

               *   .   X   .   *

               .   .   .   .   .

               X   .   *   .   X

	       i and j represent our (x,y) position in the array. The
	       first value we want to generate is at (i=2,j=0), and we use
	       "oddline" and "stride" to increment j to the desired value.
	       */
		oddline = 0;
		for (i=0; i<subSize; i+=stride) {
		    oddline = (oddline == 0);
			for (j=0; j<subSize; j+=stride) {
				if ((oddline) && !j) j+=stride;

				/* i and j are setup. Call avgDiamondVals with the
				   current position. It will return the average of the
				   surrounding diamond data points. */
				fa[(i * size) + j] =
					(short)(scale * randnum (-0.5, 0.5f) +
						  avgDiamondVals (i, j, stride, size, subSize, fa));

				/* To wrap edges seamlessly, copy edge values around
				   to other side of array */
				if (i==0)
					fa[(subSize*size) + j] =
						fa[(i * size) + j];
				if (j==0)
					fa[(i*size) + subSize] =
						fa[(i * size) + j];

				j+=stride;
			}
		}

		/* reduce random number range. */
		scale *= ratio;
		stride >>= 1;
    }
}

struct d_heightmap*
d_fractal_heightmap_new (int size) {
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
d_fractal_heightmap_destroy (struct d_heightmap *hm) {
	d_free (hm->array);
    d_free (hm);
}

short
d_fractal_heightmap_get (struct d_heightmap *hm, int x, int y) {
	if (x < 0 || y < 0 || x > hm->width || y > hm->height) {
		d_bug ("Coordinates outside heightmap.");
	}
	return hm->array[y * hm->width + x];
}
