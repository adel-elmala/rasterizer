#include "rasterizer.h"
#include <string.h>

extern bool window_should_close;
extern int nPixelsx;
extern int nPixelsy;
Matrix4 Mw, Mc, Mndc, Mvp, M_model_screen;
double *zBuffer = NULL;

void drawPoint(SDL_Surface *screen, const Vector3 &hVec, int r, int g, int b)
{
    // SDL_LockSurface(screen);
    int x = hVec.m_x;
    int y = hVec.m_y;
    if ((x > nPixelsx) || (y > nPixelsy) || (x < 0) || (y < 0))
        return; // fix out of bounds memory writes
    unsigned char *data = (unsigned char *)screen->pixels;
    unsigned int bytesPerPix = screen->format->BytesPerPixel;
    unsigned char *targetPixel = data + (y * screen->pitch) + (x * bytesPerPix);
    // fprintf(stdout,"bytes per pixel: %d\n",bytesPerPix);

    // red
    Uint32 color = SDL_MapRGB(screen->format, r, g, b);
    *(unsigned int *)targetPixel = color;
    // *(((unsigned int *)targetPixel) + 1) = color;
    // *(((unsigned int *)targetPixel) - 1) = color;
    // *(((unsigned int *)targetPixel) + screen->w) = color;
    // *(((unsigned int *)targetPixel) - screen->w) = color;

    // SDL_UnlockSurface(screen);
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
            if ((x > nPixelsx) || (y > nPixelsy) || (x < 0) || (y < 0))
                return; // fix out of bounds memory writes

            Vector3 p(x, y, z); // filled in the z value  -- dummy value

            double alpha = implicit_2d_line_eq(t_in_screen_space.v2.pos, t_in_screen_space.v3.pos, p) / implicit_2d_line_eq(t_in_screen_space.v2.pos, t_in_screen_space.v3.pos, t_in_screen_space.v1.pos);

            double beta = implicit_2d_line_eq(t_in_screen_space.v3.pos, t_in_screen_space.v1.pos, p) / implicit_2d_line_eq(t_in_screen_space.v3.pos, t_in_screen_space.v1.pos, t_in_screen_space.v2.pos);

            double gamma = implicit_2d_line_eq(t_in_screen_space.v1.pos, t_in_screen_space.v2.pos, p) / implicit_2d_line_eq(t_in_screen_space.v1.pos, t_in_screen_space.v2.pos, t_in_screen_space.v3.pos);

            if ((alpha >= 0) && (beta >= 0) && (gamma >= 0))
            {
                Vector3 tmp = t_in_screen_space.v1.pos * alpha +
                              t_in_screen_space.v2.pos * beta +
                              t_in_screen_space.v3.pos * gamma;

                // // Z-buffer Test
                double *pZValue = (zBuffer + (((int)p.m_y) * nPixelsx) + (int)p.m_x);
                if (tmp.m_z >= *pZValue)
                // if (true)
                {
                    *pZValue = p.m_z; // update z Value for that pixel;

                    RGBColor c = ((RGBColor(t_in_screen_space.v1.col)) * alpha) +
                                 (RGBColor((t_in_screen_space.v2.col)) * beta) +
                                 (RGBColor((t_in_screen_space.v3.col)) * gamma);
                    drawPoint(screen, p, int(c.r * 255), int(c.g * 255), int(c.b * 255));
                }
            }
        }
    }
    // printf("done\n");
}

