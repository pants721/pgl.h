// Copyright (C) 2023 Lucas Newcomb <lucasnewcomb721@gmail.com>
// Use of this source code is governed by the DO WHAT THE FUCK YOU WANT TO
// PUBLIC LICENSE that can be found in the LICENSE file or at
// https://choosealicense.com/licenses/wtfpl/

// =============================================================================
// Pants Graphics Library public API
// =============================================================================

// TODO: Add testing
// TODO: Add transparency

#ifndef PGL_H 
#define PGL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PGL_MIN(a,b) (((a)<(b))?(a):(b))
#define PGL_MAX(a,b) (((a)>(b))?(a):(b))
#define PGL_SWAP(T, a, b) do { T t = a; a = b; b = t; } while (0)
#define PGL_ABS(x) ((x) > 0 ? (x) : -(x))
#define PGL_SIGN(x) (((x) > 0) - ((x) < 0))
#define PGL_PIXEL(c,x,y) (c)->pixels[(x) + ((y) * (c)->width)]

// Colors
#define PGL_RGBA(r, g, b, a) ((((r)&0xFF)<<(8*0))   \
                              | (((g)&0xFF)<<(8*1)) \
                              | (((b)&0xFF)<<(8*2)) \
                              | (((a)&0xFF)<<(8*3)))
#define PGL_BLACK    PGL_RGBA(0, 0, 0, 1)
#define PGL_WHITE    PGL_RGBA(255, 255, 255, 1)
#define PGL_RED      PGL_RGBA(255, 0, 0, 1)
#define PGL_GREEN    PGL_RGBA(0, 255, 0, 1)
#define PGL_BLUE     PGL_RGBA(0, 0, 255, 1)
#define PGL_YELLOW   PGL_RGBA(255, 255, 0, 1)
#define PGL_MAGENTA  PGL_RGBA(255, 0, 255, 1)
#define PGL_CYAN     PGL_RGBA(0, 255, 255, 1)
#define PGL_SILVER   PGL_RGBA(192, 192, 192, 1)
#define PGL_GRAY     PGL_RGBA(128, 128, 128, 1)
#define PGL_DARKGRAY PGL_RGBA(64, 64, 64, 1)

typedef struct pgl_canvas {
    uint32_t *pixels;
    int width, height;
} pgl_canvas;

pgl_canvas *pgl_canvas_new(uint32_t *pixels, int width, int height);
void pgl_canvas_destroy(pgl_canvas *pc);

void pgl_set_pixel(pgl_canvas *pc, int x, int y, uint32_t color);
void pgl_fill(pgl_canvas *pc, uint32_t color);
void pgl_line(pgl_canvas *pc, int x1, int y1, int x2, int y2, uint32_t color);
void pgl_rect(pgl_canvas *pc, int x, int y, int w, int h, uint32_t color);
void pgl_rect_frame(pgl_canvas *pc, int x, int y, int w, int h, int thickness,
               uint32_t color);
void pgl_circle(pgl_canvas *pc, int center_x, int center_y, int r,
                uint32_t color);
void pgl_triangle(pgl_canvas *pc, int x1, int y1, int x2, int y2, int x3,
                  int y3, uint32_t color);
void pgl_triangle_frame(pgl_canvas *pc, int x1, int y1, int x2, int y2, int x3,
			int y3, int thickness, uint32_t color);
bool pgl_in_bounds(pgl_canvas *pc, int x, int y);

#endif // PGL_H

// =============================================================================
// Pants Graphics Library implementation
// =============================================================================

#define PGL_IMPLEMENTATION
#ifdef PGL_IMPLEMENTATION

#include <math.h>
#include <assert.h>
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

void pgl_canvas_destroy(pgl_canvas *pc) {
    free(pc->pixels);
    free(pc);
}

void pgl_set_pixel(pgl_canvas *pc, int x, int y, uint32_t color) {
    if (pgl_in_bounds(pc, x, y)) {
	PGL_PIXEL(pc, x, y) = color;
    }
}

