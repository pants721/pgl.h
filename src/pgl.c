#include "pgl.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

pgl_canvas *pgl_canvas_new(uint32_t *pixels, int width, int height) {
    pgl_canvas *pc = (pgl_canvas *)malloc(sizeof(pgl_canvas));
    pc->pixels = pixels;
    pc->width = width;
    pc->height = height;
    return pc;
}

void pgl_set_pixel(pgl_canvas *pc, int x, int y, uint32_t color) {
    if (pgl_in_bounds(pc, x, y)) {
        pc->pixels[x + (y * pc->width)] = color;
    }
}

void pgl_clear(pgl_canvas *pc) {
    for (int i = 0; i < pc->width * pc->height; i++) {
        pc->pixels[i] = NULL;
    }
}

void pgl_line(pgl_canvas *pc, int x1, int y1, int x2, int y2,
                   uint32_t color) {
    int dx = abs(x1 - x2);
    int sx = x1 < x2 ? 1 : -1;

    int dy = -abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;

    int e = dx + dy;

    while (true) {
        pgl_set_pixel(pc, x1, y1, color);
        if (x1 == x2 && y1 == y2) { break; }
        int e2 = 2 * e;
        if (e2 >= dy) {
            if (x1 == x2) { break; }
            e += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            if (y1 == y2) { break; }
            e += dx;
            y1 += sy;
        }
    }
}

void pgl_circle(pgl_canvas *pc, int center_x, int center_y, int r, 
        uint32_t color) {
    int t1 = r / 16;
    int x = r;
    int y = 0;
    while (x >= y) {
      pgl_line(pc, center_x + x, center_y + y, center_x - x, center_y + y,
               color);
      pgl_line(pc, center_x + x, center_y - y, center_x - x, center_y - y,
               color);
      pgl_line(pc, center_x + y, center_y + x, center_x - y, center_y + x,
               color);
      pgl_line(pc, center_x + y, center_y - x, center_x - y, center_y - x,
               color);
      y += 1;
      t1 += y;
      int t2 = t1 - x;
      if (t2 >= 0) {
        t1 = t2;
        x -= 1;
        }
    }
}

inline static void bottom_flat_triangle(pgl_canvas *pc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
    float inv_slope_1 = (x2 - x1) / (y2 - y1);
    float inv_slope_2 = (x3 - x1) / (y3 - y1);

    float curr_x1 = x1;
    float curr_x2 = x1;

    for (int scan_y = y1; scan_y <= y2; scan_y++) {
        pgl_line(pc, (int)curr_x1, scan_y, (int)curr_x2, scan_y, color);
        curr_x1 += inv_slope_1;
        curr_x2 += inv_slope_2;
    }
}

inline static void top_flat_triangle(pgl_canvas *pc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
    float inv_slope_1 = (x3 - x1) / (y3 - y1);
    float inv_slope_2 = (x3 - x2) / (y3 - y2);

    float curr_x1 = x3;
    float curr_x2 = x3;

    for (int scan_y = y3; scan_y <= y1; scan_y--) {  
        pgl_line(pc, (int)curr_x1, scan_y, (int)curr_x2, scan_y, color);
        curr_x1 -= inv_slope_1;
        curr_x2 -= inv_slope_2;
    }
}

void pgl_triangle(pgl_canvas *pc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
    int x_temp;
    int y_temp;

    if (y1 > y2) {
        x_temp = x1; y_temp = y1;
        x1 = x2; y1 = y2;
        x2 = x_temp; y2 = y_temp;
    }
    
    if (y1 > y3) {
        x_temp = x1; y_temp = y1;
        x1 = x3; y1 = y3;
        x3 = x_temp; y3 = y_temp;
    }

    if (y2 > y3) {
        x_temp = x2; y_temp = y2;
        x2 = x3; y2 = y3;
        x3 = x_temp; y3 = y_temp;
    }

    if (y2 == y3) {
        bottom_flat_triangle(pc, x1, y1, x2, y2, x3, y3, color);
    } else if (y1 == y2) {
        top_flat_triangle(pc, x1, y1, x2, y2, x3, y3, color);
    } else {
        int x4 = (int)(x1 + (float)(y2 - y1) / (float)(y3 - y1) * (x3 - x1));
        int y4 = y2;
        bottom_flat_triangle(pc, x1, y1, x2, y2, x4, y4, color);
        top_flat_triangle(pc, x2, y2, x4, y4, x3, y3, color);
    }
}

bool pgl_in_bounds(pgl_canvas *pc, int x, int y) {
    return 0 <= x && x < (int)pc->width && 0 <= y && y < (int)pc->height;
}
