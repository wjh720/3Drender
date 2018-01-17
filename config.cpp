#include "config.h"

#include <fstream>
#include <iostream>
#include <unistd.h>

double Config::anti_aliasing_edge_threshold = 0.01;

int Config::anti_aliasing_edge_detection_mode = 0;

int Config::anti_aliasing_samples = 4;

int Config::depth_of_field_samples = 64;

bool Config::enable_fresnel = true;

bool Config::enable_texture_filtering = true;

int Config::hightlight_exponent = 50;

bool Config::photon_map_only = true;

int Config::photon_emitted_number = 1000000;

int Config::photon_sampling_number = 1000;

int Config::photon_tracing_max_depth = 10;

double Config::ppm_alpha = 0.7;

double Config::ppm_initial_search_radius = 0.1;

int Config::ppm_iteration_depth = 20;

int Config::ppm_photon_emitted_number = 100000;

int Config::newton_iteration_depth = 20;

double Config::output_refresh_interval = 1;

int Config::raytracing_max_depth = 20;

double Config::raytracing_min_weight = 0.05;

int Config::soft_shadow_samples = 4;

int Config::thread_max_number = 4;

void Config::load_form(std::string &file) {
    std::ifstream fin(file.c_str());
    if (!fin)
        std::cerr << "No config file!" << std::endl;
    
    std::string s = "";
    for (;fin >> s;) {
        std::cerr << s << std::endl;
        if (s == "anti_aliasing_edge_threshold")
            fin >> anti_aliasing_edge_threshold;
        else if (s == "anti_aliasing_edge_detection_mode")
            fin >> anti_aliasing_edge_detection_mode;
        else if (s == "anti_aliasing_samples")
            fin >> anti_aliasing_samples;
        else if (s == "depth_of_field_samples")
            fin >> depth_of_field_samples;
        else if (s == "enable_fresnel")
            fin >> enable_fresnel;
        else if (s == "enable_texture_filtering")
            fin >> enable_texture_filtering;
        else if (s == "hightlight_exponent")
            fin >> hightlight_exponent;
        else if (s == "newton_iteration_depth")
            fin >> newton_iteration_depth;
        else if (s == "output_refresh_interval")
            fin >> output_refresh_interval;
        else if (s == "photon_map_only")
            fin >> photon_map_only;
        else if (s == "photon_emitted_number")
            fin >> photon_emitted_number;
        else if (s == "photon_sampling_number")
            fin >> photon_sampling_number;
        else if (s == "photon_tracing_max_depth")
            fin >> photon_tracing_max_depth;
        else if (s == "ppm_alpha")
            fin >> ppm_alpha;
        else if (s == "ppm_initial_search_radius")
            fin >> ppm_initial_search_radius;
        else if (s == "ppm_iteration_depth")
            fin >> ppm_iteration_depth;
        else if (s == "ppm_photon_emitted_number")
            fin >> ppm_photon_emitted_number;
        else if (s == "raytracing_max_depth")
            fin >> raytracing_max_depth;
        else if (s == "raytracing_min_weight")
            fin >> raytracing_min_weight;
        else if (s == "soft_shadow_samples")
            fin >> soft_shadow_samples;
        else if (s == "thread_max_number")
            fin >> thread_max_number;
        else if (s == "ppm_initial_search_radius")
            fin >> ppm_initial_search_radius;
        else std::cerr << "Config error!" << std::endl;
    }
    fin.close();
}
