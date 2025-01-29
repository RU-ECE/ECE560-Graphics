#include "shape.hh"
#include "log.hh"
#include <numbers>
#include <GL/glew.h>
using namespace std::numbers;
constexpr double PI = numbers::pi;https://www.amazon.com/gp/product/B005GQYXTC?ie=UTF8&psc=1

// utility function to dump vertex data to the screen
void dump_vert(float* vert, uint32_t resolution, uint32_t c) {
    if (log::level() > log::INFO) return;
    for (uint32_t i = 0; i < c; i+= 5) {
        log::info("(%f, %f, %f)", vert[i], vert[i+1], vert[i+2]);
    }
    log::info("resolution: %d", resolution);
    log::info("predicted num vert components: %d", resolution*5);  
    log::info("actual num vert components: %d", c);
}   

/*
  utility function to dump index data to the screen, 2 indices per
  for triangle_strip
*/ 
void dump_index(uint32_t* indices, uint32_t resolution, uint32_t c) {
    if (log::level() > log::INFO) return;
    for (uint32_t i = 0; i < c; i+=2) {
        log::info("(%d, %d)", indices[i], indices[i+1]);
    }
}

shape::shape(const float vert[], const uint32_t vert_size,
            const uint32_t indices[], const uint32_t index_size) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_size*sizeof(float), vert, GL_STATIC_DRAW);
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size*sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}


/*
    generate a unit sphere (r=1) with lon_res points around the equator and
    lat_res points above and below the equations (2*lat_res+1)
*/
shape shape::create_sphere(int lat_res, int lon_res) {
    const uint32_t yres = 2*latRes-1;
    const uint32_t xres = lonRes + 2; // includes + 2 for wrapping back to the start
    const uint32_t resolution = yres*xres;
    const double dlon = 2.0*PI / lonRes, dlat = PI / (2*latRes);
    double lat = -PI/2 + dlat; // latitude in radians
    float* vert = new float[resolution*5];
    uint32_t c = 0;
    for (uint32_t j = 0; j < yres; j++, lat += dlat) {
        //what is the radius of hte circle at that height?
        double rcircle = r* cos(lat); // size of the circle at this latitude
        double z = r * sin(lat); // height of each circle
    
//        log::info("rcircle=%f, z=%f", rcircle, z);
        double t = 0;
        for (uint32_t i = 0; i < lonRes; i++, t += dlon) {
            vert[c++] = rcircle * cos(t),
            vert[c++] = rcircle * sin(t);
            vert[c++] = z;
            vert[c++] = t / (2.0 * PI); // Correct u mapping
            vert[c++] = (lat + PI / 2.0) / PI; // Correct v mapping
        }
    }
    // south pole
    vert[c++] = 0;
    vert[c++] = 0;
    vert[c++] = 1;
    vert[c++] = 0.5;
    vert[c++] = 0;

    // north pole
    vert[c++] = 0;
    vert[c++] = 0;
    vert[c++] = +1;
    vert[c++] = 0.5;
    vert[c++] = 1;

    dump_vert(vert, resolution*5, c);

    const uint32_t indexSize = resolution * 2;// + (2*latRes-1) + lonRes; 
    //TODO: North and South Poles aren't used
    uint32_t* indices = new uint32_t[indexSize];
    c = 0;
    for (uint32_t j = 0; j < yres-1; j++) {
        uint32_t startrow = j*lonRes;
        for (uint32_t i = 0; i < lonRes; i++) {
            indices[c++] = startrow + i;
            indices[c++] = startrow + lonRes + i;
        }
        indices[c++] = startrow;
        indices[c++] = startrow + lonRes;
        // Add degenerate triangles to connect strips
        indices[c++] = (j + 1) * lonRes + (lonRes - 1);
        indices[c++] = (j + 1) * lonRes;
    }
    dump_index(indices, indexSize, c);

    shape s(vert, resolution*5, indices, indexSize);
    delete[] vert;
    delete[] indices;
    return s;
}

