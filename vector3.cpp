#include "const.h"
#include "vector3.h"

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

Vector3 Vector3::get_an_orthogonal_vector() const {
    Vector3 a = (*this) * Vector3(1, 0, 0);
    if (a.length2() < Const::eps) a = (*this) * Vector3(0, 1, 0);
    return a.normal();
}

Vector3 Vector3::reflect(const Vector3 &n) const {
    return *this - n * (2 * this -> dot(n));
}

Vector3 Vector3::refract(const Vector3 &n, const double &refractivity) const {
    double ratio = 1 / refractivity; // nI / nT
    double cos_I = this -> dot(n), cos_T = 1 - ratio * ratio * (1 - cos_I * cos_I);
    if (cos_T >= 0) return (*this) * ratio - n * (std::sqrt(cos_T) + ratio * cos_I);
    else return Vector3();
}

Vector3 Vector3::diffuse() const {
    double theta = acos(sqrt(Const::Rand_double())), phi = 2 * Const::pi * Const::Rand_double();
    Vector3 dir(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));
    Vector3 dx = get_an_orthogonal_vector(), dy = (*this) * dx;
    return Vector3(dx * dir.x + dy * dir.y + (*this) * dir.z);
}

Vector3 Vector3::random_normal_vector() {
    Vector3 a;
    for (;;) {
        a = Vector3(Const::Rand_double(), Const::Rand_double(), Const::Rand_double());
        if (a.length2() > Const::eps || a.length2() < 1 - Const::eps) break;
    }
    return a;
}
