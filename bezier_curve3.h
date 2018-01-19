#ifndef Bezier_curve3_h
#define Bezier_curve3_h

#include "vector2.h"

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

struct Bezier_curve3 {
    Vector2 b3_P0, b3_P1, b3_P2, b3_P3, b3_Mi, b3_Ma;
    
    Bezier_curve3(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3);
    
    
    Vector2 get_p(double t) const;
    Vector2 get_dp(double t) const;
    void get_para(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 &p3) const;
    
    static std::vector<Bezier_curve3> load_ifstream(std::ifstream &fin);
};

#endif // Bezier_curve3_h
