#include "SDL.h"
#include <stdio.h>
#include <math.h>

bool window_should_close = false;
void processEvents();

void drawScreen(SDL_Surface *screen);

int main(int argc, char *argv[])
{

    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "An SDL2 window",        // window title
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        640,                     // width, in pixels
        480,                     // height, in pixels
        SDL_WINDOW_OPENGL        // flags - see below
    );

    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    while (!window_should_close)
    {
        processEvents();
        // update state, draw the current frame

        // but instead of creating a renderer, we can draw directly to the screen
        SDL_Surface *screen = SDL_GetWindowSurface(window);
        // color: 0xaaRRGGBB
        // SDL_FillRect(screen, NULL, 0xff00ff00);

        drawScreen(screen);

        // this works just like SDL_Flip() in SDL 1.2
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void drawScreen(SDL_Surface *screen)
{
    SDL_LockSurface(screen);
    Uint32 secs = SDL_GetTicks()/1000;
    float scale = (sin(secs) / 2.0) + 0.5;
    unsigned char *data = (unsigned char *)screen->pixels;
    // unsigned char *dataPastEnd = data + screen->;
    unsigned int bytesPerPix = screen->format->BytesPerPixel;
    // fprintf(stdout,"bytes per pixel: %d\n",bytesPerPix);
    // red

    for (int y = 0; y < screen->h; ++y) // for each row
    {
        for (int x = 0; x < screen->w; x += 1, data += bytesPerPix)
        {

            Uint32 color = SDL_MapRGB(screen->format, 255 * scale, 0, 0);
            *(unsigned int *)data = color;
        }
    }
    SDL_UnlockSurface(screen);
}

void processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
        {
            window_should_close = true;
            break;
        }
        case SDL_KEYDOWN:
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                window_should_close = true;
            break;
        }
        }
    }
}