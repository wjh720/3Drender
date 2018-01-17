#include "bezier_curve3.h"

#include <vector>
#include <iostream>

Bezier_curve3::Bezier_curve3(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3) {
    b3_P0 = p0, b3_P1 = p1, b3_P2 = p2, b3_P3 = p3,
    b3_Mi = p0.min(p1.min(p2.min(p3))), b3_Ma = p0.max(p1.max(p2.max(p3)));
}

Vector2 Bezier_curve3::get_p(double t) const {
    double t2 = t * t, t3 = t2 * t;
    double v1 = 1 - t, v2 = v1 * v1, v3 = v2 * v1;
    return b3_P0 * v3 + b3_P1 * (3 * v2 * t) + b3_P2 * (3 * v1 * t2) + b3_P3 * t3;
}

Vector2 Bezier_curve3::get_dp(double t) const {
    double v = 1 - t;
    double D = 3 * t * t, C = 3 * (2 * t * v - t * t);
    double B = 3 * (v * v - 2 * t * v), A = -3 * v * v;
    return b3_P0 * A + b3_P1 * B + b3_P2 * C + b3_P3 * D;
}

//?
void Bezier_curve3::get_para(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 &p3) const {
    p0 = b3_P0,
    p1 = (b3_P1 - b3_P0) * 3,
    p2 = (b3_P2 - b3_P1 * 2 + b3_P0) * 3,
    p3 = b3_P3 - (b3_P2 - b3_P1) * 3 - b3_P0;
}

std::vector<Bezier_curve3> Bezier_curve3::load_ifstream(std::ifstream &fin) {
    int n;
    std::vector<Bezier_curve3> curves;
    std::string s = "";
    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> s;
        curves.push_back(Bezier_curve3(Vector2(fin), Vector2(fin), Vector2(fin), Vector2(fin)));
    }
    return curves;
}
