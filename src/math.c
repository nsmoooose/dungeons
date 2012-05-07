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

