#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"

#define PGL_IMPLEMENTATION
#include "pgl.h"

#define WIN_W 1920
#define WIN_H 1080

static uint32_t pixels[WIN_W * WIN_H];

int main() {
    pgl_canvas *pc = pgl_canvas_new(pixels, WIN_W, WIN_H);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "Mia", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, 0);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                          SDL_TEXTUREACCESS_STREAMING, WIN_W, WIN_H);

    bool quit = false;
    int tick = 0;
    while (!quit) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                quit = true;
                break;
            }
        }

        pgl_fill(pc, PGL_DARKGRAY);

        pgl_triangle(pc, 123, 125, 390, 400, 1, 230, PGL_GREEN);
        pgl_triangle(pc, 100, 100, 500, 500, 700, 300, PGL_BLUE);
        pgl_circle(pc, tick, 400, 400, PGL_RGBA(120, 0, 120, 100));
        pgl_rect(pc, 50, 800 - tick, 75, WIN_H - (800 - tick), PGL_BLUE);
        // pgl_rect(pc, 50, 800 - tick, 75, 5000, PGL_RED);
        // pgl_triangle(pc, 800, 800, 250, 1, 300, 500, PGL_MAGENTA);
        // pgl_triangle_frame(pc, 123, 125, 390, 400, 1, 230, 1, PGL_RED);
        // pgl_triangle_frame(pc, 100, 100, 500, 500, 700, 300, 1, PGL_CYAN);
        // pgl_triangle(pc, 700, 700, 1100, 600, 600, 600, PGL_BLUE);

        SDL_UpdateTexture(texture, NULL, pixels, WIN_W * 4);
        SDL_RenderCopyEx(renderer, texture, NULL, NULL, 0.0, NULL,
                         SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);

        tick++;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return EXIT_SUCCESS;
}
