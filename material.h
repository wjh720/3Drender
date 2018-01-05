#ifndef Material_h
#define Material_h

#include "color.h"
#include "bmp.h"
#include "vector3.h"

struct Material {
private:
    Bmp* m_texture;                          // 纹理图片
    Color (*m_texture_func)(double, double); // 纹理函数
    
public:
    Color m_color, m_absorb_color;
    double m_diffuse, m_spec; // 漫反射系数，镜面反射系数
    double m_refl, m_refr; // 反射和折射光的比例r
    double m_refractivity; // 折射率d = nT / nI
    
    Material();
    // 漫反射材质
    Material(const Color &c, const double &d, const double &s);
    // 不透明反射材质
    Material(const Color &c, const double &d, const double &s, const double &rl);
    // 透明材质
    Material(const Color &c, const double &d, const double &s, const double &rl, const double &rr,
             const double &rr_ty, const Color &ab_color);
    
    bool has_texture() const {return m_texture || m_texture_func;}
    void set_texture(Bmp* texture) {m_texture_func = nullptr, m_texture = texture;}
    void set_texture(Color (*func)(double, double)) {
        if (m_texture) delete m_texture;
        m_texture = nullptr, m_texture_func = func;
    }
    
    // 获得纹理颜色
    Color get_texture_color(double x, double y) const;
    
    // 求交时的优先级
    bool compare(const Material* B) const;
    
    // BRDF 函数值
    double get_BRDF(const Vector3 &l, const Vector3 &n, const Vector3 &v) const;
    
    // 从文件导入材料
    static Material* load_ifstream(std::ifstream &fin);
};


#endif // Material_h
