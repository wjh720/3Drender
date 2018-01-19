#include "sphere.h"
#include "const.h"

#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <bitset>
#include <cmath>
#include <ctime>
#include <queue>
#include <set>
#include <map>

Sphere::Sphere(std::ifstream &fin) {
    int n;
    std::string s = "";
    fin >> n;
    Material* material;
    Vector3 dx, dz;
    bool flag = 0;
    for (int i = 0; i < n; i++) {
        fin >> s;
        if (s == "o_material")
            material = Material::load_ifstream(fin);
        else if (s == "sphere_o")
            sphere_o = Vector3(fin);
        else if (s == "sphere_r")
            fin >> sphere_r;
        else if (s == "sphere_dx")
            dx = Vector3(fin), flag = 1;
        else if (s == "sphere_dz")
            dz = Vector3(fin), flag = 1;
        else std::cerr << "Sphere error!" << std::endl;
    }
    (*this) = Sphere(sphere_o, sphere_r, material);
    if (flag)
        set_texture_axis(dz, dx);
}

Sphere::Sphere(const Vector3 &o, const double &r, const Material* m) : Object(m) {
    sphere_o = o, sphere_r = r, sphere_dz = Vector3(0, 0, 1), sphere_dx = Vector3(1, 0, 0);
}

Collision Sphere::collide_ray(const Ray &ray) const {
    Ray normal_ray = ray.normal();
    Vector3 center_dir = sphere_o - normal_ray.ray_start;
    double center_dot = center_dir.dot(normal_ray.ray_direction);
    double a_2 = center_dot * center_dot + sphere_r * sphere_r - center_dir.length2();
    if (a_2 > Const::eps) {
        double a = std::sqrt(a_2);
        double res_1 = center_dot - a, res_2 = center_dot + a;
        if (res_1 > Const::eps) {
            Vector3 nn = normal_ray.get(res_1) - sphere_o;
            return Collision(normal_ray, res_1, nn, this, false);
        } else if (res_2 > Const::eps) {
            Vector3 nn = normal_ray.get(res_2) - sphere_o;
            return Collision(normal_ray, res_2, -nn, this, true);
        }
    }
    return Collision();
}

Color Sphere::get_texture_color(const Collision &coll) const {
    if (o_material -> has_texture()) {
        double b = acos(coll.co_n.dot(sphere_dz));
        double a = acos(std::min(std::max(coll.co_n.dot(sphere_dx) / sin(b), -1.), 1.));
        double v = b / Const::pi, u = a / 2 / Const::pi;
        if (coll.co_n.dot(sphere_dz * sphere_dx) < 0) u = 1 - u;
        return o_material -> get_texture_color(u, v);
    }
    return Color(1, 1, 1);
}

