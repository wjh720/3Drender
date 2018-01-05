#ifndef Vector3_h
#define Vector3_h

#include "const.h"
#include <cstdio>
#include <cmath>

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
    Vector3 operator -=(const Vector3 &A) const {return Vector3(x - A.x, y - A.y, z - A.z);}
    Vector3 operator +=(const Vector3 &A) const {return Vector3(x + A.x, y + A.y, z + A.z);}
    
    double operator [](int i) const {return !i ? x : i == 1 ? y : z;}
    
    double dot(const Vector3 &A) const {return x * A.x + y * A.y + z * A.z;}
    
    double length2() const {return x * x + y * y + z * z;}
    double length() const {return sqrt(length2());}
    
    Vector3 normal() const {
        double len = length();
        return *(this) / (len < Const::eps ? 1 : len);
    }
    
    // 混合积
    //double mix(const Vector3 &A, const Vector3 &B, const Vector3 &C);
    
    // 转为二维向量，忽略第 3 维
    //Vector2 to_Vector2() {return Vector2(x, y);}
    
    // 任意一个与自己垂直的向量
    Vector3 get_an_orthogonal_vector() const;
    
    // 反射
    Vector3 reflect(const Vector3 &n) const;
    
    // 折射
    Vector3 refract(const Vector3 &n, const double &refractivity) const;
    
    // 散射
    Vector3 diffuse() const;
    
    // 随机单位向量
    static Vector3 random_normal_vector();
    
    void print() const {
        printf("%lf %lf %lf\n", x, y, z);
    }
};

inline Vector3 operator + (const Vector3 &A, const Vector3 &B) {return Vector3(A.x + B.x, A.y + B.y, A.z + B.z);}
inline Vector3 operator - (const Vector3 &A, const Vector3 &B) {return Vector3(A.x - B.x, A.y - B.y, A.z - B.z);}
inline Vector3 operator * (const Vector3 &A, const Vector3 &B) {
    return Vector3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
}


#endif // Vector3_h
