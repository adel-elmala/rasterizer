#include "rasterizer.h"

bool window_should_close = false;
int nPixelsx = 640;
int nPixelsy = 480;

int main(int argc, char *argv[])
{

    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Rasterizer",            // window title
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        nPixelsx,                // width, in pixels
        nPixelsy,                // height, in pixels
        SDL_WINDOW_OPENGL        // flags - see below
    );

    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // // but instead of creating a renderer, we can draw directly to the screen
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    Vector3 tp1 = Vector3(1.0, 1.0, 0.0);
    Vector3 tp2 = Vector3(1.0, -1.0, 0.0);
    Vector3 tp3 = Vector3(-1.0, -1.0, 0.0);
    triangle t;
    t.v1.pos = tp1;
    t.v2.pos = tp2;
    t.v3.pos = tp3;
    t.v1.col = RGBColor(1.0, 0.0, 0.0);
    t.v2.col = RGBColor(0.0, 1.0, 0.0);
    t.v3.col = RGBColor(0, 0.0, 1.0);

    Vector3 t2p1 = Vector3(1.0, 1.0, 0.0);
    Vector3 t2p2 = Vector3(-1.0, -1.0, 0.0);
    Vector3 t2p3 = Vector3(-1.0, 1.0, 0.0);
    triangle t2;
    t2.v1.pos = t2p1;
    t2.v2.pos = t2p2;
    t2.v3.pos = t2p3;
    t2.v1.col = RGBColor(1.0, 0.0, 0.0);
    t2.v2.col = RGBColor(0.0, 0.0, 1.0);
    t2.v3.col = RGBColor(0, 1.0, 0.0);
    // rasterWireFrameTriangle(screen, t);
    // rasterWireFrameTriangle(screen, t2);
    // rasterTriangle(screen, t);
    // rasterTriangle(screen, t2);

    // objParser parser("./objFiles/teapot.obj");
    objParser parser("./objFiles/cow.obj");
    fprintf(stdout, "OBJ PARSER: done\n");

    char title[512];
    title[512] = '\0';
    init_Model_to_screen_mat();
    while (!window_should_close)
    {
        // clear screen
        SDL_FillRect(screen, NULL, 0x000000);
        uint32_t sTime = SDL_GetTicks();

        // update state, draw the current frame
        processEvents();
        constructWorldMat(Mw, Vector3(15.0), Vector3(0.0,0.0,-120.0), 0.0);
        update_Model_to_screen_mat();

        for (const auto t : parser.triangles)
        {
            // rasterTriangle(screen,t);
            rasterWireFrameTriangle(screen, t);
        }

        // this works just like SDL_Flip() in SDL 1.2
        SDL_UpdateWindowSurface(window);
        uint32_t frameTime = SDL_GetTicks() - sTime;
        float fps = frameTime > 0 ? (1000.0f / frameTime) : 0;
        snprintf(title, 512, "FPS: %.1f .", fps);

        SDL_SetWindowTitle(window, title);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
