#ifndef Photon_h
#define Photon_h

#include "vector3.h"
#include "color.h"

struct Photon {
    Vector3 p_start, p_direction;
    Color p_power;
    
    Photon() {p_start = Vector3(), p_direction = Vector3(), p_power = Color();}
    Photon(const Vector3 &start, const Vector3 &direction, const Color &power) {
        p_start = start, p_direction = direction, p_power = power;
    }
};


#endif // Photon_h
