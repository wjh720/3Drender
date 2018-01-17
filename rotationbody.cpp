#include "const.h"
#include "cylinder.h"
#include "rotationbody.h"
#include "polynomial6.h"
#include "bezier_curve3.h"

#include <unistd.h>
#include <iostream>

Poly::Poly() {
    for (int i = 0; i < 7; i++)
        a[i] = 0;
}

Poly::Poly(const long double aa[7]) {
    for (int i = 0; i < 7; i++)
        a[i] = aa[i];
}

void Poly::operator += (Poly &A) {
    for (int i = 0; i < 7; i++)
        a[i] += A.a[i];
}

void Poly::operator -= (Poly &A) {
    for (int i = 0; i < 7; i++)
        a[i] -= A.a[i];
}

void Poly::square() {
    Poly A;
    for (int i = 0; i <= 3; i++)
        for (int j = 0; j <= 3; j++)
            A.a[i + j] += a[i] * a[j];
    (*this) = A;
}

void Poly::operator *= (double A) {
    for (int i = 0; i < 7; i++)
        a[i] *= A;
}

void Poly::operator /= (double A) {
    for (int i = 0; i < 7; i++)
        a[i] /= A;
}

void Poly::operator += (double A) {
    a[0] += A;
}

void Poly::operator -= (double A) {
    a[0] -= A;
}

Poly Poly::operator -() {
    for (int i = 0; i < 7; i++)
        a[i] *= -1;
    return (*this);
}

Rotation_body::Rotation_body(const Vector3 &center, const Curves &curves, const Material* m) : Object(m) {
    rb_center = center, rb_curves = curves,
    rb_r = 0, rb_h = 0, rb_angle = 0;
    
    rb_texture_ratios.clear();
    for (size_t i = 0; i < rb_curves.size(); i++)
        rb_texture_ratios.push_back(1.0 / rb_curves.size());
    rb_init();
}

//?
void Rotation_body::rb_init() {
    for (auto c : rb_curves) {
        double r = std::max(std::abs(c.b3_Mi.x), std::abs(c.b3_Ma.x));
        rb_r = std::max(rb_r, r);
        rb_h = std::max(rb_h, c.b3_Ma.y);
        
        Cylinder* A =new Cylinder(Vector3(rb_center.x, rb_center.y, rb_center.z + c.b3_Mi.y),
                                  r, c.b3_Ma.y - c.b3_Mi.y);
        rb_sub_bounding_cy.push_back(A);
        rb_labels.push_back(Const::Rand_uint64());
    }
    rb_bounding_cy = new Cylinder(rb_center, rb_r, rb_h);
    
    double s = 0;
    rb_texture_ratios_sum.clear();
    for (auto r : rb_texture_ratios) {
        rb_texture_ratios_sum.push_back(s);
        s += r;
    }
}

