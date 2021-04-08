#include <stdio.h>
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_timer.h"
#include "SDL_image.h"

int main()
{
    //initiation of SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) !=0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Hello, world!",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       640, 480, 0);
    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //create renderer
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //load image into memory
    SDL_Surface* surface = IMG_Load("resources/hello.png");
    if (!surface)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //Create texture from image
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if(!tex)
    {
        printf("error creating texture: %s", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //clear window and then present image for 5 seconds
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, tex, NULL, NULL);
    SDL_RenderPresent(rend);
    SDL_Delay(5000);

    //let go of resources
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

