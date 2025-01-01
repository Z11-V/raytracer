#pragma once

class texture
{
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture
{
public:
    solid_color(const color& albedo) : albedo(albedo) {}
    solid_color(double red, double green, double blue) : solid_color(color(red,green,blue)) {}

    color value(double u, double v, const point3& p) const override
    {
        return albedo;
    }
private:
    color albedo;
};

class checker_texture : public texture
{
private:
    double inv_scale;
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;

public:
    checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
        : inv_scale(1.0/scale), even(even), odd(odd) {}

    checker_texture(double scale, const color& even, const color& odd)
        : checker_texture(scale,std::make_shared<solid_color>(even),std::make_shared<solid_color>(odd)) {}

    color value(double u, double v, const point3& p) const override
    {
        auto xInt = int(std::floor(inv_scale * p.x()));
        auto yInt = int(std::floor(inv_scale * p.y()));
        auto zInt = int(std::floor(inv_scale * p.z()));

        bool is_even = (xInt + yInt + zInt) % 2 == 0;

        return is_even ? even->value(u,v,p) : odd->value(u,v,p);
    }
};