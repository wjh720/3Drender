#ifndef Vector3_h
#define Vector3_h

#include "const.h"
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

#include <fstream>

struct Vector3 {
    double x, y, z;
    
    Vector3() {x = y = z = 0;}
    Vector3(double xx, double yy, double zz) {x = xx, y = yy, z = zz;}
    Vector3(std::ifstream &fin) {fin >> x >> y >> z;}
    
    Vector3 operator -() const {return Vector3(-x, -y, -z);}
    Vector3 operator +() const {return Vector3(x, y, z);}
    Vector3 operator /(double A) const {return Vector3(x / A, y / A, z / A);}
    Vector3 operator *(double A) const {return Vector3(x * A, y * A, z * A);}
    
    double operator [](int i) const {return !i ? x : i == 1 ? y : z;}
    
    double dot(const Vector3 &A) const {return x * A.x + y * A.y + z * A.z;}
    
    double length2() const {return x * x + y * y + z * z;}
    double length() const {return sqrt(length2());}
    
    Vector3 normal() const {
        double len = length();
        return *(this) / (len < Const::eps ? 1 : len);
    }
    
    double mix(const Vector3 &A, const Vector3 &B, const Vector3 &C) {return A.dot(B * C);}
    Vector2 to_Vector2() {return Vector2(x, y);}
    Vector3 get_an_orthogonal_vector() const;
    Vector3 reflect(const Vector3 &n) const;
    Vector3 refract(const Vector3 &n, const double &refractivity) const;
    Vector3 diffuse() const;
    static Vector3 random_normal_vector();
    
    Vector3 operator +(const Vector3 &A) const {return Vector3(x + A.x, y + A.y, z + A.z);}
    Vector3 operator -(const Vector3 &A) const {return Vector3(x - A.x, y - A.y, z - A.z);}
    Vector3 operator *(const Vector3 &A) const {
        return Vector3(y * A.z - z * A.y, z * A.x - x * A.z, x * A.y - y * A.x);
    }
    
    void print() const {
        printf("%lf %lf %lf\n", x, y, z);
    }
};


#endif // Vector3_h
