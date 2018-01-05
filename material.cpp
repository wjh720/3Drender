#include "vector3.h"
#include "const.h"
#include "material.h"
#include "config.h"

#include <iostream>

Material::Material() {
    m_color = Color(1, 1, 1), m_absorb_color = Color(0, 0, 0),
    m_diffuse = 0.8, m_spec = 0.2, m_refl = 0, m_refr = 0, m_refractivity = 1,
    m_texture = nullptr, m_texture_func = nullptr;
}

Material::Material(const Color &c, const double &d, const double &s) {
    m_color = c, m_absorb_color = Color(0, 0, 0),
    m_diffuse = d, m_spec = s, m_refl = 0, m_refr = 0, m_refractivity = 1,
    m_texture = nullptr, m_texture_func = nullptr;
}

Material::Material(const Color &c, const double &d, const double &s, const double &rl) {
    m_color = c, m_absorb_color= Color(0, 0, 0),
    m_diffuse = d, m_spec = s, m_refl = rl, m_refr = 0, m_refractivity = 1,
    m_texture = nullptr, m_texture_func = nullptr;
}

Material::Material(const Color &c, const double &d, const double &s, const double &rl, const double &rr,
                   const double &rr_ty, const Color & ab_color) {
    m_color = c, m_absorb_color = ab_color,
    m_diffuse = d, m_spec = s, m_refl = rl, m_refr = rr, m_refractivity = rr_ty,
    m_texture = nullptr, m_texture_func = nullptr;
}

Color Material::get_texture_color(double x, double y) const {
    return m_texture ? m_texture -> getColor(x, y) : m_texture_func ? m_texture_func(x, y) : Color(1, 1, 1);
}

bool Material::compare(const Material* B) const {
    return this -> m_refl + Const::eps < B -> m_refl || (std::abs(this -> m_refl - B -> m_refl) < Const::eps &&
            this -> m_refr + Const::eps < B -> m_refr);
}

double Material::get_BRDF(const Vector3 &l, const Vector3 &n, const Vector3 &v) const {
    Vector3 r = l.reflect(n);
    return m_diffuse * l.dot(n) + m_spec * std::pow(r.dot(v), Config::hightlight_exponent);
}

Material* Material::load_ifstream(std::ifstream &fin) {
    int n;
    std::string s = "";
    Material material;
    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> s;
        if (s == "m_color")
            material.m_color = Color(fin);
        else if (s == "m_diffuse")
            fin >> material.m_diffuse;
        else if (s == "m_spec")
            fin >> material.m_spec;
        else if (s == "m_absorb_color")
            material.m_absorb_color = Color(fin);
        else if (s == "m_refl")
            fin >> material.m_refl;
        else if (s == "m_refr")
            fin >> material.m_refr;
        else if (s == "m_refractivity")
            fin >> material.m_refractivity;
        else std::cerr << "Material error!" << std::endl;
                
        // not used till now
        /*
        case "texture":
            material.m_texture = new Bmp(fin);
            break;
        */
    }
    return new Material(material);
}
