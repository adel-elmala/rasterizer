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

class objParser
{
private:
    unsigned int nVerts;
    unsigned int nNorms;
    unsigned int nFaces;
    char *fileBuffer;
    long fileSizeBytes;
    std::vector<Vertex> verticies;
    std::vector<Normal> normals;
    std::vector<FaceIndicies> FaceIdx;

public:
    objParser(const char *pathname);
    ~objParser();
    void ReadFile(const char *pathname);
    char *processVLine(char *lineStart);
    char *processVnLine(char *lineStart);
    char *processFLine(char *lineStart);
};

void objParser::ReadFile(const char *pathname)
{
    FILE *fd = fopen(pathname, "r");
    if (fd == NULL)
    {
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    fseek(fd, 0L, SEEK_END);

    fileSizeBytes = ftell(fd);

    rewind(fd);
    fileBuffer = (char *)malloc(fileSizeBytes + 1);
    fileBuffer[fileSizeBytes] = '\0'; // null-terminator

    if (fread((void *)fileBuffer, 1, fileSizeBytes, fd) != fileSizeBytes)
    {
        fprintf(stderr, "ERROR: Failed to open file: %s\n.", pathname);
        free(fileBuffer);
        fclose(fd);
        exit(EXIT_FAILURE);
    }

    fclose(fd);
}

char *objParser::processVLine(char *lineStart)
{
    float pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    assert(*lineStart == 'v');
    lineStart++;
    char *tokenStart = lineStart;
    char *tokenEnd = lineStart;

    int i = 0;
    while ((*tokenStart != '\n') && (*tokenStart != '\0'))
    {
        if (*tokenStart == ' ') // skip spaces
        {
            tokenStart++;
            tokenEnd++;
            continue;
        }
        while ((*tokenEnd != ' ') && (*tokenEnd != '\n') && (*tokenEnd != '\0'))
            tokenEnd++;

        pos[i] = strtof(tokenStart, &tokenEnd);
        i++;
        tokenStart = tokenEnd;
    }
    Vertex v = {pos[0], pos[1], pos[2], pos[3]};
    verticies.push_back(v);
    nVerts++;
    return tokenEnd;
}
char *objParser::processVnLine(char *lineStart)
{
    float norm[3];
    Normal n;
    assert(*lineStart == 'v' && (*(++(lineStart))) == 'n');
    lineStart++;
    char *tokenStart = lineStart;
    char *tokenEnd = lineStart;

    int i = 0;
    while ((*tokenStart != '\n') && (*tokenStart != '\0'))
    {
        if (*tokenStart == ' ') // skip spaces
        {
            tokenStart++;
            tokenEnd++;
            continue;
        }
        while ((*tokenEnd != ' ') && (*tokenEnd != '\n') && (*tokenEnd != '\0'))
            tokenEnd++;

        norm[i] = strtof(tokenStart, &tokenEnd);
        i++;
        tokenStart = tokenEnd;
    }
    n = {norm[0], norm[1], norm[2]};
    normals.push_back(n);
    nNorms++;
    return tokenEnd;
}

char *objParser::processFLine(char *lineStart)
{
    unsigned int ids[3];
    FaceIndicies fId;
    assert(*lineStart == 'f');
    lineStart++;
    char *tokenStart = lineStart;
    char *tokenEnd = lineStart;

    int i = 0;
    while ((*tokenStart != '\n') && (*tokenStart != '\0'))
    {
        if (*tokenStart == ' ') // skip spaces
        {
            tokenStart++;
            tokenEnd++;
            continue;
        }
        // char *peek = tokenStart;
        // if (*tokenStart == '/')
        // {
        //     int afterVal = isdigit(*(++peek));
        //     peek = tokenStart;
        //     int beforeVal = isdigit(*(--peek));
        //     if (!(afterVal & beforeVal)) // bitwise And then logical not
        //     {
        //         ids[i] = 0;
        //         i++;
        //         tokenEnd = ++(++tokenStart);
        //         continue;
        //     }
        // }

        while ((*tokenEnd != ' ') && (*tokenEnd != '\n') && (*tokenEnd != '\0'))
            tokenEnd++;

        ids[i] = strtoul(tokenStart, &tokenEnd, 0);
        i++;
        tokenStart = tokenEnd;
    }
    fId = {ids[0], ids[1], ids[2]};
    FaceIdx.push_back(fId);
    nFaces++;
    return tokenEnd;
}

objParser::objParser(const char *filePath)
{
    ReadFile(filePath);
    char *current = fileBuffer;
    char *peek = current;
    while (*current != '\0')
    {
        if (*current == '\n' || *current == ' ')
        {
            current++;
            peek++;
            continue;
        }
        if (*current == 'v' && (*(++peek)) == 'n')
            current = processVnLine(current);

        else if (*current == 'v')
            current = processVLine(current);
        else if (*current == 'f')
            current = processFLine(current);
        peek = current;
    }
}

objParser::~objParser()
{
    free(fileBuffer);
}

#endif