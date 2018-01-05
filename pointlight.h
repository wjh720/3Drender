#ifndef Pointlight_h
#define Pointlight_h

#include "vector3.h"
#include "light.h"

class Pointlight : public Light {
private:
    Vector3 point_o;
    
public:
    Pointlight(const Color &color, const Vector3 &o, double power = 1);
    Pointlight(std::ifstream &fin);
    
    // 发光点
    virtual Vector3 get_source() const override {return point_o;}
    
    // 与视线相交
    virtual Collision collide(const Ray &ray) const override;
    
    // 计算阴影比例
    virtual double get_shadow_ratio(const Scene* scene, const Vector3 &p) const override;
    
    // 发射光子
    virtual Photon emit_photon(double power) const override;
};


#endif // Pointlight_h
