#ifndef Rotationbody_h
#define Rotationbody_h

#include "vector3.h"
#include "object.h"
#include "bezier_curve3.h"

#include <vector>

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
    
    Vector3 rb_center;              // 底面中心点
    Curves rb_curves;               // 曲线
    double rb_r, rb_h, rb_angle;    // 包围圆柱体的底面半径、高，纹理起点极角
    std::vector<double> rb_texture_ratios, rb_texture_ratios_sum;   // 每个子曲面纹理的比例，比例前缀和
    
    Cylinder* rb_bounding_cy;                       // 包围圆柱体
    std::vector<Cylinder*> rb_sub_bounding_cy;      // 子旋转面的包围圆柱体
    std::vector<uint64> rb_labels;
    
    void rb_init();
    
public:
    Rotation_body(const Vector3 &center, const Curves &curves = {}, const Material* m = nullptr);
    Rotation_body(std::ifstream &fin);
    
    virtual std::string get_type() const override {return "Rotation body!";}
    
    // 与视线相交
    virtual Collision collide_ray(const Ray &ray) const override;
    
    // 交点处的纹理颜色
    virtual Color get_texture_color(const Collision &coll) const override;
    
    // 设置底面中心点
    void set_center(const Vector3 &center) {rb_center = center;}
    
    // 设置纹理起点极角
    void set_texture_angle(double a) {rb_angle = a;}
    
    // 设置子曲面纹理比例
    void set_texture_ratios(const std::vector<double> &ratios) {rb_texture_ratios = ratios;}
    
    Bezier_curve3 get_curve(int i) const {return rb_curves[i];}
    void add_curve(const Bezier_curve3 &curve) {rb_curves.push_back(curve);}
};


#endif // Rotationbody_h
