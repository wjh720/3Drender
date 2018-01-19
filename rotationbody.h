#ifndef Rotationbody_h
#define Rotationbody_h

#include "vector3.h"
#include "object.h"
#include "bezier_curve3.h"

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

struct Poly {
    long double a[7];
    Poly(const long double a[7]);
    Poly();
    
    void operator += (Poly &A);
    void operator -= (Poly &A);
    
    void square();
    void operator *= (double A);
    void operator /= (double A);
    void operator += (double A);
    void operator -= (double A);
    
    Poly operator -();
};

class Rotation_body : public Object {
private:
    typedef std::vector<Bezier_curve3> Curves;
    
    Vector3 rb_center;
    Curves rb_curves;
    double rb_r, rb_h, rb_angle;
    std::vector<double> rb_texture_ratios, rb_texture_ratios_sum;
    
    Cylinder* rb_bounding_cy;
    std::vector<Cylinder*> rb_sub_bounding_cy;
    std::vector<uint64> rb_labels;
    
    void rb_init();
    
public:
    Rotation_body(const Vector3 &center, const Curves &curves = {}, const Material* m = nullptr);
    Rotation_body(std::ifstream &fin);
    
    virtual std::string get_type() const override {return "Rotation body!";}
    virtual Collision collide_ray(const Ray &ray) const override;
    virtual Color get_texture_color(const Collision &coll) const override;
    void set_center(const Vector3 &center) {rb_center = center;}
    void set_texture_angle(double a) {rb_angle = a;}
    void set_texture_ratios(const std::vector<double> &ratios) {rb_texture_ratios = ratios;}
    
    Bezier_curve3 get_curve(int i) const {return rb_curves[i];}
    void add_curve(const Bezier_curve3 &curve) {rb_curves.push_back(curve);}
};


#endif // Rotationbody_h
