#pragma once
/*
  author: Dov Kruger
  A textured sphere with the same geometry as the sphere class,
  but with an applied texture.
*/
#include "sphere.h"


class texturedsphere : public sphere {
  public:
    texturedsphere(const point3& center, double radius, shared_ptr<material> mat,
         const std::string& texture_file)
      : sphere(center, radius, mat) {

        // Load the texture from the file.
        texture = load_texture(texture_file);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        if (!sphere::hit(r, ray_t, rec)) {
            return false;
        }

        //compute the point at which the ray hits the sphere
        point3 p = r.at(rec.t);

        //compute the spherical coordinates of the point
        double theta = std::acos(p.y() / radius);
        double phi = std::atan2(p.z(), p.x());

        //compute the texture coordinates
        double u = phi / (2 * M_PI);
        double v = theta / M_PI;
        // convert to an pixel position within the texture and get the color
        color c = texture.get_color(u, v);
        // the material properties should be modified by the texture color
        // we can do albedo later
        rec.mat = mat;
        rec.p = p;
        rec.normal = (p - center) / radius;
        rec.set_face_normal(r, rec.normal);
        return true;
    }

  private:
    texture texture;
};
