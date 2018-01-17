#ifndef Sphere_h
#define Sphere_h

#include "vector3.h"
#include "object.h"

class Sphere : public Object {
private:
    Vector3 sphere_o;
    double sphere_r;
    Vector3 sphere_dz, sphere_dx; // 北极向量和 0度经线方向
    
public:
    Sphere(const Vector3 &o, const double &r, const Material* m = nullptr);
    Sphere(std::ifstream &fin);
    
    virtual std::string get_type() const override {return "Sphere!";}
    // 与视线相交
    virtual Collision collide_ray(const Ray &ray) const override;
    virtual Color get_texture_color(const Collision &coll) const override;
    
    void set_texture_axis(const Vector3 & dz, const Vector3 & dx) {
        sphere_dz = dz, sphere_dx = dx;
    }
};


#endif // Sphere_h
