CC = g++

LIBS = `pkg-config --libs sdl2`
INCLUDE = -I./include 
CFLAGS = -Wall -pedantic `pkg-config --cflags sdl2`

SRC = ./src/rasterizer.cpp 
rasterizer:
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o ./bin/$@ $(LIBS)