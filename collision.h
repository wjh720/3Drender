#ifndef Collision_h
#define Collision_h

#include "ray.h"

class Object;
class Light;

struct Collision {
    const Object* co_object;   // intersecting object
    const Light* co_light;     // intersecting light
    Ray co_ray;                // 射线
    Vector3 co_p, co_n;        // intersection and normal vector
    double co_distance;        // 交点对应的射线参数，ray_d 单位化后就是交点与射线起点间的距离
    double co_u, co_v;         // 如果是参数曲面，保存交点处的参数
    bool co_is_internal;       // 射线起点是否在物体内部（可选）
    uint64 co_object_label;    // 相交物体标识符
    
    // no intersecting
    Collision();
    
    // 与物体相交
    Collision(const Ray& r, double t, const Vector3 &nn, const Object* ob, bool in_flag, uint64 label = 0);
    
    // 与物体相交，包含参数
    Collision(const Ray& r, double t, const double &u, const double &v,
              const Vector3 &nn, const Object* ob, bool in_flag, uint64 label = 0);
    
    // 与光源相交
    Collision(const Ray& r, double t, const Light* l);
    
    bool is_intersecting() const {return co_distance < 1e9;}
    bool intersect_with_object() const {return co_object != nullptr;}
    bool intersect_with_light() const {return co_light != nullptr;}
};


#endif // Collision_h
