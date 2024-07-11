#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "interval.h"

class sphere : public hittable {
  public:
    sphere(
    const point3& center,
    double radius, 
    shared_ptr<Material> mat_ptr) : 
    center(center), 
    radius(fmax(0,radius)),
    mat_ptr(mat_ptr) { }

    bool hit(const ray& r, Interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.set_face_normal(r, (rec.p - center) / radius);
        rec.mat_ptr = mat_ptr;

        return true;
    }

  private:
    point3 center;
    double radius;
    shared_ptr<Material> mat_ptr;
};

#endif