#include "Shape.hpp"
#include <cmath>
using namespace std;
vector<point3d> Shape::sphere(uint32_t latRes, uint32_t longRes, float radius) {
   return vector<point3d>();
}
vector<point3d> Shape::cube(float size) {
    return vector<point3d>();
}

vector<point3d>  Shape::cylinder(uint32_t res, float radius, float height) {

	// create a cylinder with the given resolution (number of points around the circle)
	// return an array of vertices, each vertex is x,y,r,g,b
	// THERE IS NO TOP OR BOTTOM, just the sides
	vector<point3d> vertices;
	for (int i = 0; i <= res; i++) {
		float angle =  (2 * M_PI/res) * i;
		float x = cos(angle);
		float z = sin(angle);
//		cout << "vertex " << i << " is at " << x << ",0," << z << endl;
		vertices.push_back({x, 0, z, 1, 1, 1}); // white color
		vertices.push_back({x, 1, z, 1, 0, 0}); // red color
	}
	return vertices;
}

vector<point3d> Shape::cone(uint32_t res, float radius, float height) {
    return vector<point3d>();
}
vector<point3d> Shape::torus(uint32_t latRes, uint32_t longRes, float innerRadius, float outerRadius) {
    vector<point3d> vertices;
    float R = outerRadius;
    float rin = innerRadius;
    float rcenter = (R + rin) / 2;
    float rcross_section = (R - rin) / 2;
    for (int i = 0; i < latRes; i++) {
        float r = 0, g = 1, b = 0;
        float latAngle = (2 * M_PI / latRes) * i;
        float latAngle2 = (2 * M_PI / latRes) * (i+1);
        const float dcolor = 1.0 / (longRes+1);
        for (int j = 0; j <= longRes; j++) {
            float longAngle = (2 * M_PI / longRes) * j;
            b = 0;
            float x = (R + rin * cos(latAngle)) * cos(longAngle);
            float y = rin * sin(latAngle);
            float z = (R + rin * cos(latAngle)) * sin(longAngle);
            vertices.push_back({x, y, z, r, g, b});
            float x2 = (R + rin * cos(latAngle2)) * cos(longAngle);
            float y2 = rin * sin(latAngle2);
            float z2 = (R + rin * cos(latAngle2)) * sin(longAngle);
            vertices.push_back({x2, y2, z2, r, g, b});
            r += dcolor;
            g -= dcolor;
        }
    }
    return vertices;
}


/**
 * The problem with the shapes above is that we replicate points. THey are up to 6 times slower than they need to be
 * Every transformation must be applied to every point. Copying a point is a lot of work
 * We can do better using indices
 * 
 * 
 * p00  p01   p02   p03  ... p0cols
 *    \     \     \
 * p10  p11   p12   p13  ... p1cols
 *    \     \     \
 * p20  p21   p22   p23  ... p2cols
 * ...
 * prows0  prows1   ...      prows,cols     
 * 

    Heatmap creates a 2d grid, where each point is x,y,0, r,g,b
    24 bytes per vertex.

    heatmap function should allocate a grid of rows*cols boxes
 */


 void Shape::heatmap(uint32_t rows, uint32_t cols, std::vector<point3d>& vert, vector<uint32_t>& indices,
     const vector<float>& values, float minVal, float maxVal) {
    vert.reserve((rows+1)*(cols+1)); // 1 more vertex to surround rows*cols boxes
    int c = 0; // the linear position of the current vertex, used for the value
    for (int i = 0; i <= rows; i++) {
        for (int j = 0; j <= cols; j++, c++) {
            float f = (values[c] - minVal) / (maxVal - minVal); // fraction 0 to 1
            float r = f, b = 1-f, g = 0;
            vert.push_back({j/4.0f, i/4.0f, 0, r, g, b});
        }
    }

    // now for each triangle, list 3 indices
    // first triangle is 0, 1, 2, next is 3 = (1,2,3)
    // then 4 (2,3,4)
    int ind = 0;
    for (int i = 0; i < rows-1; i++) {
        for (int j = 0; j < cols; j++, ind++) {
            indices.push_back(ind); // 0
            indices.push_back(ind+(cols+1)); // next row is j + cols + 1
        }
        int next_row = ind + cols + 1 - cols; // first point of next row
        // build a degenerate triangle to start at the next row
        indices.push_back(ind); // 0
        indices.push_back(ind); // next row is j + cols + 1
    }
    // vert and indices will be set upon return
}
