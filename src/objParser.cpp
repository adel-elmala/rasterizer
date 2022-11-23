#include "objParser.h"
#include <iostream>

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
    Vector3 n;
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
    formTriangles();
    fprintf(stdout, "n Verticies: %u\n", nVerts);
    fprintf(stdout, "n Normals: %u\n", nNorms);
    fprintf(stdout, "n Triangles: %u\n", nFaces);

    std::cout << "=========== " << std::endl;
    std::cout << "n Verticies: " << verticies.size() << std::endl;
    std::cout << "n Normals: " << normals.size() << std::endl;
    std::cout << "n Triangles: " << triangles.size() << std::endl;
}

objParser::~objParser()
{
    free(fileBuffer);
}

void objParser::formTriangles(void)
{
    Vector3 tv1;
    Vector3 tv2;
    for (auto face : FaceIdx)
    {
        triangle t;
        t.v1.pos = Vector3(verticies[(face.v1) - 1].x,
                           verticies[(face.v1) - 1].y,
                           verticies[(face.v1) - 1].z);
        t.v1.col = RGBColor(1.0f, 0.0f, 0.0f);

        t.v2.pos = Vector3(verticies[(face.v2) - 1].x,
                           verticies[(face.v2) - 1].y,
                           verticies[(face.v2) - 1].z);
        t.v2.col = RGBColor(1.0f, 0.0f, 0.0f);

        t.v3.pos = Vector3(verticies[(face.v3) - 1].x,
                           verticies[(face.v3) - 1].y,
                           verticies[(face.v3) - 1].z);
        t.v3.col = RGBColor(1.0f, 0.0f, 0.0f);
        if (normals.size() == verticies.size())
        {
            t.nNorms = 3 ;
            t.tNorm = normals[(face.v1) - 1];
            t.tNorm = normals[(face.v2) - 1];
            t.tNorm = normals[(face.v3) - 1];
        }
        else 
        {
            t.nNorms = 1 ;
            tv1 = t.v2.pos - t.v1.pos;
            tv2 = t.v3.pos - t.v1.pos;
            t.tNorm = (tv2 ^ tv1).hat();
        }
        // t.tNorm.prettyPrint();
        triangles.push_back(t);
    }
}
