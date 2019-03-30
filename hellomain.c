
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_timer.h"
#include "SDL_image.h"

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

//hastighet pixlar/sekund
#define SPEED (300)

int main()
{
    //initiering av grafik
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

    //skapa en renderare som ordnar hårdvaran
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //ladda in bild i minnet
    SDL_Surface* surface = IMG_Load("resources/hello.png");
    if (!surface)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //Ladda in data i grafikminnet
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

    //struct som styr position och storlek av sprite
    SDL_Rect dest;

    //få dimensioner av texturen och ändra dimensioner
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w /= 4;
    dest.h /= 4;

    //lägg sprite i mitten av fönster
    //koorinaten är högst upp till vänster i rektangeln
    float x_pos = (WINDOW_WIDTH - dest.w) /2;
    float y_pos = (WINDOW_HEIGHT - dest.h) /2;

    //ge sprite starthastighet
    float x_vel = 0;
    float y_vel = 0;

    //håll reda på inputs
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    //blir 1 när man vill stänga rutan
    int close_requested = 0;


    //animationsloop
    while (!close_requested)
    {
        // hantera events
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

        //hantera hastighet
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

        //uppdatera position
        x_pos += x_vel / 60;
        y_pos += y_vel /60;

        //hantera kollision
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


        //ange position i struct
        dest.y = (int) y_pos;
        dest.x = (int) x_pos;

        //rensa fönster
        SDL_RenderClear(rend);

        //rita bild till fönster
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend);

        //vänta en 60dels sekund
        SDL_Delay(1000/60);
    }

    //rensa fönster
    //SDL_RenderClear(rend);

    //rita bild till fönster
    //SDL_RenderCopy(rend, tex, NULL, NULL);
    //SDL_RenderPresent(rend);

    //vänta några sek
    //SDL_Delay(5000);


    //printf("initialization successful!\n");

    //städa resurser
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

