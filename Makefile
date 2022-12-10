CC = g++

# LIBS = `pkg-config --libs sdl2` `pkg-config --libs assimp`
LIBS = `pkg-config --libs sdl2` 
INCLUDE = -I./include 
# CFLAGS = -Wall -pedantic `pkg-config --cflags sdl2` -ggdb -g -O0
CFLAGS = -Wall -pedantic `pkg-config --cflags sdl2` -O3 -msse2  -Wno-maybe-uninitialized
# CFLAGS = -Wall -pedantic `pkg-config --cflags sdl2` `pkg-config --cflags assimp` -ggdb -g -O0
# CFLAGS = -Wall -pedantic `pkg-config --cflags sdl2` `pkg-config --cflags assimp` -O3 -msse2 

SRC = ./src/main.cpp \
	./src/rasterizer.cpp \
	./src/objParser.cpp \
	./src/Matrix4.cpp \
	./src/Vector3.cpp \
	./src/Vector4.cpp \
	./src/RGBColor.cpp
OBJS = $(patsubst ./src/%,./bin/%,$(patsubst %.cpp,%.o,$(SRC)))

all:rasterizer

rasterizer:$(OBJS)
	@echo "Building $@ "
	@$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o ./bin/$@ $(LIBS)


./bin/%.o:./src/%.cpp
	@echo "Compileing $^ "
	@$(CC) $(CFLAGS) $(INCLUDE) -c $^ -o $@ $(LIBS)

modelTest:
	g++ -w -g $(INCLUDE) ./src/modelTest.cpp -o ./bin/$@ 