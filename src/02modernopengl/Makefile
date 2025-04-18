DEBUG	:=	-g -std=c++20
OPT	:=	#-O2
COMP	:= g++ $(OPT) $(DEBUG)

ifeq ($(OS),Windows_NT)
	LIBS	:= common.o -I/mingw64/include/GL -L/mingw64/lib -lOpenGL32 -lglfw3 -lglew32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LIBS	:= common.o -framework OpenGL -lglfw -lGLEW
		MWELIBS	:= -framework OpenGL -lglfw -lGLEW
	else
		# Linux
		LIBS	:= common.o -lglfw $(shell pkg-config --libs glew) -lwebp
		MWELIBS	:= -lglfw $(shell pkg-config --libs glew) -lwebp
	endif
endif

#TODO: bin/02b_rect
#TODO: bin/06c_litsphere 
#TODO: bin/lighting

all: bin/01window bin/02triangle bin/03gouraud \
	bin/04transform bin/04c_projection bin/05sphere1 \
	bin/06sphere2 bin/06b_sphere3 bin/07visualizefunc \
	bin/mwe bin/mwe2 bin/texture \
	bin/convert_obj_blockloader bin/testblockload

#this builds the single common object file used by all demos
#which contains a standard main, loader for shaders,
# and function to open windows
common.o:  common/common.cc common/common.hh
	$(COMP) -c $<

bin/01window: 01window.cc common.o
	$(COMP) $< -o $@ $(LIBS)

bin/02triangle: 02triangle.cc common.o
	$(COMP) $< -o $@ $(LIBS)

bin/02b_rect: 02b_rect.cc common.o
	$(COMP) $< -o $@ $(LIBS)

bin/03gouraud: 03gouraud.cc common.o
	$(COMP) $< -o $@ $(LIBS)

bin/04transform: 04transform.cc common.o
	$(COMP) $< -o $@ $(LIBS)

bin/04b_input2: 04b_input2.cc common.o
	$(COMP) $< -o $@ $(LIBS)

bin/04c_projection: 04c_projection.cc common.o
	$(COMP) $< -o $@ $(LIBS)

bin/05sphere1: 05sphere1.cc Shape.hh Shape.cc common.o
	$(COMP) $< Shape.cc -o $@ $(LIBS)

bin/06sphere2: 06sphere2.cc Shape.hh Shape.cc common.o
	$(COMP) $< Shape.cc -o $@ $(LIBS)

bin/06b_sphere3: 06b_sphere3.cc Shape.hh Shape.cc common.o
	$(COMP) $< Shape.cc -o $@ $(LIBS)

bin/06c_litsphere: 06c_litsphere.cpp technique.cpp lighting_technique.cpp lighting_technique.h Shape.hh Shape.cc common.o
	$(COMP) $< Shape.cc technique.cpp lighting_technique.cpp -o $@ $(LIBS)

bin/07visualizefunc:  07visualizefunc.cc  common.o
	$(COMP) $< -o $@ $(LIBS)

#bin/08lights: 08lights.cc common.o
#	$(COMP) $< -o $@ $(LIBS)

bin/input2: input2.cpp
	$(COMP) $< -o $@ $(LIBS) -lglut

bin/mwe: mwe.cpp
	$(COMP) $< -o $@ $(MWELIBS)

bin/mwe2: mwe2.cpp
	$(COMP) $< -o $@ $(MWELIBS)

bin/texture: texture.cpp
	$(COMP) $< -o $@ $(MWELIBS) -lwebp

bin/lighting: lighting.cpp
	$(COMP) $< -o $@ $(MWELIBS) -lassimp

bin/convert_obj_blockloader: convert_obj_blockloader.cpp mesh.hh
	$(COMP) $< -o $@ $(MWELIBS) -lassimp

bin/testblockload: testblockload.cpp mesh.hh
	$(COMP) $< -o $@ $(MWELIBS) -lassimp

clean:
	rm bin/*

