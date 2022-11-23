CC = g++

LIBS = `pkg-config --libs sdl2`
INCLUDE = -I./include 
# CFLAGS = -Wall -pedantic `pkg-config --cflags sdl2` -ggdb -g -O0
CFLAGS = -Wall -pedantic `pkg-config --cflags sdl2` -O3 -msse2

SRC = ./src/main.cpp \
	./src/rasterizer.cpp \
	./src/objParser.cpp \
	./src/Matrix4.cpp \
	./src/Vector3.cpp \
	./src/Vector4.cpp \
	./src/RGBColor.cpp 
rasterizer:
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o ./bin/$@ $(LIBS)

modelTest:
	g++ -w -g $(INCLUDE) ./src/modelTest.cpp -o ./bin/$@ 