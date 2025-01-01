#pragma once

#include "hittable.hpp"

class quad : public hittable
{

private:
    point3 Q;
    vec3 u, v;
    vec3 w;
    std::shared_ptr<material> mat;
    vec3 normal;
    double D;

public:
    quad(const point3& Q, const vec3& u, const vec3& v, std::shared_ptr<material> mat) : Q(Q), u(u), v(v), mat(mat)
    {
        auto n = cross(u,v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override
    {
        auto denom = dot(normal, r.direction());

        // No hit if ray parallel to plane
        if (std::fabs(denom) < 1e-8)
        {
            return false;
        }
        
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
        {
            return false;
        }

        auto intersection = r.at(t);
        vec3 planar_hitpoint_vec = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpoint_vec, v));
        auto beta = dot(w, cross(u, planar_hitpoint_vec));

        if (!is_interior(alpha, beta, rec)) // rec to set uv
        {
            return false;
        }
        

        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal); 
        
        return true;
    }

    virtual bool is_interior(double a, double b, hit_record& rec) const
    {
        if (!interval::zero_to_one.contains(a) || !interval::zero_to_one.contains(b))
        {
            return false;
        }
        rec.u = a;
        rec.v = b;
        return true;
    }
};