// p1 & p2 are in screen space
void drawLine(SDL_Surface *screen, const Vector3 &p1, const Vector3 &p2, int r, int g, int b)
{
    Vector3 lp = p1.m_x <= p2.m_x ? p1 : p2;
    Vector3 rp = p1.m_x <= p2.m_x ? p2 : p1;
    float m = (lp.m_y - rp.m_y) / (lp.m_x - rp.m_x);
    if ((m == INFINITY) || (m == -INFINITY))
    {
        Vector3 bp = p1.m_y <= p2.m_y ? p1 : p2;
        Vector3 up = p1.m_y <= p2.m_y ? p2 : p1;

        for (double y = bp.m_y, x = bp.m_x; y <= up.m_y; ++y) // loop on rows
        {
            Vector3 tmp(x, y, lp.m_z);
            // Vector3 midPoint(x + 0.5, y + 1, lp.m_z);

            drawPoint(screen, tmp, r, g, b);
            // if (implicit_2d_line_eq(lp, rp, midPoint) > 0)
            //     ++x;
        }
    }
    if ((m > 0) && (m <= 1)) // slope m(0,1]
    {
        for (double x = lp.m_x, y = lp.m_y; x <= rp.m_x; ++x)
        {
            Vector3 tmp(x, y, lp.m_z);
            Vector3 midPoint(x + 1, y + 0.5, lp.m_z);

            drawPoint(screen, tmp, r, g, b);
            if (implicit_2d_line_eq(lp, rp, midPoint) < 0) // if line passes above the midpoint test
                ++y;                                       // move up
        }
    }

    else if ((m > -1) && (m <= 0)) // slope m(-1,0]
    {
        for (double x = lp.m_x, y = lp.m_y; x <= rp.m_x; ++x)
        {
            Vector3 tmp(x, y, lp.m_z);
            Vector3 midPoint(x + 1, y - 0.5, lp.m_z);

            drawPoint(screen, tmp, r, g, b);
            if (implicit_2d_line_eq(lp, rp, midPoint) > 0) // if line passes below the midpoint test
                --y;                                       // move down
        }
    }

    else if ((m > 1) && (m < INFINITY)) // slope m(1,inf]
    {
        for (double y = lp.m_y, x = lp.m_x; y <= rp.m_y; ++y) // loop on rows
        {
            Vector3 tmp(x, y, lp.m_z);
            Vector3 midPoint(x + 0.5, y + 1, lp.m_z);

            drawPoint(screen, tmp, r, g, b);
            if (implicit_2d_line_eq(lp, rp, midPoint) > 0)
                ++x;
        }
    }

    else if ((m > -INFINITY) && (m <= -1)) // slope m(-inf,-1]
    {
        for (double y = lp.m_y, x = lp.m_x; y >= rp.m_y; --y) // loop on rows
        {
            Vector3 tmp(x, y, lp.m_z);
            Vector3 midPoint(x + 0.5, y - 1, lp.m_z);

            drawPoint(screen, tmp, r, g, b);
            if (implicit_2d_line_eq(lp, rp, midPoint) < 0)
                ++x;
        }
    }
}

// t is in model coord.
void rasterWireFrameTriangle(SDL_Surface *screen, const triangle &t)
{
    triangle t_in_screen_space = t;
    t_in_screen_space.v1.pos = processVertex(t.v1.pos);
    t_in_screen_space.v2.pos = processVertex(t.v2.pos);
    t_in_screen_space.v3.pos = processVertex(t.v3.pos);

    // pretty_print(t);
    // pretty_print(t_in_screen_space);
    // line 1 v1-v2

    // p1 & p2 are in screen space
    drawLine(screen, t_in_screen_space.v1.pos, t_in_screen_space.v2.pos, 255, 255, 0);

    // line 2 v1-v3
    drawLine(screen, t_in_screen_space.v1.pos, t_in_screen_space.v3.pos, 255, 255, 0);

    // line 3 v2-v3
    drawLine(screen, t_in_screen_space.v2.pos, t_in_screen_space.v3.pos, 255, 255, 0);

    // printf("done\n");
}

void pretty_print(triangle t)
{
    fprintf(stdout, "\t\t{%f\t%f\t%f}\n", t.v1.pos.m_x, t.v1.pos.m_y, t.v1.pos.m_z);
    fprintf(stdout, "\t\t{%f\t%f\t%f}\n", t.v2.pos.m_x, t.v2.pos.m_y, t.v2.pos.m_z);
    fprintf(stdout, "\t\t{%f\t%f\t%f}\n\n", t.v3.pos.m_x, t.v3.pos.m_y, t.v3.pos.m_z);
}

Vector3 processVertex(const Vector3 &vec)
{
    // read vertex in model coordinates. Vm
    // read 3d coord then transform to 4d
    Vector4 hv(vec, 1.0);

    hv = M_model_screen * hv;

    Vector3 vec_in_screen_coord(hv.m_x / hv.m_w, hv.m_y / hv.m_w, hv.m_z / hv.m_w);
    return vec_in_screen_coord; // the z-coord will be used for z-buffering later
}
void constructWorldMat(Matrix4 &Mw, Vector3 scale, Vector3 translate, double rotate)
{

    // 2 - transform to world coord
    //      2.1 - construct the homog. linear trans model-to-world matrix Mw
    Matrix4 Ms;
    Ms.col1.m_x = scale.m_x;
    Ms.col2.m_y = scale.m_y;
    Ms.col3.m_z = scale.m_z;
    Matrix4 Mr_y;
    Mr_y.col1 = Vector4(cos(rotate), 0.0, -sin(rotate), 0.0);
    Mr_y.col2 = Vector4(0.0, 1.0, 0.0, 0.0);
    Mr_y.col3 = Vector4(sin(rotate), 0.0, cos(rotate), 0.0);
    Mr_y.col4 = Vector4(0.0, 0.0, 0.0, 1.0);
    Matrix4 Mt;
    Mt.col4 = Vector4(translate, 1.0); // translate in the -ve z direction 50 units

    Mw = Mt * Mr_y * Ms;
}

