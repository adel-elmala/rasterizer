#include "SDL.h"
#include <stdio.h>
#include <math.h>
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

bool window_should_close = false;
int nPixelsx = 640;
int nPixelsy = 480;
void processEvents();
void drawPoint(SDL_Surface *screen, int x, int y, int r, int g, int b);
void rasterPoints(SDL_Surface *screen, const Vector3 &v);

int main(int argc, char *argv[])
{

    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "An SDL2 window",        // window title
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
    // SDL_Surface *screen = NULL;
    // Vector3 tp1 = Vector3(-49.0, -49.0, 0.0);
    Vector3 tp1 = Vector3(40.0,0.0,0.0);
    Vector3 tp2 = Vector3(40.0,40.0,0.0);
    Vector3 tp3 = Vector3(-40.0,0.0,0.0);
    rasterPoints(screen, tp1);
    rasterPoints(screen,tp2);
    rasterPoints(screen,tp3);
    while (!window_should_close)
    {
        processEvents();
        // update state, draw the current frame

        // color: 0xaaRRGGBB
        // SDL_FillRect(screen, NULL, 0xff00ff00);

        // drawScreen(screen);

        // this works just like SDL_Flip() in SDL 1.2
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void drawPoint(SDL_Surface *screen, int x, int y, int r, int g, int b)
{
    // SDL_LockSurface(screen);
    unsigned char *data = (unsigned char *)screen->pixels;
    unsigned int bytesPerPix = screen->format->BytesPerPixel;
    unsigned char *targetPixel = data + (y * screen->pitch) + (x * bytesPerPix);
    // fprintf(stdout,"bytes per pixel: %d\n",bytesPerPix);

    // red
    Uint32 color = SDL_MapRGB(screen->format, r, g, b);
    *(unsigned int *)targetPixel = color;
    *(((unsigned int *)targetPixel) + 1) = color;
    *(((unsigned int *)targetPixel) - 1) = color;
    *(((unsigned int *)targetPixel) + screen->w) = color;
    *(((unsigned int *)targetPixel) - screen->w) = color;

    // SDL_UnlockSurface(screen);
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

void rasterPoints(SDL_Surface *screen, const Vector3 &vec)
{
    // 1 - read vertex in model coordinates. Vm
    //      1.1 read 3d coord then transform to 4d
    Vector4 hv(vec, 1.0);

    // 2 - transform to world coord
    //      2.1 - construct the homog. linear trans model-to-world matrix Mw
    Matrix4 Mw;
    // Mw.col1 = Vector4(1.0, 0.0, 0.0, 0.0);
    // Mw.col2 = Vector4(0.0, 1.0, 0.0, 0.0);
    // Mw.col3 = Vector4(0.0, 0.0, 1.0, 0.0);
    Mw.col4 = Vector4(0.0, 0.0, -50.0, 1.0); // translate in the -ve z direction 50 units

    //      2.2 - multyply the vertex by Mw : Mw*Vm (now in world coord)
    hv = Mw * hv;
    // 3 - transform to camera coord
    //      3.1 - constuct the camera coord sys, u,v,w & e
    Vector3 e = Vector3(0.0); // camera eye at world origin

    Vector3 cameraGazeDir = Vector3(0.0, 0.0, -1.0); // -z dir
    Vector3 cameraUpDir = Vector3(0.0, 1.0, 0.0);    // y dir
    Vector3 w = (-cameraGazeDir);
    w.normalize(); // done
    Vector3 v = cameraUpDir;
    v.normalize();
    Vector3 u = (v ^ w);
    u.normalize(); // done
    v = (w ^ u).hat();

    //      3.2 - consturct the world-to-camera matrix Mc
    Matrix4 Mc;
    Matrix4 Mct;
    Mc.col1 = Vector4(u.m_x, v.m_x, w.m_x, 0.0);
    Mc.col2 = Vector4(u.m_y, v.m_y, w.m_y, 0.0);
    Mc.col3 = Vector4(u.m_z, v.m_z, w.m_z, 0.0);
    Mc.col4 = Vector4(0.0, 0.0, 0.0, 1.0);
    Mct.col4 = Vector4(-e.m_x, -e.m_x, -e.m_x, 1.0);
    Mc = Mc * Mct;
    //      3.3 - multyply the vertex by Mx : Mc*Mw*Vm (now in cam coord)
    hv = Mc * hv;

    // 4 - trasnform to NDC
    //      4.1 - using orthographic projection , define the orthographic volume (i.e : l,r,t,b,n,f) , but skip clipping for now
    double l = -50, r = 50;
    double t = 50, b = -50;
    double n = -50, f = -150;

    //      4.2 - constuct the ortho.g. matrix Mo (camera-to-NDC): Mo * Mc * Mm * Vm (now in NDC coord)
    Matrix4 Mo;
    Mo.col1 = Vector4(2.0 / (r - l), 0.0, 0.0, 0.0);
    Mo.col2 = Vector4(0.0, 2.0 / (t - b), 0.0, 0.0);
    Mo.col3 = Vector4(0.0, 0.0, 2.0 / (n - f), 0.0);
    Mo.col4 = Vector4(-(r + l) / (r - l), -(t + b) / (t - b), -(n + f) / (n - f), 1.0);
    hv = Mo * hv;
    // 5 - transform to viewport coord
    //      5.1 - constuct the NDC-to-viewPort matrix Mv: Mv * Mo * Mc * Mm * Vm (now in pixel coord)
    Matrix4 Mv;
    Matrix4 MvFlipV;
    Mv.col1 = Vector4(nPixelsx / 2, 0.0, 0.0, 0.0);
    Mv.col2 = Vector4(0.0, nPixelsy / 2, 0.0, 0.0);
    Mv.col4 = Vector4((nPixelsx - 1) / 2, (nPixelsy - 1) / 2, 0.0, 1.0);
    MvFlipV.col2 = Vector4(0.0, -1.0, 0.0, 0.0);
    hv = Mv * MvFlipV * hv;
    // 6 - draw pixel with vetrex color
    drawPoint(screen, hv.m_x, hv.m_y, 255, 0, 0);
}
