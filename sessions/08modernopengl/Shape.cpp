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