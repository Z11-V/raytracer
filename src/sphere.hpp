#pragma once

#include "hittable.hpp"

class sphere : public hittable
{
private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat;
public:
    sphere(const point3& center, double radius, std::shared_ptr<material> mat) 
        : center(center), radius(std::fmax(0, radius)), mat(mat) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override
    {
        auto oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - (radius*radius);

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
        {
            return false;
        }

        auto sqrtd = std::sqrt(discriminant);

        auto root_t = (h - sqrtd) / a;
        if (!ray_t.surrounds(root_t))
        {
            root_t = (h + sqrtd) / a;
            if (!ray_t.surrounds(root_t))
            {
                return false;
            } 
        }
        
        rec.t = root_t;
        rec.p = r.at(rec.t);
        auto outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }
};