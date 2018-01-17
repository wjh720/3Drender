#ifndef Color_h
#define Color_h

#include "const.h"

#include <cstdio>
#include <cmath>
#include <fstream>

struct Color {
    double r, g, b;
    Color() {r = g = b = 0;}
    Color(const double &rr, const double &gg, const double &bb) {r = rr, g = gg, b = bb;}
    Color(std::ifstream &fin) {fin >> r >> g >> b;}
    
    Color operator -() {return Color(-r, -g, -b);}
    Color operator +() {return Color(r, g, b);}
    Color operator /(const double &A) const {return Color(r / A, g / A, b / A);}
    Color operator *(const double &A) const {return Color(r * A, g * A, b * A);}
    
    Color operator -=(const Color &A) {return (*(this) = Color(r - A.r, g - A.g, b - A.b));}
    Color operator +=(const Color &A) {return (*(this) = Color(r + A.r, g + A.g, b + A.b));}
    Color operator *=(const Color &A) {return (*(this) = Color(r * A.r, g * A.g, b * A.b));}
    Color operator /=(const double &A) {return (*(this) = Color(r / A, g / A, b / A));}
    Color operator *=(const double &A) {return (*(this) = Color(r * A, g * A, b * A));}
    
    double length2() {return r * r + g * g + b * b;}
    double length() {return sqrt(length2());}
    double power() {return (r + g + b) / 3;}
    Color exp() {return Color(std::exp(r), std::exp(g), std::exp(b));}
    Color inverse() {return Color(1 - r, 1 - g, 1 - b);}
    
    // 限制颜色取值为 [0, 1]
    double confine(double a) {return std::fmin(1, std::fmax(0, a));}
    Color confine() {return Color(confine(r), confine(g), confine(b));}
    
    void print() const {
        printf("%lf %lf %lf\n", r, g, b);
    }
};

inline Color operator + (const Color &A, const Color &B) {return Color(A.r + B.r, A.g + B.g, A.b + B.b);}
inline Color operator - (const Color &A, const Color &B) {return Color(A.r - B.r, A.g - B.g, A.b - B.b);}
inline Color operator * (const Color &A, const Color &B) {return Color(A.r * B.r, A.g * B.g, A.b * B.b);}


#endif // Color_h
