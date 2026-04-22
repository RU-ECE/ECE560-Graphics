CXX      := g++
CXXFLAGS := -std=c++17 -Wall -g
LDFLAGS  := -lvulkan -lglfw

SHADERS  := triangle_vert_spv.h simple_frag_spv.h triangle_transform_vert_spv.h sphere_vert_spv.h
BIN      := ../bin
all: $(BIN)/00_introvulkan $(BIN)/01_vulkan $(BIN)/02_hellotriangle $(BIN)/03_rotating_triangle $(BIN)/04_sphere
COMMON  := vulkan_common.o

$(BIN)/00_introvulkan: 00_introvulkan.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

$(BIN)/01_vulkan: 01_vulkan.cpp $(COMMON) vulkan_common.hpp
	$(CXX) $(CXXFLAGS) -o $@ 01_vulkan.cpp $(COMMON) $(LDFLAGS)

$(BIN)/02_hellotriangle: 02_hellotriangle.cpp $(COMMON) vulkan_common.hpp $(SHADERS)
	$(CXX) $(CXXFLAGS) -o $@ 02_hellotriangle.cpp $(COMMON) $(LDFLAGS)

$(BIN)/03_rotating_triangle: 03_rotating_triangle.cpp $(COMMON)  $(SHADERS)
	$(CXX) $(CXXFLAGS) -o $@ 03_rotating_triangle.cpp $(COMMON) $(LDFLAGS)

$(BIN)/04_sphere: 04_sphere.cpp $(COMMON) vulkan_common.hpp sphere_with_indices.hpp $(SHADERS)
	$(CXX) $(CXXFLAGS) -o $@ 04_sphere.cpp $(COMMON) $(LDFLAGS)

vulkan_common.o: vulkan_common.cpp vulkan_common.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

triangle_vert.spv: triangle_vert.glsl
	glslangValidator -V -S vert -o $@ $<

triangle_transform_vert.spv: triangle_transform_vert.glsl
	glslangValidator -V -S vert -o $@ $<

sphere_vert.spv: sphere_vert.glsl
	glslangValidator -V -S vert -o $@ $<

simple_frag.spv: simple_frag.glsl
	glslangValidator -V -S frag -o $@ $<

%_spv.h: %.spv
	xxd -i $< > $@

clean:
	rm -f $(BIN)/00_introvulkan $(BIN)/01_vulkan $(BIN)/02_hellotriangle $(BIN)/03_rotating_triangle $(BIN)/04_sphere *.spv *_spv.h
