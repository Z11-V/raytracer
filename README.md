# Raytracer ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
This project implements a simple ray tracer and is written in C++. Based on  Peter Shirley's book series "Ray tracing in one weekend"
## Renders

![Cornell Box](images/cornell_box.png?raw=true "Cornell Box")
<br>
*Cornell Box*

![Spheres](images/spheres.png?raw=true "Spheres")
<br>

## Features
**Monte Carlo Ray Tracing** : A basic implementation of Monte Carlo methods for sampling in rendering.

**Materials** :
* Diffuse: Lambertian material (idealized diffuse reflection).
* Metal: Specular reflection with metallic surface properties.
* Emissive: Materials that emit light, simulating light sources.
* Dielectric: Transparent material with refraction, using Snell's law for simulating glass-like materials.

**Primitives** :
* Sphere: Standard sphere shape for rendering.
* Quad: A flat quadrilateral primitive for more general scene shapes.
* Triangles: A triangle primitive to support meshes.

**Texturing** : Support for applying 2D textures to materials.