shape shape::create_cube() {
    // corners of the cube
    float cube_verts[] = {
        -1.0f, -1.0f, -1.0f,//0
        -1.0f, +1.0f, -1.0f,//1
        +1.0f, -1.0f, -1.0f,//2
        +1.0f, +1.0f, -1.0f,//3
        -1.0f, -1.0f, +1.0f,//4
        -1.0f, +1.0f, +1.0f,//5
        +1.0f, -1.0f, +1.0f //6
        +1.0f, +1.0f, +1.0f,//7
    };
    // indices for the cube in a triangle strip
    // traversing clockwise from the back face
    uint32_t cube_indices[] = {
        0, 1, 2, 3, // back face
        6, 7,       // right face
        4, 5,       // front face
        0, 1,       // left face
        // now the top face
        1, //degenerate triangle
        3, 5, 7, // top face?
        7, //degenerate triangle
        6, 0, //degenerate triangle moving down to bottom?
        2, 4 // bottom face
    };
    return shape(cube_verts, 3*8, cube_indices, 3*12);
}

/*
    create a cylinder with the number of facets around the circumference
*/
shape shape::create_cylinder(int res) {
    float* vert = new float[res*(5*2)]; // 5 floats per vertex, 2 vertices per facet
    const double da = 2.0*PI / res;
    const double TO_UNIT = 1.0 / (2*PI);
    double a = 0;
    for (int i = 0; i < res; i++, a += da) {
        vert[c++] = cos(a);
        vert[c++] = sin(a);
        vert[c++] = 0;
        vert[c++] = a * TO_UNIT;
        vert[c++] = 0;

        vert[c++] = cos(a);
        vert[c++] = sin(a);
        vert[c++] = 1;
        vert[c++] = a * TO_UNIT;
        vert[c++] = 0;
    }
    shape s(vert, res*5);
    delete[] vert;
    return s;
}

shape shape::create_cone(int res) {
    float* vert = new float[res*(5*2)]; // 5 floats per vertex, 2 vertices per facet
    const double da = 2.0*PI / res;
    const double TO_UNIT = 1.0 / (2*PI);
    double a = 0;
    for (int i = 0; i < res; i++, a += da) {
        vert[c++] = cos(a);
        vert[c++] = sin(a);
        vert[c++] = 0;
        vert[c++] = a * TO_UNIT;
        vert[c++] = 0;
    }
    vert[c++] = 0;
    vert[c++] = 0;
    vert[c++] = 1;
    vert[c++] = 0.5;
    vert[c++] = 0;
}

/*
    create a torus with a major radius of 1.0, broken into ring_res sections
    around the torus, with the tube radius of radius broken into
    tube_res sections
*/
shape shape::gen_torus(float radius, uint32_t ring_res, uint32_t tube_res) {
    float* vert = new float[ring_res*lon_res*5];
    const double dring = 2.0 * PI / ring_res;
    const double dlon = 2.0 * PI / lon_res;
    const double TO_UNIT = 1.0 / (2*PI);
    double ring = 0;
    for (int i = 0; i < ring_res; i++, ring += dring) {
        double x = cos(ring), y = sin(ring);
        double t = 0;
        for (int j = 0; j < tube_res; j++, t += dlon) {
            double hor = 1.0 + radius * cos(t);

            vert[c++] = hor * cos(t);
            vert[c++] = hor * sin(t);
            vert[c++] = radius * sin(t);
            vert[c++] = t * TO_UNIT;
            vert[c++] = ring * TO_UNIT;
        }
    }
    dump_vert(vert, ring_res*lon_res*5, c);
    float* ind = new float[ring_res*lon_res*5];
    c = 0;
    for (int i = 0; i < ring_res; i++) {
        double t = 0;
        for (int j = 0; j < tube_res; j++, t += dt) {
            ind[c++] = i*ring_res + j;
            ind[c++] = i*ring_res + (j+1)%tube_res;
            ind[c++] = (i+1)%ring_res*tube_res + j;
            ind[c++] = ring * TO_UNIT;
            ind[c++] = t * TO_UNIT;

            ind[c++] = ;
            ind[c++] = ;
            ind[c++] = ;
            ind[c++] = ring * TO_UNIT;
            ind[c++] = t * TO_UNIT;
        }
    }

    shape s(vert, ring_res*lon_res*5, ind, ring_res*tube_res*2);
    delete [] vert;
    delete [] ind;
    return s;
}

shape shape::gen_grid(uint32_t nx, uint32_t ny) {
    return shape();
}

shape shape::gen_circle(int circle_res) {
  float* vert = new float[circle_res*5];
  int c = 0;
  double t = 0;
  const double dt = 2.0*PI / circle_res;
  for (int i = 0; i < circle_res; i++, t += dt) {
    vert[c++] = cos(t);
    vert[c++] = sin(t);
  }
  shape s(vert, circle_res*2);
  delete [] vert;
  return s;
}

