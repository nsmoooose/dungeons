#include "error.h"
#include "map.h"

/* Symbols to use:
   ~ =
   ▓ =
   ■ =
   · =
   ' =
   ▼ =
   ▲ =
*/

void
d_map_draw (
	struct d_ui_area *area, struct d_viewpoint *vp, int zoom, struct d_heightmap *hm) {
	for (int x=area->pos.x;x<=area->size.width;++x) {
		for (int y=area->pos.y;y<=area->size.height;++y) {
			int realx = vp->x - (area->size.width / 2) * zoom + x * zoom;
			int realy = vp->y - (area->size.height / 2) * zoom + y * zoom;
			if (realx < 0 || realy < 0 || realx > hm->width || realy > hm->height) {
				d_ui->set_color (d_black_white);
				d_ui->printf_left (x, y, "X");
				continue;
			}

			/* Our point can be in four different places.
			   - sea.
			   - air
			   - below ground level.
			   - ground level.
			*/

			float f = d_heightmap_get (hm, realx, realy);
			if (vp->z - 15.0 > f && vp->z <= 0.0) {
				/* above ground but below sea level. */
				d_ui->set_color (d_black_blue);
				d_ui->printf_left (x, y, "~");
			}
			else if (f <= vp->z + 15.0 && f >= vp->z - 15.0) {
				if (f <= 0.0) {
					d_ui->set_color (d_black_blue);
				}
				else {
					/* ground level. */
					d_ui->set_color (d_black_green);
				}
				d_ui->printf_left (x, y, ".");
			}
			else if (vp->z + 15.0 > f && vp->z > 0.0) {
				/* above ground and above sea level. */
				d_ui->set_color (d_black_cyan);
				d_ui->printf_left (x, y, "'");
			}
			else if (vp->z - 15.0 < f) {
				/* below ground level. */
				d_ui->set_color (d_black_white);
				d_ui->printf_left (x, y, " ");
			}
			else {
				d_bug ("Not supposed to happen. Viewpoint z: %d, Ground level: %f", vp->z, f);
			}
		}
	}
}
