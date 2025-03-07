/**
 * Optimized ray tracing code based on Peter Shirley's book "Ray Tracing in One Weekend"
 * 
 *  Author: Dov Kruger
 *  Date: 2025-03-07
*/

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include <fstream>
#include <chrono>
using namespace std;

/*
    TODO: replace the rand() generator with a better one. rand() has many problems, including
    not being thread safe, and being correlated with pairs of values.
*/
std::mt19937 gen(0); // seed with a known value for reproducibility
std::uniform_int_distribution<> int_dist(0, 999);
std::uniform_real_distribution<> delta_pos(-.9, .9);

void generate_world(hittable_list &world, int min_coord, int max_coord) {
    // Reset the generator before world creation for reproducibility
    gen.seed(0);
    std::uniform_real_distribution<double> random_unit(0.0, 1.0);

    auto ground_material = make_shared<lambertian>(color(0.3, 0.3, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));
    for (int a = min_coord; a < max_coord; a++) {
        for (int b = min_coord; b < max_coord; b++) {
            auto choose_mat = random_unit(gen);
            point3 center(a + 0.9*random_unit(gen), 0.2, b + 0.9*random_unit(gen));

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                double radius = 0.2; // + 0.1*random_double();
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, radius, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, radius, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, radius, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
}

int main(int argc, char **argv) {
    const char* filename = argc > 1 ? argv[1] : "test.webp";
    const int min_coord = argc > 2 ? atoi(argv[2]) : -13;
    const int max_coord = argc > 3 ? atoi(argv[3]) : 13;
    const int samples_per_pixel = argc > 4 ? atoi(argv[4]) : 10;
    const int max_depth = argc > 5 ? atoi(argv[5]) : 20;

    cout << "filename: " << filename << '\n'
         << "min_coord: " << min_coord << '\n'
         << "max_coord: " << max_coord << '\n'
         << "samples_per_pixel: " << samples_per_pixel << '\n'
         << "max_depth: " << max_depth << '\n';

    hittable_list world;

    generate_world(world, min_coord, max_coord);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = samples_per_pixel;
    cam.max_depth         = max_depth;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    auto start = chrono::high_resolution_clock::now();
    cam.render(world, filename);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "render time: " << duration.count()*.001 << "s\n";
}
