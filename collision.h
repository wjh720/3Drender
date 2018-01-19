#ifndef Collision_h
#define Collision_h

#include "ray.h"

class Object;
class Light;

struct Collision {
    double co_distance;
    double co_u, co_v;
    bool co_is_internal;
    uint64 co_object_label;
    const Object* co_object;
    const Light* co_light;
    Ray co_ray;
    Vector3 co_p, co_n;
    
    Collision();
    Collision(const Ray& r, double t, const Vector3 &nn, const Object* ob, bool in_flag, uint64 label = 0);
    Collision(const Ray& r, double t, const double &u, const double &v,
              const Vector3 &nn, const Object* ob, bool in_flag, uint64 label = 0);
    
    Collision(const Ray& r, double t, const Light* l);
    
    bool is_intersecting() const {return co_distance < 1e9;}
    bool intersect_with_object() const {return co_object != nullptr;}
    bool intersect_with_light() const {return co_light != nullptr;}
};


#endif // Collision_h