void pgl_fill(pgl_canvas *pc, uint32_t color) {
    for (int i = 0; i < pc->width * pc->height; ++i) {
        pc->pixels[i] = color;
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

void pgl_rect(pgl_canvas *pc, int x, int y, int w, int h, uint32_t color) {
    for (int col = 0; col <= w; ++col) {
	for (int row = 0; row <= h; ++row) {
	    pgl_set_pixel(pc, col + x, row + y, color);
	}
    }
}

void pgl_rect_frame(pgl_canvas *pc, int x, int y, int w, int h, int thickness,
               uint32_t color) {
    pgl_rect(pc, x, y, w, thickness, color); // Top line
    pgl_rect(pc, x, y, thickness, h, color); // Left line
    pgl_rect(pc, x, y + h - thickness, w, thickness, color); // Bottom line
    pgl_rect(pc, x + w - thickness, y, thickness, h, color); // Right line
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

// NOTE: Looks like shit unless thickness 1
void pgl_triangle_frame(pgl_canvas *pc, int x1, int y1, int x2, int y2,
			int x3, int y3, int thickness, uint32_t color) {
    if (y1 > y2) {
	PGL_SWAP(int, x1, x2);
	PGL_SWAP(int, y1, y2);
    }
    
    if (y1 > y3) {
	PGL_SWAP(int, x1, x3);
	PGL_SWAP(int, y1, y3);
    }

    if (y2 > y3) {
	PGL_SWAP(int, x2, x3);
	PGL_SWAP(int, y2, y3);
    }

    // TODO: Fine tune the thickness
    for (int i = 0; i < thickness; ++i) {
	pgl_line(pc, x1 - i, y1, x2 - i, y2, color);
	pgl_line(pc, x1 - i, y1, x3 - i, y3, color);
	pgl_line(pc, x2, y2 + i, x3, y3 + i, color);
    }
}

void pgl_triangle(pgl_canvas *pc, int x1, int y1, int x2, int y2,
                  int x3, int y3, uint32_t color) {
    if (y1 > y2) {
	PGL_SWAP(int, x1, x2);
	PGL_SWAP(int, y1, y2);
    }
    
    if (y1 > y3) {
	PGL_SWAP(int, x1, x3);
	PGL_SWAP(int, y1, y3);
    }

    if (y2 > y3) {
	PGL_SWAP(int, x2, x3);
	PGL_SWAP(int, y2, y3);
    }

    // HACK: jesus fucking christ
    // FIXME: The whole thing with the tmp and the -1
    uint32_t tmp_pixels[pc->width * pc->height];
    pgl_canvas *tmp = pgl_canvas_new(tmp_pixels, pc->width, pc->height);
    pgl_fill(tmp, PGL_BLACK); // Clear the tmp pixels for the next triangle because for some god forsaken reason it carries over
    pgl_triangle_frame(tmp, x1, y1, x2, y2, x3, y3, 1, PGL_WHITE);
    for (int row = 0; row < tmp->height; ++row) {
	int x_left = -1;
	int x_right = -1;
	for (int col = 0; col < tmp->width; ++col) {
	    if (PGL_PIXEL(tmp, col, row) == PGL_WHITE) {
		if (x_left == -1) {
		    x_left = col;
		} else {
		    x_right = col;
		}
	    }
	}

	// HACK: for single pixel lines, only one side is -1
	/* printf("%d, %d\n", x_left, x_right); */
	if (x_right == -1 && x_left != -1) {
	    pgl_set_pixel(pc, x_left, row, color);
	} else if (x_left == -1 && x_right != -1) {
	    pgl_set_pixel(pc, x_right, row, color);
	} else if (x_left != -1 && x_right != -1) { // Both sides are found
	    pgl_line(pc, x_left, row, x_right, row, color);
	}
    }
}

bool pgl_in_bounds(pgl_canvas *pc, int x, int y) {
    return 0 <= x && x < (int)pc->width && 0 <= y && y < (int)pc->height;
}

#endif // PGL_IMPLEMENTATION
