#ifndef Cylinder_h
#define Cylinder_h

#include "vector3.h"
#include "object.h"

class Cylinder : public Object {
private:
    Vector3 cy_center;              // 底面圆心
    double cy_r, cy_h, cy_angle;    // 底面半径，高，纹理起点极角
    
public:
    Cylinder(const Vector3 &center, double r, double h, const Material* m = nullptr);
    Cylinder(std::ifstream &fin);
    
    virtual std::string get_type() const override {return "Cylinder!";}
    
    // 与视线相交
    virtual Collision collide_ray(const Ray &ray) const override;
    
    // 交点处的纹理颜色
    virtual Color get_texture_color(const Collision &coll) const override;
    
    // 设置底面中心点
    void set_center(const Vector3 &center) {cy_center = center;}
    
    // 设置纹理起点极角
    void set_texture_angle(double a) {cy_angle = a;}
};


#endif // Cylinder_h
