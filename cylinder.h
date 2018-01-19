#ifndef Cylinder_h
#define Cylinder_h

#include "vector3.h"
#include "object.h"

class Cylinder : public Object {
private:
    Vector3 cy_center;
    double cy_r, cy_h, cy_angle;
    
public:
    Cylinder(const Vector3 &center, double r, double h, const Material* m = nullptr);
    Cylinder(std::ifstream &fin);
    
    virtual std::string get_type() const override {return "Cylinder!";}
    
    virtual Collision collide_ray(const Ray &ray) const override;
    virtual Color get_texture_color(const Collision &coll) const override;
    void set_center(const Vector3 &center) {cy_center = center;}
    void set_texture_angle(double a) {cy_angle = a;}
};


#endif // Cylinder_h
