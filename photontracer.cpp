#include "config.h"
#include "const.h"
#include "photontracer.h"
#include "hitpointmap.h"
#include "light.h"
#include "material.h"
#include "object.h"
#include "scene.h"

#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <bitset>
#include <cmath>
#include <ctime>
#include <queue>
#include <set>
#include <map>

#include <mutex>
#include <thread>
#include <unistd.h>


void PhotonTracer::emit_photons(int number_photons) {
    double sum_power = 0;
    for (auto l = pt_scene -> lights_begin(); l != pt_scene -> lights_end(); l++)
        sum_power += (*l) -> get_power();
    
    std::mutex lock;
    int sum = 0, num_threads = std::max(Config::thread_max_number, 1);
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_threads; i++) {
        double del_power = sum_power / (1. * number_photons / num_threads);
        threads.push_back(std::thread([this, sum_power, del_power, &sum, &lock, i]() {
            srand(1ll * Const::Rand_uint() * (i + 1));
            for (auto l = pt_scene -> lights_begin(); l != pt_scene -> lights_end(); l++) {
                double light_power = (*l) -> get_power();
                for (;light_power > 0; light_power -= del_power) {
                    Photon photon = (*l) -> emit_photon(sum_power);
                    pt_photon_tracing(photon, 1);
                    
                    lock.lock();
                    if ((++sum) % 10000 == 0)
                        std::cout << sum << " photons now!" << std::endl;
                    lock.unlock();
                }
            }
        }));
    }
    
    for (int i = 0; i < num_threads; i++)
        threads[i].join();
}

void PhotonTracer::pt_photon_tracing(Photon &photon, int depth) {
    if (depth > Config::photon_tracing_max_depth)
        return;
    
    Collision coll = pt_scene -> find_nearest_collision(Ray(photon.p_start, photon.p_direction));
    
    if (coll.is_intersecting() && coll.intersect_with_object()) {
        photon.p_start = coll.co_p;
        const Object* obj = coll.co_object;
        const Material* material = obj -> get_material();
        
        if (material -> m_diffuse > Const::eps)
            pt_hit_point_map -> update_by_photon(photon);
        
        Color color = material -> m_color * coll.co_object -> get_texture_color(coll);
        Color absorb = Color(1, 1, 1);
        
        if (coll.co_is_internal) {
            Color asd = (material -> m_absorb_color * -coll.co_distance).exp();
            color *= asd, absorb *= asd;
        }
        
        double random_draw = Const::Rand_double();
        double prob_1 = (material -> m_diffuse + material -> m_spec) * color.power();
        double prob_2 = material -> m_refl * color.power();
        double prob_3 = material -> m_refr * absorb.power();
        
        if (random_draw < prob_1) {
            photon.p_direction = coll.co_n.diffuse();
            photon.p_power *= color / color.power();
            pt_photon_tracing(photon, depth + 1);
        } else if (random_draw < prob_1 + prob_2) {
            photon.p_direction = coll.co_ray.ray_direction.reflect(coll.co_n);
            photon.p_power *= color / color.power();
            pt_photon_tracing(photon, depth + 1);
        } else if (random_draw < prob_1 + prob_2 + prob_3) {
            double refractivity = material -> m_refractivity;
            if (coll.co_is_internal)
                refractivity = 1. / refractivity;
            photon.p_direction = coll.co_ray.ray_direction.refract(coll.co_n, refractivity);
            photon.p_power *= absorb / absorb.power();
            pt_photon_tracing(photon, depth + 1);
        }
    }
}
