#include "config.h"
#include "const.h"
#include "rectlight.h"
#include "object.h"
#include "scene.h"

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

Rectlight::Rectlight(std::ifstream &fin) {
    int n;
    std::string s = "";
    fin >> n;
    Color color;
    double power = 1;
    for (int i = 0; i < n; i++) {
        fin >> s;
        if (s == "l_color")
            color = Color(fin);
        else if (s == "l_power")
            fin >> power;
        else if (s == "rect_normal")
            rect_normal = Vector3(fin);
        else if (s == "rect_center")
            rect_center = Vector3(fin);
        else if (s == "rect_dx")
            rect_dx = Vector3(fin);
        else if (s == "rect_dy")
            rect_dy = Vector3(fin);
        else std::cerr << "Rectlgiht error!" << std::endl;
    }
    (*this) = Rectlight(color, rect_center, rect_normal, rect_dx, rect_dy, power);
}


Rectlight::Rectlight(const Color &color, const Vector3 &center, const Vector3 &n, const Vector3 &dx,
          const Vector3 & dy, double power) : Light(color, power) {
    rect_center = center, rect_normal = n, rect_dx = dx, rect_dy = dy;
}

Collision Rectlight::collide(const Ray &ray) const {
    double dis = rect_normal.dot(ray.ray_direction);
    if (std::abs(dis) < Const::eps) return Collision();
    double t = (rect_normal.dot(rect_center) - rect_normal.dot(ray.ray_start)) / dis;
    //std::cerr << t << std::endl;
    if (t < Const::eps) return Collision();
    Vector3 ray_end = ray.get(t) - rect_center;
    if (std::abs(ray_end.dot(rect_dx)) + Const::eps < rect_dx.length2() &&
                 std::abs(ray_end.dot(rect_dy)) + Const::eps < rect_dy.length2())
        return Collision(ray, t, this);
    else return Collision();
}

double Rectlight::get_shadow_ratio(const Scene* scene, const Vector3 &p) const {
    int samples = Config::soft_shadow_samples, sum = samples * samples;
    for (int i = 0; i < samples; i++)
        for (int j = 0; j < samples; j++) {
            double x = (i + 0.5) * 2 / samples - 1;
            double y = (j + 0.5) * 2 / samples - 1;
            Vector3 sample_point = rect_center + rect_dx * x + rect_dy * y;
            Vector3 dir = sample_point - p;
            double t = dir.length();
            
            for (auto obj = scene -> objects_begin(); obj != scene -> objects_end(); obj++) {
                Collision colli = (*obj) -> collide_ray(Ray(p, dir));
                if (colli.is_intersecting() && colli.co_distance + Const::eps < t) {
                    sum--;
                    break;
                }
            }
        }
    return 1. * sum / samples / samples;
}

Photon Rectlight::emit_photon(double power) const {
    return Photon(rect_center + rect_dx * (Const::Rand_double() * 2 - 1) + rect_dy * (Const::Rand_double() * 2 - 1),
                  rect_normal.diffuse(), l_color * power);
}


