#ifndef PGL_H
#define PGL_H

#include <stdlib.h>

typedef struct {
    uint32_t *pixels;
    int width, height;
} pgl_canvas;

pgl_canvas *pgl_canvas_new(uint32_t *pixels, int width, int height);
void pgl_set_pixel(pgl_canvas *pc, int x, int y, uint32_t color);
void pgl_draw_line(pgl_canvas *pc, int x1, int y1, int x2, int y2,
                   uint32_t color);

#endif // PGL_H
