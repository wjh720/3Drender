#ifndef Bezier_curve3_h
#define Bezier_curve3_h

#include "vector2.h"

#include <vector>

struct Bezier_curve3 {
    Vector2 b3_P0, b3_P1, b3_P2, b3_P3, b3_Mi, b3_Ma;
    
    Bezier_curve3(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3);
    
    
    Vector2 get_p(double t) const;                                              // 曲线上一点 P(t)
    Vector2 get_dp(double t) const;                                             // 曲线上一点 P(t) 的切向量 P'(t)
    void get_para(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 &p3) const;    // 曲线方程各项的系数
    
    static std::vector<Bezier_curve3> load_ifstream(std::ifstream &fin);
};

#endif // Bezier_curve3_h
