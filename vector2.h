#ifndef Vector2_h
#define Vector2_h

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

#include <fstream>

struct Vector2 {
    double x, y;
    
    Vector2() {x = y = 0;}
    Vector2(double xx, double yy) {x = xx, y = yy;}
    Vector2(std::ifstream &fin) {fin >> x >> y;}
    
    Vector2 operator -() const {return Vector2(-x, -y);}
    Vector2 operator +() const {return Vector2(x, y);}
    Vector2 operator /(double A) const {return Vector2(x / A, y / A);}
    Vector2 operator *(double A) const {return Vector2(x * A, y * A);}
    
    double dot(const Vector2 &A) const {return x * A.x + y * A.y;}
    
    double length2() const {return x * x + y * y;}
    double length() const {return sqrt(length2());}
    
    Vector2 normal() const {
        double len = length();
        return *(this) / (len < Const::eps ? 1 : len);
    }
    
    Vector2 min(const Vector2 &A) const {return Vector2(std::min(x, A.x), std::min(y, A.y));}
    Vector2 max(const Vector2 &A) const {return Vector2(std::max(x, A.x), std::max(y, A.y));}
    
    double polar_angle() const {return atan2(y, x);}
    
    void print() const {
        printf("%lf %lf\n", x, y);
    }
};

inline Vector2 operator + (const Vector2 &A, const Vector2 &B) {return Vector2(A.x + B.x, A.y + B.y);}
inline Vector2 operator - (const Vector2 &A, const Vector2 &B) {return Vector2(A.x - B.x, A.y - B.y);}
inline double operator * (const Vector2 &A, const Vector2 &B) {return A.x * B.y - A.y * B.x;}

#endif // Vector2_h
