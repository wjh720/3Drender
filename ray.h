#ifndef Ray_h
#define Ray_h

#include "vector3.h"
#include <iostream>

struct Ray {
    Vector3 ray_start, ray_direction;
    
    Ray() {ray_start = Vector3(), ray_direction = Vector3();}
    Ray(const Vector3 &start, const Vector3 &direction) {ray_start = start, ray_direction = direction;}
    
    // get a point in the ray
    Vector3 get(double t) const {return ray_start + ray_direction * t;}
    
    // normalize the direction
    Ray normal() const {return Ray(ray_start, ray_direction.normal());}
    
    void print() const {
        std::cerr << "Ray" << std::endl;
        ray_start.print();
        ray_direction.print();
        std::cerr << "End Ray" << std::endl;
    }
};


#endif // Ray_h
