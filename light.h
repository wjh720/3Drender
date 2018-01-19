#ifndef Light_h
#define Light_h

#include "color.h"
#include "collision.h"
#include "photon.h"

class Scene;

class Light {
protected:
    Color l_color;
    double l_power;
    uint64 l_label;
    
public:
    static Light* load_ifstream(std::ifstream &fin);
    
    Light(const Color &color, double power = 1);
    Light() {}
    
    uint64 get_label() const {return l_label;}
    double get_power() const {return l_power;}
    Color get_color() const {return l_color;}
    
    virtual Vector3 get_source() const = 0;
    virtual Collision collide(const Ray &ray) const = 0;
    virtual double get_shadow_ratio(const Scene* scene, const Vector3 &p) const = 0;
    virtual Photon emit_photon(double power) const = 0;
    
    
};


#endif // Light_h
