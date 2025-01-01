#pragma once

#include <array>

class vec3
{
private:
    /* data */
public:
    std::array<double,3> e;

    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double s)
    {
        e[0] *= s;
        e[1] *= s;
        e[2] *= s;
        return *this;
    }

    vec3& operator/=(double s)
    {
        return *this *= 1/s;
    }

    double length_squared() const
    {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    double length() const
    {
        return std::sqrt(length_squared());
    }

    bool near_zero() const
    {
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

};

using point3 = vec3; //alias

std::ostream& operator<<(std::ostream& out, const vec3& v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

vec3 operator+(const vec3& u, const vec3& v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

vec3 operator-(const vec3& u, const vec3& v)
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

vec3 operator*(const vec3& u, const vec3& v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

vec3 operator*(const vec3& v, double s)
{
    return vec3(v.e[0]*s, v.e[1]*s, v.e[2]*s);
}

vec3 operator*(double s, const vec3& v)
{
    return v * s;
}

vec3 operator/(const vec3& v, double s)
{
    return v * (1/s);
}

double dot(const vec3& u, const vec3& v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

vec3 cross(const vec3&u, const vec3& v)
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1] ,
                u.e[2] * v.e[0] - u.e[0] * v.e[2] ,
                u.e[0] * v.e[1] - u.e[1] * v.e[0] );
}

vec3 unit_vector(const vec3& v)
{
    return v / v.length();
}

vec3 lerp(vec3 startValue, vec3 endValue, double a)
{
    return (1.0-a) * startValue + a * endValue;
}

vec3 random_unit_vector()
{
    while (true)
    {
        auto p = vec3::random(-1,1);
        auto lensq = p.length_squared();
        // lensq should not be very close to zero
        if (1e-160 < lensq && lensq <= 1)
        {
            return p / std::sqrt(lensq);
        }
    }
}

vec3 random_vec_on_hemisphere(const vec3& normal)
{
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(normal, on_unit_sphere) < 0)
    {
        on_unit_sphere = -on_unit_sphere;
    }
    return on_unit_sphere;
}

vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2*dot(v,n)*n;
}

vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
    auto cos_theta = std::fmin(dot(-uv,n),1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}