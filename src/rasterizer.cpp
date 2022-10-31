#include "SDL.h"
#include <stdio.h>
#include <math.h>
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "RGBColor.h"

#include <sys/timeb.h>

struct vertexAttrib
{
    Vector3 pos;
    RGBColor col;
};

struct line
{
    vertexAttrib p1;
    vertexAttrib p2;
};

struct triangle
{
    vertexAttrib v1;
    vertexAttrib v2;
    vertexAttrib v3;
};

bool window_should_close = false;
int nPixelsx = 640;
int nPixelsy = 480;
double r = 0;
void processEvents();
void drawPoint(SDL_Surface *screen, const Vector3 &hVec, int r, int g, int b);
Vector3 processVertex(const Vector3 &v);
void rasterTriangle(SDL_Surface *screen, const triangle &t);

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
    Vector3 tp1 = Vector3(30.0, -10.0, 0.0);
    Vector3 tp2 = Vector3(0.0, 40.0, 0.0);
    Vector3 tp3 = Vector3(-30.0, -10.0, 0.0);
    triangle t;
    t.v1.pos = tp1;
    t.v2.pos = tp2;
    t.v3.pos = tp3;
    struct timeb tp;
    while (!window_should_close)
    {

        ftime(&tp);
        r = (sin(tp.millitm / 100) / 2.0) + 0.5;
        t.v1.col = RGBColor(r, 0.0, 0.0);
        t.v2.col = RGBColor(0.0, r, 0.0);
        t.v3.col = RGBColor(0, 0.0, r);

        rasterTriangle(screen, t);
        // update state, draw the current frame
        processEvents();

        // this works just like SDL_Flip() in SDL 1.2
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void drawPoint(SDL_Surface *screen, const Vector3 &hVec, int r, int g, int b)
{
    // SDL_LockSurface(screen);
    int x = hVec.m_x;
    int y = hVec.m_y;
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
            if (event.key.keysym.sym == SDLK_w)
            {
                r = r + 0.01;
                if (r > 1.0)
                    r = 0;
                printf("%f\n", r);
            }

            break;
        }
        }
    }
}

Vector3 processVertex(const Vector3 &vec)
{
    // 1 - read vertex in model coordinates. Vm
    //      1.1 read 3d coord then transform to 4d
    Vector4 hv(vec, 1.0);

    // 2 - transform to world coord
    //      2.1 - construct the homog. linear trans model-to-world matrix Mw
    Matrix4 Mw;
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
    Vector3 vec_in_screen_coord(hv.m_x, hv.m_y, hv.m_z);
    return vec_in_screen_coord; // the z-coord will be used for z-buffering later
}

double implicit_2d_line_eq(const Vector3 &p1, const Vector3 &p2, const Vector3 &v)
{
    return ((p1.m_y - p2.m_y) * v.m_x + (p2.m_x - p1.m_x) * v.m_y + (p1.m_x * p2.m_y) - (p2.m_x * p1.m_y));
}
// t is in model coord.
void rasterTriangle(SDL_Surface *screen, const triangle &t)
{
    triangle t_in_screen_space = t;
    t_in_screen_space.v1.pos = processVertex(t.v1.pos);
    t_in_screen_space.v2.pos = processVertex(t.v2.pos);
    t_in_screen_space.v3.pos = processVertex(t.v3.pos);

    double x_max, x_min;
    double y_max, y_min;
    double z = t_in_screen_space.v1.pos.m_z;
    x_max = t_in_screen_space.v1.pos.m_x;
    x_max = x_max > t_in_screen_space.v2.pos.m_x ? x_max : t_in_screen_space.v2.pos.m_x;
    x_max = x_max > t_in_screen_space.v3.pos.m_x ? x_max : t_in_screen_space.v3.pos.m_x;

    y_max = t_in_screen_space.v1.pos.m_y;
    y_max = y_max > t_in_screen_space.v2.pos.m_y ? y_max : t_in_screen_space.v2.pos.m_y;
    y_max = y_max > t_in_screen_space.v3.pos.m_y ? y_max : t_in_screen_space.v3.pos.m_y;

    x_min = t_in_screen_space.v1.pos.m_x;
    x_min = x_min < t_in_screen_space.v2.pos.m_x ? x_min : t_in_screen_space.v2.pos.m_x;
    x_min = x_min < t_in_screen_space.v3.pos.m_x ? x_min : t_in_screen_space.v3.pos.m_x;

    y_min = t_in_screen_space.v1.pos.m_y;
    y_min = y_min < t_in_screen_space.v2.pos.m_y ? y_min : t_in_screen_space.v2.pos.m_y;
    y_min = y_min < t_in_screen_space.v3.pos.m_y ? y_min : t_in_screen_space.v3.pos.m_y;

    for (int y = y_min; y <= y_max; ++y)
    {
        for (int x = x_min; x <= x_max; ++x)
        {
            Vector3 p(x, y, z);
            double alpha = implicit_2d_line_eq(t_in_screen_space.v2.pos, t_in_screen_space.v3.pos, p) / implicit_2d_line_eq(t_in_screen_space.v2.pos, t_in_screen_space.v3.pos, t_in_screen_space.v1.pos);

            double beta = implicit_2d_line_eq(t_in_screen_space.v3.pos, t_in_screen_space.v1.pos, p) / implicit_2d_line_eq(t_in_screen_space.v3.pos, t_in_screen_space.v1.pos, t_in_screen_space.v2.pos);

            double gamma = implicit_2d_line_eq(t_in_screen_space.v1.pos, t_in_screen_space.v2.pos, p) / implicit_2d_line_eq(t_in_screen_space.v1.pos, t_in_screen_space.v2.pos, t_in_screen_space.v3.pos);

            if ((alpha >= 0) && (beta >= 0) && (gamma >= 0))
            {
                RGBColor c = ((RGBColor(t_in_screen_space.v1.col)) * alpha) +
                             (RGBColor((t_in_screen_space.v2.col)) * beta) +
                             (RGBColor((t_in_screen_space.v3.col)) * gamma);
                drawPoint(screen, p, int(c.r * 255), int(c.g * 255), int(c.b * 255));
            }
        }
    }
    // printf("done\n");
}
