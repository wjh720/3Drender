#ifndef Light_h
#define Light_h

#include "color.h"
#include "collision.h"
#include "photon.h"

class Scene;

class Light {
protected:
    Color l_color;      // 光色
    double l_power;     // 光能
    uint64 l_label;     // 标识符
    
public:
    Light(const Color &color, double power = 1);
    Light() {}
    
    uint64 get_label() const {return l_label;}
    double get_power() const {return l_power;}
    Color get_color() const {return l_color;}
    
    // 发光点
    virtual Vector3 get_source() const = 0;
    
    // 与视线相交
    virtual Collision collide(const Ray &ray) const = 0;
    
    // 计算阴影比例，值越小阴影越深
    virtual double get_shadow_ratio(const Scene* scene, const Vector3 &p) const = 0;
    
    // 发射光子
    virtual Photon emit_photon(double power) const = 0;
    
    // 从文件导入光源
    static Light* load_ifstream(std::ifstream &fin);
};


#endif // Light_h
