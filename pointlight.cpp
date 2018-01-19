#include "const.h"
#include "pointlight.h"
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

Pointlight::Pointlight(std::ifstream &fin) : Light() {
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
        else if (s == "point_o")
            point_o = Vector3(fin);
        else std::cerr << "Pointlight error!" << std::endl;
    }
    (*this) = Pointlight(color, point_o, power);
}

Pointlight::Pointlight(const Color &color, const Vector3 &o, double power) : Light(color, power) {
    point_o = o;
}

Collision Pointlight::collide(const Ray &ray) const {
    return Collision();
}

double Pointlight::get_shadow_ratio(const Scene* scene, const Vector3 &p) const {
    Vector3 dir = p - point_o;
    double t = dir.length();
    for (auto obj = scene -> objects_begin(); obj != scene -> objects_end(); obj++) {
        Collision colli = (*obj) -> collide_ray(Ray(point_o, dir));
        if (colli.is_intersecting() && colli.co_distance + Const::eps < t) return 0;
    }
    return 1;
}

Photon Pointlight::emit_photon(double power) const {
    return Photon(point_o, Vector3::random_normal_vector(), l_color * power);
}