//?
Collision Rotation_body::collide_ray(const Ray &ray) const {
    Ray normal_ray = ray.normal();
    Vector3 dir = normal_ray.ray_direction;
    int curve_id = 0;
    Vector2 res = Vector2(1e9, 0);
    
    Collision coll = rb_bounding_cy -> collide_ray(ray);
    if (!coll.is_intersecting())
        return Collision();
    
    for (size_t i = 0; i < rb_curves.size(); i++) {
        coll = rb_sub_bounding_cy[i] -> collide_ray(ray);
        if (!coll.is_intersecting() || (!coll.co_is_internal && coll.co_distance > res.x - Const::eps))
            continue;
        
        
        Vector3 r_c = ray.ray_start;
        Vector3 center_dir = r_c - rb_center;
        Vector2 dir2 = dir.to_Vector2(), center_dir2 = center_dir.to_Vector2();
        Vector2 q0, q1, q2, q3;
        
        // A.y^2 + B.y + C + D.x^2 = 0
        long double A = dir2.length2();
        long double B = 2 * dir2.dot(center_dir2) * dir.z - 2 * center_dir.z * A;
        long double C = Vector2(center_dir.z * dir.x - center_dir.x * dir.z,
                                center_dir.z * dir.y - center_dir.y * dir.z).length2();
        long double D = -dir.z * dir.z, a[7];
        
        rb_curves[i].get_para(q0, q1, q2, q3);
        a[0] = A * q0.y * q0.y + D * q0.x * q0.x + C + B * q0.y;
        a[1] = 2 * A * q0.y * q1.y + 2 * D * q0.x * q1.x + B * q1.y;
        a[2] = A * (q1.y * q1.y + 2 * q0.y * q2.y) + D * (q1.x * q1.x + 2 * q0.x * q2.x) + B * q2.y;
        a[3] = 2 * A * (q0.y * q3.y + q1.y * q2.y) + 2 * D * (q0.x * q3.x + q1.x * q2.x) + B * q3.y;
        a[4] = A * (2 * q1.y * q3.y + q2.y * q2.y) + D * (2 * q1.x * q3.x + q2.x * q2.x);
        a[5] = 2 * (A * q2.y * q3.y + D * q2.x * q3.x);
        a[6] = A * q3.y * q3.y + D * q3.x * q3.x;
        Polynomial6 poly(a);
        
        /*
        long double a[7];
        Vector3 r_c = ray.ray_start;
        Vector3 center_dir = rb_center - r_c;
        Vector2 dir2 = dir.to_Vector2(), center_dir2 = center_dir.to_Vector2();
        Vector2 q0, q1, q2, q3;
        rb_curves[i].get_para(q0, q1, q2, q3);
        
        a[0] = q0.x, a[1] = q1.x, a[2] = q2.x, a[3] = q3.x, a[4] = a[5] = a[6] = 0;
        Poly A(a);
        a[0] = q0.y, a[1] = q1.y, a[2] = q2.y, a[3] = q3.y, a[4] = a[5] = a[6] = 0;
        Poly B(a), C, D, E;
        B += rb_center.z - r_c.z;
        C = B, C *= dir2.x, C = -C, C += center_dir2.x * dir.z, C.square();
        D = B, D *= dir2.y, D = -D, D += center_dir2.y * dir.z, D.square();
        A.square();
        A *= dir.z * dir.z;
        E = C, E += D, E -= A;
        
        Polynomial6 poly(E.a);
         */
        
        std::vector<double> roots = poly.return_all_roots(0, 1);
        
        center_dir = -center_dir;
        for (auto u : roots) {
            double t = -1;
            Vector2 p = rb_curves[i].get_p(u);
            if (std::abs(dir.z) > Const::eps)
                t = (p.y + center_dir.z) / dir.z;
            else {
                double center_dot = center_dir2.dot(dir2);
                double a_2 = center_dot * center_dot + p.x * p.x - center_dir2.length2();
                if (a_2 > Const::eps) {
                    double a = std::sqrt(a_2);
                    double res_1 = center_dot - a, res_2 = center_dot + a;
                    if (res_1 > Const::eps)
                        t = res_1;
                    else if (res_2 > Const::eps)
                        t = res_2;
                }
            }
            if (t > 1e-4 && t < res.x - Const::eps)
                res = Vector2(t, u), curve_id = i;
        }
    }
    
    //res.print();
    
    if (res.x < 1e9) {
        Vector2 v = (normal_ray.get(res.x) - rb_center).to_Vector2().normal();
        Vector2 dp = rb_curves[curve_id].get_dp(res.y);
        //Vector3 n = Vector3(dp.y * v.x, dp.y * v.y, -dp.x);
        double theta = v.polar_angle();
        Vector3 n = Vector3(v.y, -v.x, 0) * Vector3(dp.x * cos(theta), dp.x * sin(theta), dp.y);
        if (n.dot(dir) < Const::eps)
            return Collision(normal_ray, res.x, curve_id + res.y, fmod(v.polar_angle(), 2 * Const::pi),
                             n, this, false, rb_labels[curve_id]);
        else
            return Collision(normal_ray, res.x, curve_id + res.y, fmod(v.polar_angle(), 2 * Const::pi),
                             -n, this, true, rb_labels[curve_id]);
    } else
        return Collision();
}

//?
Color Rotation_body::get_texture_color(const Collision &coll) const {
    if (o_material -> has_texture()) {
        int id = coll.co_u;
        double u = fmod(coll.co_v - rb_angle + 4 * Const::pi, Const::pi * 2) / 2 / Const::pi;
        double v = 1 - (rb_texture_ratios_sum[id] + (coll.co_u - id) * rb_texture_ratios[id]);
        return o_material -> get_texture_color(u, v);
    }
    return Color(1, 1, 1);
}

Rotation_body::Rotation_body(std::ifstream &fin) {
    int n;
    std::string s = "";
    fin >> n;
    Material* material;
    for (int i = 0; i < n; i++) {
        fin >> s;
        if (s == "o_material")
            material = Material::load_ifstream(fin);
        else if (s == "rb_center")
            rb_center = Vector3(fin);
        else if (s == "rb_curves")
            rb_curves = Bezier_curve3::load_ifstream(fin);
        else std::cerr << "Rotation_body error!" << std::endl;
    }
    (*this) = Rotation_body(rb_center, rb_curves, material);
}
