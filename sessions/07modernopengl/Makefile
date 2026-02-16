common = ../../src/02modernopengl
CXX = g++ -g
CXXFLAGS = -std=c++17 -Wall -O2
LDFLAGS = -lGL -lGLU -lglfw -lGLEW -lwebp
# for windows use LDFLAGS = -lopengl32 -lglu32 -lglfw3 -lglew32

00rectangle_with_border: 00rectangle_with_border.cpp
	$(CXX) $(CXXFLAGS) -I$(common) $(common)/common/common.cc -o $@ $^ $(LDFLAGS)