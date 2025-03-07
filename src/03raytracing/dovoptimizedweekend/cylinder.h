#pragma once
#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "material.h"
#include "interval.h"
#include "point3.h"
#include "unit_vector.h"

class cylinder : public hittable {
  public:
    cylinder(const point3& base, const vec3& axis, double radius, double height, shared_ptr<material> mat)
      : base(base), axis(unit_vector(axis)), radius(std::fmax(0,radius)), 
        height(std::fmax(0,height)), mat(mat) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Transform ray to cylinder's local space where axis is along y
        vec3 oc = r.origin() - base;
        vec3 dir = r.direction();
        
        // Project ray onto plane perpendicular to axis
        double a = dir.length_squared() - pow(dot(dir, axis), 2);
        double b = 2 * (dot(oc, dir) - dot(dir, axis) * dot(oc, axis));
        double c = oc.length_squared() - pow(dot(oc, axis), 2) - radius * radius;

        double discriminant = b*b - 4*a*c;
        if (discriminant < 0) return false;

        double sqrtd = sqrt(discriminant);
        double t = (-b - sqrtd) / (2*a);
        
        // Check if hit point is within height bounds
        if (!ray_t.surrounds(t)) {
            t = (-b + sqrtd) / (2*a);
            if (!ray_t.surrounds(t))
                return false;
        }

        point3 hit_point = r.at(t);
        double hit_height = dot(hit_point - base, axis);
        
        if (hit_height < 0 || hit_height > height)
            return false;

        rec.t = t;
        rec.p = hit_point;
        vec3 outward_normal = (hit_point - (base + axis * hit_height)) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

  private:
    point3 base;     // Center of bottom circle
    vec3 axis;       // Unit vector pointing up
    double radius;
    double height;
    shared_ptr<material> mat;
};