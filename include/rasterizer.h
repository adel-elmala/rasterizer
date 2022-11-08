#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <stdio.h>
#include <math.h>

#include "SDL.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "RGBColor.h"
#include "objParser.h"

struct view_volume_bounds
{
    double l = -100, r = 100;
    double t = 100, b = -100;
    double n = -50, f = -250;
};
extern Matrix4 Mw, Mc, Mndc, Mvp, M_model_screen;

void pretty_print(triangle t);

void drawPoint(SDL_Surface *screen, const Vector3 &hVec, int r, int g, int b);
Vector3 processVertex(const Vector3 &v);

void rasterTriangle(SDL_Surface *screen, const triangle &t);
void rasterWireFrameTriangle(SDL_Surface *screen, const triangle &t);

void constructWorldMat(Matrix4 &Mw, Vector3 scale, Vector3 translate, double rotate);
void constructCamMat(Matrix4 &Mc, Vector3 eye, Vector3 gazeDir, Vector3 up);
void constructNDCMat(Matrix4 &Mndc, view_volume_bounds vvb, bool prespective);
void constructViewPortMat(Matrix4 &Mvp, unsigned int winWidth, unsigned int winHeight);

void init_Model_to_screen_mat();
void update_Model_to_screen_mat();

#endif