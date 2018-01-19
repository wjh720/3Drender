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
    
    virtual Vector3 get_source() const override {return point_o;}
    virtual Collision collide(const Ray &ray) const override;
    virtual double get_shadow_ratio(const Scene* scene, const Vector3 &p) const override;
    virtual Photon emit_photon(double power) const override;
};


#endif // Pointlight_h
