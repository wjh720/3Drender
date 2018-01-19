#ifndef Rectlight_h
#define Rectlight_h

#include "vector3.h"
#include "light.h"

class Rectlight : public Light {
private:
    Vector3 rect_center, rect_normal; 
    Vector3 rect_dx, rect_dy;
    
public:
    Rectlight(const Color &color, const Vector3 &center, const Vector3 &n, const Vector3 &dx,
              const Vector3 & dy, double power = 1);
    Rectlight(std::ifstream &fin);
    
    virtual Vector3 get_source() const override {return rect_center;}
    virtual Collision collide(const Ray &ray) const override;
    virtual double get_shadow_ratio(const Scene* scene, const Vector3 &p) const override;
    virtual Photon emit_photon(double power) const override;
};


#endif // Rectlight_h
