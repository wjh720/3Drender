#include "config.h"
#include "const.h"
#include "raytracer.h"
#include "camera.h"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <cmath>
#include <unistd.h>

#define For(i, x, y) for (int i = x; i < y; i++)

void Print(std::string s) {
    std::cerr << s << std::endl;
}

RayTracer::RayTracer(Scene* scene) : Engine(scene) {
    if (scene) {
        tracer_hash = new uint64*[e_w];
        tracer_is_edge = new bool*[e_w];
        For(i, 0, e_w) {
            tracer_hash[i] = new uint64[e_h];
            tracer_is_edge[i] = new bool[e_h];
        }
    }
}

void RayTracer::run(const std::string &file_name) {
    if (!e_scene) return;
    
    double t_1 = clock();
    
    std::cout << "Start RayTracing!" << std::endl;
    clock_t Last_refresh_time = clock();
    For(i, 0, e_w) {
        For(j, 0, e_h) {
            if (!j) std::cout << "Raytrace column " << i << "!" << std::endl;
            tracer_pixel_x = i, tracer_pixel_y = j;
            tracer_hash[i][j] = 0;
            e_camera -> set_color(i, j, tracer_DOF_sampling_color(i, j));
        }
        //sleep(1);
        
        if (Config::output_refresh_interval > 0 &&
            clock() - Last_refresh_time > Config::output_refresh_interval * CLOCKS_PER_SEC) {
            Last_refresh_time = clock();
            e_camera -> print(file_name);
        }
    }
    e_camera -> print(file_name);
    //std::cerr << "jb" << std::endl;
    std::cerr << (clock() - t_1) / CLOCKS_PER_SEC << std::endl;
    //sleep(1);
    
    if (Config::anti_aliasing_samples) {
        std::cout << "Smooth!" << std::endl;
        
        std::vector<std::pair<int ,int > > aa_list;
        switch (Config::anti_aliasing_edge_detection_mode) {
            case 0:     // 根据不同物体
                For(i, 0, e_w)
                For(j, 0, e_h) {
                    tracer_is_edge[i][j] = 0;
                    if (i + 1 < e_w && j + 1 < e_h)
                        if (tracer_hash[i][j] != tracer_hash[i + 1][j + 1] ||
                            tracer_hash[i + 1][j] != tracer_hash[i][j + 1]) {
                            aa_list.push_back(std::make_pair(i, j));
                            aa_list.push_back(std::make_pair(i + 1, j));
                            aa_list.push_back(std::make_pair(i, j + 1));
                            aa_list.push_back(std::make_pair(i + 1, j + 1));
                        }
                }
                sort(aa_list.begin(), aa_list.end());
                aa_list.erase(unique(aa_list.begin(), aa_list.end()), aa_list.end());
                break;
            case 1:     // 根据相邻像素色差
                aa_list = e_camera -> detect_edge();
                break;
            default:    // 全图采样
                For(i, 0, e_w)
                For(j, 0, e_h)
                    aa_list.push_back(std::make_pair(i, j));
                break;
        }
        
        Last_refresh_time = clock();
        for (size_t t = 0; t < aa_list.size(); t++) {
            int i = aa_list[t].first, j = aa_list[t].second;
            
            if (!t || i != aa_list[t - 1].first)
                std::cout << "Smooth column: " << i << "!" << std::endl;
            
            tracer_is_edge[i][j] = 1;
            e_camera -> set_color(i, j, tracer_AA_sampling_color(i, j));
            
            if (Config::output_refresh_interval > 0 &&
                clock() - Last_refresh_time > Config::output_refresh_interval * CLOCKS_PER_SEC) {
                Last_refresh_time = clock();
                e_camera -> print(file_name);
            }
        }
        e_camera -> print(file_name);
        std::cerr << (clock() - t_1) / CLOCKS_PER_SEC << std::endl;
        //sleep(1);
    }
}

Color RayTracer::tracer_AA_sampling_color(int ox, int oy) {
    tracer_pixel_x = ox, tracer_pixel_y = oy;
    if (!Config::anti_aliasing_samples)
        return tracer_DOF_sampling_color(ox, oy);
    
    std::vector<std::pair<double ,double > > points;
    int samples = Config::anti_aliasing_samples;
    
    
    For(i, 0, samples * 2)
    For(j, 0, samples * 2) {
        // 旋转网格采样
        double x = (i + 0.5) / samples - 1, y = (j + 0.5) / samples - 1;
        double angle = atan(0.5); //?
        double xx = cos(angle) * x - sin(angle) * y;
        double yy = sin(angle) * x + cos(angle) * y;
        if (std::abs(xx) < 0.5 && std::abs(yy) < 0.5)
            points.push_back(std::make_pair(ox + xx, oy + yy));
    }
    
    /*
    For(i, 0, samples * samples * 4) {
        double xx = Const::Rand_double() - 0.5, yy = Const::Rand_double() - 0.5;
        if (std::abs(xx) < 0.5 && std::abs(yy) < 0.5)
            points.push_back(std::make_pair(ox + xx, oy + yy));
    }
    */
    Color color;
    for (auto i : points) {
        color += tracer_DOF_sampling_color(i.first, i.second, 1. / points.size());
    }
    return color;
}

