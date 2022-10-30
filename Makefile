CC = g++

LIBS = `pkg-config --libs sdl2`
INCLUDE = -I./include 
CFLAGS = -Wall -pedantic `pkg-config --cflags sdl2`

SRC = ./src/rasterizer.cpp \
	./src/Matrix4.cpp \
	./src/Vector3.cpp \
	./src/Vector4.cpp 
rasterizer:
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o ./bin/$@ $(LIBS)