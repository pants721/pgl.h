#ifndef PGL_H
#define PGL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PGL_MIN(a,b) (((a)<(b))?(a):(b))
#define PGL_MAX(a,b) (((a)>(b))?(a):(b))

// Colors
#define PGL_RGBA(r, g, b, a) ((((r)&0xFF)<<(8*0)) | (((g)&0xFF)<<(8*1)) | (((b)&0xFF)<<(8*2)) | (((a)&0xFF)<<(8*3)))
#define PGL_BLACK    PGL_RGBA(0, 0, 0, 1)
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
void pgl_set_pixel(pgl_canvas *pc, int x, int y, uint32_t color);
void pgl_clear(pgl_canvas *pc);
void pgl_line(pgl_canvas *pc, int x1, int y1, int x2, int y2, uint32_t color);
void pgl_circle(pgl_canvas *pc, int center_x, int center_y, int r, uint32_t color);
void pgl_triangle(pgl_canvas *pc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);

bool pgl_in_bounds(pgl_canvas *pc, int x, int y);

#endif // PGL_H
