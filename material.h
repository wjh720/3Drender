#ifndef Material_h
#define Material_h

#include "color.h"
#include "bmp.h"
#include "vector3.h"

struct Material {
private:
    Bmp* m_texture;
    Color (*m_texture_func)(double, double);
    
public:
    Color m_color, m_absorb_color;
    double m_diffuse, m_spec;
    double m_refl, m_refr;
    double m_refractivity; // d = nT / nI
    
    static Material* load_ifstream(std::ifstream &fin);
    
    Material();
    Material(const Color &c, const double &d, const double &s);
    Material(const Color &c, const double &d, const double &s, const double &rl);
    Material(const Color &c, const double &d, const double &s, const double &rl, const double &rr,
             const double &rr_ty, const Color &ab_color);
    
    bool has_texture() const {return m_texture || m_texture_func;}
    void set_texture(Bmp* texture) {m_texture_func = nullptr, m_texture = texture;}
    void set_texture(Color (*func)(double, double)) {
        if (m_texture) delete m_texture;
        m_texture = nullptr, m_texture_func = func;
    }
    
    Color get_texture_color(double x, double y) const;
    bool compare(const Material* B) const;
    double get_BRDF(const Vector3 &l, const Vector3 &n, const Vector3 &v) const;
};


#endif // Material_h
