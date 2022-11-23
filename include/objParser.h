#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <ctype.h>

#include "Vector3.h"
#include "RGBColor.h"

struct Vertex
{
    float x, y, z, w = 1.0;
};
struct Normal
{
    float x, y, z;
};
struct FaceIndicies
{
    unsigned int v1, v2, v3;
};
struct vertexAttrib
{
    Vector3 pos;
    RGBColor col;
};

struct triangle
{
    vertexAttrib v1;
    vertexAttrib v2;
    vertexAttrib v3;
    Vector3 tNorm;
    Vector3 tNorm2;
    Vector3 tNorm3;
    int nNorms;
};

class objParser
{
public:
    unsigned int nVerts;
    unsigned int nNorms;
    unsigned int nFaces;
    char *fileBuffer;
    unsigned long fileSizeBytes;
    std::vector<Vertex> verticies;
    std::vector<Vector3> normals;
    std::vector<FaceIndicies> FaceIdx;
    std::vector<triangle> triangles;

public:
    objParser(const char *pathname);
    ~objParser();
    void ReadFile(const char *pathname);
    char *processVLine(char *lineStart);
    char *processVnLine(char *lineStart);
    char *processFLine(char *lineStart);
    void formTriangles(void);
};

#endif