
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_timer.h"
#include "SDL_image.h"

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

//velocity in pixels/second
#define SPEED (300)

int main()
{
    //initialize SDL
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

    //Load image to texture
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

    //rectangle to present the texture on
    SDL_Rect dest;

    //shrink image
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w /= 4;
    dest.h /= 4;

    //put the image in the center of the screen
    float x_pos = (WINDOW_WIDTH - dest.w) /2;
    float y_pos = (WINDOW_HEIGHT - dest.h) /2;

    //start velocity
    float x_vel = 0;
    float y_vel = 0;

    //variables that keep track of inputs
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;
    int close_requested = 0;


    //"game loop"
    while (!close_requested)
    {
        // handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                {
                   close_requested = 1;
                }
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        up = 1;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        left = 1;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        down = 1;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        right = 1;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        up = 0;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        left = 0;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        down = 0;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        right = 0;
                        break;
                }
                break;
            }
        }

        //handle velocity
        x_vel = y_vel = 0;
        if (up && !down)
        {
            y_vel = -SPEED;
        }
        if (down && !up)
        {
            y_vel = SPEED;
        }
        if (left && !right)
        {
            x_vel = -SPEED;
        }
        if (right && !left)
        {
            x_vel = SPEED;
        }

        //update position of image
        x_pos += x_vel / 60;
        y_pos += y_vel /60;

        //handle collisions with boundaries
        if (x_pos <= 0)
        {
            x_pos = 0;
            printf("hit left\n");
        }
        if (y_pos <=0)
        {
            y_pos=0;
            printf("hit top\n");
        }
        if (x_pos >= WINDOW_WIDTH - dest.w)
        {
            x_pos = WINDOW_WIDTH - dest.w;
            printf("hit right\n");
        }
        if (y_pos >= WINDOW_HEIGHT - dest.h)
        {
            y_pos = WINDOW_HEIGHT - dest.h;
            printf("hit bottom\n");
        }


        //set the new position to the image
        dest.y = (int) y_pos;
        dest.x = (int) x_pos;

        //update window with new position
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend);

        //Set framerate to roughly 60fps (can be improved with SDL_GetTicks() for larger game loops)
        SDL_Delay(1000/60);
    }

    //let go of resources and close application
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

