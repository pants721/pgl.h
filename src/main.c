#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#include "pgl.h"

#define WIN_W 1280
#define WIN_H 960

static uint32_t pixels[WIN_W * WIN_H];

int main(int argc, char **argv) {
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

        pgl_clear(pc);

        pgl_triangle(pc, 100, 100, 500, 500, 700, 300, PGL_BLUE);
        pgl_circle(pc, tick, 100, 500, PGL_GREEN);

        SDL_UpdateTexture(texture, NULL, pc->pixels, WIN_W * 4);
        SDL_RenderCopyEx(renderer, texture, NULL, NULL, 0.0, NULL,
                         SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);

        tick++;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
