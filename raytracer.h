#ifndef Raytracer_h
#define Raytracer_h

#include "ray.h"
#include "engine.h"

class RayTracer : public Engine {
protected:
    uint64** tracer_hash;
    bool** tracer_is_edge;
    int tracer_pixel_x, tracer_pixel_y;
    
    Color tracer_AA_sampling_color(int ox, int oy);
    Color tracer_DOF_sampling_color(double ox, double oy, double factor = 1) const;
    Color tracer_ray_tracing(const Ray &ray, const Color &factor, double weight, int depth) const;
    virtual Color tracer_calc_local_illumination(const Collision &coll, const Material* material,
                                                 const Color &factor) const;
    
public:
    RayTracer(Scene* scene);
    
    virtual void run(const std::string &file_name) override;
};

#endif // Raytracer_h
