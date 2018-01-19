#ifndef Object_h
#define Object_h

#include "collision.h"
#include "material.h"

class Plane;
class Sphere;
class Cylinder;
class Rotation_body;

class Object {
protected:
    const Material* o_material;
    uint64 o_label;
    
    Object() {}
    Object(const Material* m);
    Object(std::ifstream &fin);
    
public:
    
    static Object* load_ifstream(std::ifstream &fin);
    
    uint64 get_label() const {return o_label;}
    const Material* get_material() const {return o_material;}
    
    virtual std::string get_type() const = 0;
    virtual Collision collide_ray(const Ray &ray) const = 0;
    virtual Color get_texture_color(const Collision &colli) const = 0;
    
};


#endif // Object_h
