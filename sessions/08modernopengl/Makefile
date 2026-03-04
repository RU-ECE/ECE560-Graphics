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

all: bin/04transform  bin/05cylinder bin/06heatmap

#this builds the single common object file used by all demos
#which contains a standard main, loader for shaders,
# and function to open windows
common.o:  common/common.cc common/common.hh
	$(COMP) -c $<

bin/04transform: 04transform.cc common.o
	$(COMP) $< -o $@ $(LIBS)

bin/05cylinder: 05cylinder.cc Shape.o common.o
	$(COMP) 05cylinder.cc Shape.o -o $@ $(LIBS)

bin/06heatmap: 06heatmap.cc Shape.o common.o
	$(COMP) 06heatmap.cc Shape.o -o $@ $(LIBS)

Shape.o: Shape.cpp
	$(COMP) -c $<

clean:
	rm bin/*

