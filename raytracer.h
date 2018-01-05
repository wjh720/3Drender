#ifndef Raytracer_h
#define Raytracer_h

#include "ray.h"
#include "engine.h"

class RayTracer : public Engine {
protected:
    uint64** tracer_hash;                   // 光线经过物体的 HASH 值，优化 SSAA
    bool** tracer_is_edge;                  // 是否是物体边缘
    int tracer_pixel_x, tracer_pixel_y;     // 每次跟踪时像素的位置
    
    // 抗锯齿采样
    Color tracer_AA_sampling_color(int ox, int oy);
    
    // 景深采样
    Color tracer_DOF_sampling_color(double ox, double oy, double factor = 1) const;
    
    // 跟踪光线
    Color tracer_ray_tracing(const Ray &ray, const Color &factor, double weight, int depth) const;
    
    // 处理漫反射
    virtual Color tracer_calc_local_illumination(const Collision &coll, const Material* material,
                                                 const Color &factor) const;
    
public:
    RayTracer(Scene* scene);
    
    virtual void run(const std::string &file_name) override;
};

#endif // Raytracer_h
