#include "plane.h"

#include <iostream>

Plane::Plane(const Vector3 &n, double d, const Material* m) : Object(m) {
    plane_n = n, plane_d = d, plane_o = Vector3(0, 0, 0),
    plane_dx = n.get_an_orthogonal_vector(), plane_dy = (n * plane_dx).normal();
}

Collision Plane::collide_ray(const Ray &ray) const {
    double n = plane_n.dot(ray.ray_start) + plane_d;
    double d = plane_n.dot(ray.ray_direction);
    if (std::abs(d) < Const::eps) return Collision();
    double t = -n / d;
    if (t < Const::eps) return Collision();
    if (n > Const::eps) return Collision(ray, t, plane_n, this, false);
    else return Collision(ray, t, -plane_n, this, true);
}

Color Plane::get_texture_color(const Collision &colli) const {
    if (o_material -> has_texture()) {
        double x = (colli.co_p - plane_o).dot(plane_dx) / plane_dx.length2();
        double y = (colli.co_p - plane_o).dot(plane_dy) / plane_dy.length2();
        x -= floor(x), y -= floor(y);
        return o_material -> get_texture_color(x, y);
    } else return Color(1, 1, 1);
}

Plane::Plane(std::ifstream &fin) : Object() {
    int n;
    std::string s = "";
    fin >> n;
    Material* material;
    for (int i = 0; i < n; i++) {
        fin >> s;
        if (s == "o_material")
            material = Material::load_ifstream(fin);
        else if (s == "plane_n")
            plane_n = Vector3(fin);
        else if (s == "plane_d")
            fin >> plane_d;
        else std::cerr << "Plane error!" << std::endl;
    }
    (*this) = Plane(plane_n, plane_d, material);
}
