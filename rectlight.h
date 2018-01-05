#ifndef Rectlight_h
#define Rectlight_h

#include "vector3.h"
#include "light.h"

class Rectlight : public Light {
private:
    Vector3 rect_center, rect_normal;   // 矩形中心点、光源朝向
    Vector3 rect_dx, rect_dy;           // 横向、纵向向量
    
public:
    Rectlight(const Color &color, const Vector3 &center, const Vector3 &n, const Vector3 &dx,
              const Vector3 & dy, double power = 1);
    Rectlight(std::ifstream &fin);
    
    // 发光点
    virtual Vector3 get_source() const override {return rect_center;}
    
    // 与视线相交
    virtual Collision collide(const Ray &ray) const override;
    
    // 计算阴影比例
    virtual double get_shadow_ratio(const Scene* scene, const Vector3 &p) const override;

    // 发射光子
    virtual Photon emit_photon(double power) const override;
};


#endif // Rectlight_h
