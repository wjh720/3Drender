#ifndef Ppm_h
#define Ppm_h

#include "raytracer.h"

class Hit_point_map;

class PPM : public RayTracer {
private:
    Hit_point_map* ppm_map;
    bool ppm_find_edge; 
    
    virtual Color tracer_calc_local_illumination(const Collision &coll, const Material* material,
                                                 const Color &factor) const override;
    
public:
    PPM(Scene* scene) : RayTracer(scene) {}
    virtual void run(const std::string &file_name) override;
};

#endif // Ppm_h
