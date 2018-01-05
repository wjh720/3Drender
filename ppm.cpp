#include "config.h"
#include "const.h"
#include "photontracer.h"
#include "hitpointmap.h"
#include "ppm.h"
#include "camera.h"

#include <iostream>

#define For(i, x, y) for (int i = x; i < y; i++)

void PPM::run(const std::string &file_name) {
    if (!e_scene)
        return;
    ppm_map = new Hit_point_map();
    
    ppm_find_edge = true;
    RayTracer::run(file_name);
    ppm_find_edge = false;
    
    std::cout << "PPM!" << std::endl;
    For(i, 0, e_w)
    For(j, 0, e_h) {
        if (!j) {
            std::cout << "PPM column: " << i << "!" << std::endl;
            //ppm_map -> build();
        }
        tracer_pixel_x = i, tracer_pixel_y = j;
        if (tracer_is_edge[i][j])
            e_camera -> set_color(i, j, tracer_AA_sampling_color(i, j));
        else
            e_camera -> set_color(i, j, tracer_DOF_sampling_color(i, j));
    }
    
    ppm_map -> build();
    
    Bmp* figure = e_camera -> copy_picture();
    
    std::cout << "Start PPM!" << std::endl;
    PhotonTracer* tracer = new PhotonTracer(e_scene, ppm_map);
    
    //?
    int sum = 0;
    For(i, 0, Config::ppm_iteration_depth) {
        std::cout << "Round: " << i << "!" << std::endl;
        tracer -> emit_photons(Config::ppm_photon_emitted_number);
        ppm_map -> update_hit_point();
        sum += Config::ppm_photon_emitted_number;
        
        e_camera -> set_bmp(figure);
        for (auto k = ppm_map -> hit_point_map_points_begin(); k != ppm_map -> hit_point_map_points_end(); k++) {
            Color color_1 = k -> hit_point_color * k -> hit_point_flux * (1. / Const::pi / k -> hit_point_r2 / sum);

            int x = k -> hit_point_x, y = k -> hit_point_y;
            Color color_2 = e_camera -> get_color(x, y);
            
            Color color = color_1 + color_2;
            e_camera -> set_color(x, y, color);
        }
        e_camera -> print(file_name.c_str());
    }
}

//?
Color PPM::tracer_calc_local_illumination(const Collision &coll, const Material* material,
                                     const Color &factor) const {
    if (ppm_find_edge)
        return RayTracer::tracer_calc_local_illumination(coll, material, factor);
    
    Color color = material -> m_color * coll.co_object -> get_texture_color(coll) *
                    factor * material -> m_diffuse;
    Hit_point point = Hit_point(coll.co_p, coll.co_n, coll.co_ray.ray_direction, material,
                                tracer_pixel_x, tracer_pixel_y, color, Config::ppm_initial_search_radius);
    ppm_map -> add_hit_point(point);
    
    if (Config::photon_map_only)
        return Color();
    else
        return RayTracer::tracer_calc_local_illumination(coll, material, factor);
}
