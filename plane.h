#ifndef Plane_h
#define Plane_h

#include "vector3.h"
#include "object.h"

class Plane : public Object {
private:
    Vector3 plane_n;
    double plane_d; // plane_n * t + plane_d = 0
    Vector3 plane_center, plane_dx, plane_dy; // 纹理原点和坐标轴向量
    
public:
    Plane(const Vector3 &n, double d, const Material* m = nullptr);
    Plane(std::ifstream &fin);
    
    virtual std::string get_type() const override {return "Plane!";}
    virtual Collision collide_ray(const Ray &ray) const override;
    virtual Color get_texture_color(const Collision &coll) const override;
    
    void set_texture_axis(const Vector3 &center, const Vector3 & dx, const Vector3 & dy) {
        plane_center = center, plane_dx = dx, plane_dy = dy;
    }
};


#endif // Plane_h
