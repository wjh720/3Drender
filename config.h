#ifndef Config_h
#define Config_h

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
#include <string>

struct Config
{
    static void load_form(std::string &file);
    static double raytracing_min_weight;
    static int soft_shadow_samples;
    static int thread_max_number;
    static double anti_aliasing_edge_threshold;
    static int anti_aliasing_edge_detection_mode;
    static int anti_aliasing_samples;
    static int depth_of_field_samples;
    static bool enable_fresnel;
    static bool enable_texture_filtering;
    static int hightlight_exponent;
    static int newton_iteration_depth;
    static double output_refresh_interval;
    static bool photon_map_only;
    static int photon_emitted_number;
    static int photon_sampling_number;
    static int photon_tracing_max_depth;
    static double ppm_alpha;
    static double ppm_initial_search_radius;
    static int ppm_iteration_depth;
    static int ppm_photon_emitted_number;
    static int raytracing_max_depth;

private:
    Config();
};

#endif // Config_h
