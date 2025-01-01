#include "common.hpp"
#include "camera.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "quad.hpp"
#include "constant_medium.hpp"

int main()
{
    hittable_list world;

    // Cornell box

    auto red   = std::make_shared<lambertian>(color(.65, .05, .05));
    auto white = std::make_shared<lambertian>(color(.73, .73, .73));
    auto green = std::make_shared<lambertian>(color(.12, .45, .15));
    auto light = std::make_shared<diffuse_light>(color(15, 15, 15));
    auto see_thru = std::make_shared<one_sided_material>(white);
    auto sphere_mat = std::make_shared<metal>(color(1,1,1));

    world.add(std::make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(std::make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(std::make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(std::make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(std::make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(std::make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));
    world.add(std::make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,555,0), see_thru));

    world.add(std::make_shared<sphere>(point3(278,278, 300),100,sphere_mat));
    

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 5000;
    cam.max_bounces       = 25;
    cam.lookfrom = point3(278, 278, -1000);
    cam.lookat   = point3(278, 278, 0);
    cam.up     = vec3(0,1,0);

    cam.render(world);
}