Color RayTracer::tracer_DOF_sampling_color(double ox, double oy, double factor) const {
    if (!e_camera -> bool_depth_of_field())
        return tracer_ray_tracing(e_camera -> emit(ox, oy), Color(1, 1, 1) * factor, 1, 1);

    Color color;
    For(i, 0, Config::depth_of_field_samples)
        color += tracer_ray_tracing(e_camera -> depth_of_field_emit(ox, oy),
                                    Color(1, 1, 1) * factor / Config::depth_of_field_samples, 1, 1);
    return color;
}

Color RayTracer::tracer_ray_tracing(const Ray &ray, const Color &factor, double weight, int depth) const {
    if (weight < Config::raytracing_min_weight || depth > Config::raytracing_max_depth)
        return Color();
    
    
    Collision coll = e_scene -> find_nearest_collision(ray);
    if (!coll.is_intersecting()) {
        return e_scene -> get_ambient_color() * factor;
    } else if (coll.intersect_with_light()) {
        (tracer_hash[tracer_pixel_x][tracer_pixel_y] *= Const::hash_p) += coll.co_object_label;
        return coll.co_light -> get_color() * factor;
    } else {
        (tracer_hash[tracer_pixel_x][tracer_pixel_y] *= Const::hash_p) += coll.co_object_label;
        
        Color res, absorb(1, 1, 1);
        const Object* obj = coll.co_object;
        const Material* material = obj -> get_material();
        
        // 透明材质的颜色过滤
        // 越远吸收的越多 law
        if (coll.co_is_internal)
            absorb = (material -> m_absorb_color * -coll.co_distance).exp();

        if (material -> m_diffuse > Const::eps || material -> m_spec > Const::eps)
            res += tracer_calc_local_illumination(coll, material, factor * absorb);

        // 反射, 透射
        if (material -> m_refl > Const::eps || material -> m_refr > Const::eps) {
            double refractivity = material -> m_refractivity;
            if (coll.co_is_internal) refractivity = 1. / refractivity;
            
            Vector3 refl = coll.co_ray.ray_direction.reflect(coll.co_n);
            Vector3 refr = coll.co_ray.ray_direction.refract(coll.co_n, refractivity);
            
            if (material -> m_refr < Const::eps)            // 全镜面反射
                res += tracer_ray_tracing(Ray(coll.co_p, refl),
                                          factor * absorb * (material -> m_color * material -> m_refl),
                                          weight * material -> m_refl, depth + 1);
            else if (refr.length2() < Const::eps) {         // 全反射
                double k = material -> m_refl + material -> m_refr;
                res += tracer_ray_tracing(Ray(coll.co_p, refl),
                                     factor * absorb * (material -> m_color * k),
                                     weight * k, depth + 1);
            } else if (material -> m_refl < Const::eps)     // 全透射
                res += tracer_ray_tracing(Ray(coll.co_p, refr),
                                          factor * absorb * material -> m_refr,
                                          weight * material -> m_refr, depth + 1);
            else {                                        //反射 + 透射
                double kl = material -> m_refl, kr = material -> m_refr;
                
                // Fresnel equations ?
                if (Config::enable_fresnel) {
                    double cosI = -coll.co_ray.ray_direction.dot(coll.co_n);
                    double cosT = sqrt(1 - (1 - cosI * cosI) / refractivity / refractivity);
                    double r1 = (cosI * refractivity - cosT) / (cosI * refractivity + cosT);
                    double r2 = (cosI - cosT * refractivity) / (cosI + cosT * refractivity);
                    kl = (r1 * r1 + r2 * r2) / 2, kr = 1 - kl;
                }
                
                if (kl > Const::eps)
                    res += tracer_ray_tracing(Ray(coll.co_p, refl),
                                              factor * absorb * (material -> m_color * kl),
                                              weight * kl, depth + 1);
                if (kr > Const::eps)
                    res += tracer_ray_tracing(Ray(coll.co_p, refr),
                                              factor * absorb * kr,
                                              weight * kr, depth + 1);
            }
        }
        
        return res;
    }
}

Color RayTracer::tracer_calc_local_illumination(const Collision &coll, const Material* material,
                                                const Color &factor) const {
    Color color = material -> m_color * coll.co_object -> get_texture_color(coll);
    Color res = color * e_scene -> get_ambient_color() * material -> m_diffuse;  // 环境光

    for (auto light = e_scene -> lights_begin(); light != e_scene -> lights_end(); light++) {
        Vector3 l = ((*light) -> get_source() - coll.co_p).normal();
        if (l.dot(coll.co_n) < Const::eps) continue;

        double shade = (*light) -> get_shadow_ratio(e_scene, coll.co_p);
        if (shade > Const::eps)
            res += color * (*light) -> get_color() *
                    material -> get_BRDF(l, coll.co_n, coll.co_ray.ray_direction) * shade;
    }
    return res * factor;
}