void constructCamMat(Matrix4 &Mc, Vector3 eye, Vector3 gazeDir, Vector3 up)
{
    // 3 - transform to camera coord
    //      3.1 - constuct the camera coord sys, u,v,w & e

    Vector3 w = (-gazeDir);
    w.normalize(); // done
    Vector3 v = up;
    v.normalize();
    Vector3 u = (v ^ w);
    u.normalize(); // done
    v = (w ^ u).hat();

    //      3.2 - consturct the world-to-camera matrix Mc
    Matrix4 Mcam;
    Matrix4 Mct;
    Mcam.col1 = Vector4(u.m_x, v.m_x, w.m_x, 0.0);
    Mcam.col2 = Vector4(u.m_y, v.m_y, w.m_y, 0.0);
    Mcam.col3 = Vector4(u.m_z, v.m_z, w.m_z, 0.0);
    Mcam.col4 = Vector4(0.0, 0.0, 0.0, 1.0);
    Mct.col4 = Vector4(-eye.m_x, -eye.m_y, -eye.m_z, 1.0);
    Mc = Mcam * Mct;
}

void constructNDCMat(Matrix4 &Mndc, view_volume_bounds vvb, bool prespective)
{

    // 4 - trasnform to NDC
    //      4.1 - using orthographic projection , define the orthographic volume (i.e : l,r,t,b,n,f) , but skip clipping for now
    double l = vvb.l, r = vvb.r;
    double t = vvb.t, b = vvb.b;
    double n = vvb.n, f = vvb.f;

    //      4.2 - constuct the ortho.g. matrix Mo (camera-to-NDC): Mo * Mc * Mm * Vm (now in NDC coord)
    Matrix4 Mo;
    Mo.col1 = Vector4(2.0 / (r - l), 0.0, 0.0, 0.0);
    Mo.col2 = Vector4(0.0, 2.0 / (t - b), 0.0, 0.0);
    Mo.col3 = Vector4(0.0, 0.0, 2.0 / (n - f), 0.0);
    Mo.col4 = Vector4(-(r + l) / (r - l), -(t + b) / (t - b), -(n + f) / (n - f), 1.0);
    Matrix4 Mp;
    Mp.col1 = Vector4(n, 0.0, 0.0, 0.0);
    Mp.col2 = Vector4(0.0, n, 0.0, 0.0);
    Mp.col3 = Vector4(0.0, 0.0, (n + f), 1.0);
    Mp.col4 = Vector4(0, 0, -(f * n), 0.0);

    if (prespective)

        Mndc = Mo * Mp;
    else
        Mndc = Mo;
}

void constructViewPortMat(Matrix4 &Mvp, unsigned int winWidth, unsigned int winHeight)
{
    // 5 - transform to viewport coord
    //      5.1 - constuct the NDC-to-viewPort matrix Mv: Mv * Mo * Mc * Mm * Vm (now in pixel coord)
    Matrix4 Mv;
    Matrix4 MvFlipV;
    Mv.col1 = Vector4(winWidth / 2, 0.0, 0.0, 0.0);
    Mv.col2 = Vector4(0.0, winHeight / 2, 0.0, 0.0);
    Mv.col4 = Vector4((winWidth - 1) / 2, (winHeight - 1) / 2, 0.0, 1.0);
    MvFlipV.col2 = Vector4(0.0, -1.0, 0.0, 0.0);
    Mvp = Mv * MvFlipV;
}

void init_Model_to_screen_mat()
{
    view_volume_bounds vvbInit;
    constructWorldMat(Mw, Vector3(1.0), Vector3(0.0, 0.0, -120.0), 0.0);
    constructCamMat(Mc, Vector3(0.0), Vector3(0.0, 0.0, -1.0), Vector3(0.0, 1.0, 0.0));
    constructNDCMat(Mndc, vvbInit, true);
    constructViewPortMat(Mvp, nPixelsx, nPixelsy);
    M_model_screen = Mvp * Mndc * Mc * Mw;
}

void update_Model_to_screen_mat()
{
    M_model_screen = Mvp * Mndc * Mc * Mw;
}

void initZBuffer(unsigned int winWidth, unsigned int winHeight)
{
    unsigned int nBytes = sizeof(double) * winWidth * winHeight;
    zBuffer = (double *)malloc(nBytes);
    // clearZBuffer(winWidth, winHeight);
}
