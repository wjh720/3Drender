#include "const.h"
#include "cylinder.h"

#include <iostream>
#include <unistd.h>

Cylinder::Cylinder(const Vector3 &center, double r, double h, const Material* m) : Object(m) {
    cy_center = center, cy_r = r, cy_h = h, cy_angle = 0;
}

Collision Cylinder::collide_ray(const Ray &ray) const {
    Ray normal_ray = ray.normal();
    Vector3 r_c = ray.ray_start;
    Vector3 dir = normal_ray.ray_direction;
    Vector2 dir2 = dir.to_Vector2();
    Vector2 center_dir2 = (cy_center - r_c).to_Vector2();
    bool flag_in = r_c.z > cy_center.z - Const::eps && r_c.z < cy_center.z + cy_h + Const::eps &&
                    center_dir2.length2() < cy_r * cy_r + Const::eps;
    
    // 与两个底面求交
    if (std::abs(dir.z) > Const::eps) {
        double res1 = (cy_center.z - r_c.z) / dir.z, u1 = 0;
        double res2 = (cy_center.z + cy_h - r_c.z) / dir.z, u2 = 1;
        if (res1 > res2)
            std::swap(res1, res2), std::swap(u1, u2);
        if (res1 > Const::eps) {                                        // 若射线和第一个底面相交，直接返回
            Vector2 A = (normal_ray.get(res1) - cy_center).to_Vector2();
            if (A.length2() < cy_r * cy_r + Const::eps)
                return Collision(normal_ray, res1, u1, A.polar_angle(), Vector3(0, 0, -dir.z), this, flag_in);
        } else if (res2 > Const::eps) {                                 // 若射线和第二个底面相交，且射线起点在圆柱体内，也直接返回
            Vector2 A = (normal_ray.get(res2) - cy_center).to_Vector2();
            if (A.length2() < cy_r * cy_r + Const::eps && flag_in)
                return Collision(normal_ray, res2, u2, A.polar_angle(), Vector3(0, 0, -dir.z), this, flag_in);
        } else                                                          // 若射线不和上下底面所在的平面相交，则无交点
            return Collision();
    }
    
    // 若射线垂直于 xy 平面，则交点不会在圆柱面上
    if (dir2.length2() < Const::eps)
        return Collision();
    
    // 否则第一个交点为圆柱面
    double center_dot = center_dir2.dot(dir2.normal());
    double a_2 = center_dot * center_dot + cy_r * cy_r - center_dir2.length2();
    double res = -1;
    if (a_2 > Const::eps) {
        double a = std::sqrt(a_2);
        double res_1 = center_dot - a, res_2 = center_dot + a;
        if (res_1 > Const::eps)
            res = res_1;
        else if (res_2 > Const::eps)
            res = res_2;
    }
    if (res > Const::eps) {
        res /= dir2.length();
        Vector3 A = normal_ray.get(res);
        if (A.z > cy_center.z - Const::eps && A.z < cy_center.z + cy_h + Const::eps)
            return Collision(normal_ray, res, (A.z - cy_center.z) / cy_h, (A - cy_center).to_Vector2().polar_angle(),
                             Vector3(A.x - cy_center.x, A.y - cy_center.y, 0) * (flag_in ? -1 : 1), this, flag_in);
    }
    return Collision();
}

//?
Color Cylinder::get_texture_color(const Collision &coll) const {
    if (o_material -> has_texture()) {
        double v = 1 - coll.co_u;
        double u = fmod(coll.co_v - cy_angle + 4 * Const::pi, Const::pi * 2) / 2 / Const::pi;
        return o_material -> get_texture_color(u, v);
    }
    return Color(1, 1, 1);
}

Cylinder::Cylinder(std::ifstream &fin) {
    int n;
    std::string s = "";
    fin >> n;
    Material* material;
    for (int i = 0; i < n; i++) {
        fin >> s;
        if (s == "o_material")
            material = Material::load_ifstream(fin);
        else if (s == "cy_center")
            cy_center = Vector3(fin);
        else if (s == "cy_r")
            fin >> cy_r;
        else if (s == "cy_h")
            fin >> cy_h;
        else std::cerr << "Cylinder error!" << std::endl;
    }
    (*this) = Cylinder(cy_center, cy_r, cy_h, material);
}
