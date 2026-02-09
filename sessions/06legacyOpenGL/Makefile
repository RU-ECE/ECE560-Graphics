DEBUG := -g
OPT   := #-O2
CXX   :=  g++ $(DEBUG) $(OPT)
LIBS  := -lGL -lGLU -lglut  # this is for linux
#LIBS := -lopengl32 -lglu32 -lfreeglut   # this is for windows (with freeglut)

DEMOS := 00nodraw 01emptywindow 02hellosphere 03hellotriangle 04gouraudtriangles
all: $(DEMOS)

00nodraw: 00nodraw.cpp
	$(CXX) $< -o $@ $(LIBS)

01emptywindow: 01emptywindow.cpp
	$(CXX) $< -o $@ $(LIBS)

02hellosphere: 02hellosphere.cpp
	$(CXX) $< -o $@ $(LIBS)

03hellotriangle: 03hellotriangle.cpp
	$(CXX) $< -o $@ $(LIBS)

04gouraudtriangles: 04gouraudtriangles.cpp
	$(CXX) $< -o $@ $(LIBS)

clean:
	rm *.o *.exe $(DEMOS)