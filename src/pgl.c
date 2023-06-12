#include "pgl.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

pgl_canvas *pgl_canvas_new(uint32_t *pixels, int width, int height) {
    pgl_canvas *pc = (pgl_canvas *)malloc(sizeof(pgl_canvas));
    pc->pixels = pixels;
    pc->width = width;
    pc->height = height;
    return pc;
}

void pgl_set_pixel(pgl_canvas *pc, int x, int y, uint32_t color) {
    pc->pixels[x + (y * pc->width)] = color;
}

/* static void _pgl_draw_line_low(pgl_canvas *pc, int x1, int y1, int x2, int y2, */
/*                                uint32_t color) { */
/*     int diff_x = x2 - x1; */
/*     int diff_y = y2 - y1; */
/*     int y_inc = 1; */
/*     if (diff_y < 0) { */
/*         y_inc = -1; */
/*         diff_y = -diff_y; */
/*     } */
/*     int D = (2 * diff_y) - diff_x; */
/*     int y = y1; */

/*     for (int x = x1; x <= x2; x++) { */
/*         pgl_set_pixel(pc, x, y, color); */
/*         if (D > 0) { */
/*             y += y_inc; */
/*             D += (2 * (diff_y - diff_x)); */
/*         } else { */
/*             D += 2 * diff_y; */
/*         } */
/*     } */
/* } */

/* static void _pgl_draw_line_high(pgl_canvas *pc, int x1, int y1, int x2, int y2, */
/*                                 uint32_t color) { */
/*     int diff_x = x2 - x1; */
/*     int diff_y = y2 - y1; */
/*     int x_inc = 1; */

/*     if (diff_x < 0) { */
/*         x_inc = -1; */
/*         diff_x = -diff_x; */
/*     } */
/*     int D = (2 * diff_x) - diff_y; */
/*     int x = x1; */

/*     for (int y = y1; y <= y2; y++) { */
/*         pgl_set_pixel(pc, x, y, color); */
/*         if (D > 0) { */
/*             x += x_inc; */
/*             D += (2 * (diff_x - diff_y)); */
/*         } else { */
/*             D += 2 * diff_x; */
/*         } */
/*     } */
/* } */

/* void pgl_draw_line(pgl_canvas *pc, int x1, int y1, int x2, int y2, */
/*                    uint32_t color) { */
/*     if (x1 == x2) { */
/*         for (int i = MIN(y1, y2); i <= MAX(y1, y2); i++) { */
/*             pgl_set_pixel(pc, x1, i, color); */
/*         } */
/*     } else if (y1 == y2) { */
/*         for (int i = MIN(x1, x2); i <= MAX(x1, x2); i++) { */
/*             pgl_set_pixel(pc, i, y1, color); */
/*         } */
/*     } else { */
/*         // Diagonal line */
/*         // Source: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm */
/*         if (abs(y2 - y1) < abs(x2 - x1)) { */
/*             if (x1 > x2) { */
/*                 _pgl_draw_line_low(pc, x2, y2, x1, y1, color); */
/*             } else { */
/*                 _pgl_draw_line_low(pc, x1, y1, x2, y2, color); */
/*             } */
/*         } else { */
/*             if (y1 > y2) { */
/*                 _pgl_draw_line_high(pc, x2, y2, x1, y1, color); */
/*             } else { */
/*                 _pgl_draw_line_high(pc, x1, y1, x2, y2, color); */
/*             } */
/*         } */
/*     } */
/* } */

void pgl_draw_line(pgl_canvas *pc, int x1, int y1, int x2, int y2,
                   uint32_t color) {
    int diff_x = abs(x1 - x2);
    int sign_x = x1 < x2 ? 1 : -1;

    int diff_y = -abs(y2 - y1);
    int sign_y = y1 < y2 ? 1 : -1;

    int error = diff_x + diff_y;

    while (true) {
        pgl_set_pixel(pc, x1, y1, color);
        if (x1 == x2 && y1 == y2) { break; }
        int error2 = 2 * error;
        if (error2 >= diff_y) {
            if (x1 == x2) { break; }
            error += diff_y;
            x1 += sign_x;
        }
        if (error2 <= diff_x) {
            if (y1 == y2) { break; }
            error += diff_x;
            y1 += sign_y;
        }
    }

}
