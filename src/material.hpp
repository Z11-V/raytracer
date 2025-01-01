#pragma once

#include "hittable.hpp"
#include "texture.hpp"

class material{
public:
    virtual color emitted(double u, double v, const point3& p) const
    {
        return color(0,0,0);
    }
    virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const
    {
        return false;
    }
};

class lambertian : public material
{
public:
    lambertian(const color& albedo) : albedo(std::make_shared<solid_color>(albedo)) {}
    lambertian(std::shared_ptr<texture> albedo) : albedo(albedo) {}

    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
        {
            scatter_direction = rec.normal;
        }
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
    
private:
    std::shared_ptr<texture> albedo;    
};

class metal : public material
{
public:
    metal(const color& albedo, double fuzz = 0) : albedo(std::make_shared<solid_color>(albedo)), fuzz(fuzz < 1 ? fuzz : 1) {}
    metal(std::shared_ptr<texture> albedo, double fuzz = 0) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        vec3 reflected = reflect(ray_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected);
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    std::shared_ptr<texture> albedo;
    double fuzz;
};

class dielectric : public material
{
private:
    double refraction_index;

    static double reflectance(double cosine, double refraction_index)
    {
        // Schlick's approximation
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1-r0)*std::pow((1-cosine),5);
    }

public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        attenuation = color(1.0,1.0,1.0);
        double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(ray_in.direction());
        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);
        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;
        if (cannot_refract || reflectance(cos_theta,ri) > random_double())
        {
            direction = reflect(unit_direction, rec.normal);
        }
        else
        {
            direction = refract(unit_direction, rec.normal, ri);
        }

        scattered = ray(rec.p, direction);
        return true;
    }
};

class diffuse_light : public material
{
public:
    diffuse_light(const color& emission_color, double emission_strength = 1.0) : emission_color(std::make_shared<solid_color>(emission_color)), emission_strength(emission_strength) {}
    diffuse_light(std::shared_ptr<texture> emission_color, double emission_strength = 1.0) : emission_color(emission_color), emission_strength(emission_strength) {}

    color emitted(double u, double v, const point3& p) const override
    {
        return emission_strength * emission_color->value(u, v, p);
    }

private:
    std::shared_ptr<texture> emission_color;
    double emission_strength;
};

class one_sided_material : public material
{
public:
    one_sided_material(std::shared_ptr<material> mat) : mat(mat)  {}

    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        if (!rec.front_face)
        {
            attenuation = color(1.0,1.0,1.0);
            scattered = ray(rec.p, ray_in.direction());
            return true;
        }
        else
        {
            return mat->scatter(ray_in, rec, attenuation, scattered);
        }  
    }

private:
    std::shared_ptr<material> mat;
};

class isotropic : public material
{
private:
    std::shared_ptr<texture> tex;

public:
    isotropic(const color& albedo) : tex(std::make_shared<solid_color>(albedo)) {}
    isotropic(std::shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        scattered = ray(rec.p, random_unit_vector());
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }

};