#include "../error.h"
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

/* Notes:
 * Viewpoint always refers to the center of the screen.
 */

void
d_map_coord_to_screen (struct d_ui_area *area, struct d_viewpoint *vp,
					   int zoom, struct d_point2 *coord, struct d_point2 *pos) {
	/* TODO We need to handle zoom level. */
	pos->x = coord->x - vp->x + (area->size.width / 2);
	pos->y = coord->y - vp->y + (area->size.height / 2);
}

void
d_map_coord_to_real (struct d_ui_area *area, struct d_viewpoint *vp,
					 int zoom, struct d_point2 *screen_coords, struct d_point2 *pos) {
	pos->x = vp->x - (area->size.width / 2) * zoom + screen_coords->x * zoom;
	pos->y = vp->y - (area->size.height / 2) * zoom + screen_coords->y * zoom;
}

void
d_map_draw (
	struct d_ui_area *area, struct d_viewpoint *vp, int zoom, struct d_heightmap *hm) {
	for (int x=area->pos.x;x<=area->size.width;++x) {
		for (int y=area->pos.y;y<=area->size.height;++y) {
			struct d_point2 screen = { x, y };
			struct d_point2 real;
			d_map_coord_to_real (area, vp, zoom, &screen, &real);

			if (real.x < 0 || real.y < 0 || real.x > hm->width || real.y > hm->height) {
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

			float f = d_heightmap_get (hm, real.x, real.y);
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
