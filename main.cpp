#include "raytracer.h"
#include "ppm.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

void show_help()
{
    printf("Usage: ./render -e <engine> -s <SCENE_FILE> -o <OUTPUT_FILE> -config <CONFIG_FILE>\n"
           "Options:\n"
           "  --help                    Display this information.\n"
           "  -e <engine>               Select the rendering engine. Available engine list:\n"
           "                              0: Ray tracing (default)\n"
           "                              1: Progressive photon mapping\n");
    exit(0);
}

int main(int argc, char* argv[]) {
    int engine_id = 0;
    std::string scene_file = "", output_file = "", config_file = "";
    
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-e")) {
            if (i + 1 < argc && '0' <= argv[i + 1][0] && argv[i + 1][0] <= '1') {
                engine_id = argv[i + 1][0] - '0';
                i++;
            } else show_help();
        } else if (!strcmp(argv[i], "-s")) {
            if (i + 1 < argc) {
                scene_file = argv[i + 1];
                i++;
            } else show_help();
        } else if (!strcmp(argv[i], "-o")) {
            if (i + 1 < argc) {
                output_file = argv[i + 1];
                i++;
            } else show_help();
        } else if (!strcmp(argv[i], "-config")) {
            if (i + 1 < argc) {
                config_file = argv[i + 1];
                i++;
            } else show_help();
        } else if (!strcmp(argv[i], "--help"))
            show_help();
    }
    if (!scene_file.length() || !output_file.length() || !config_file.length())
        show_help();
    
    srand(time(0));
    Engine* engine;
    Scene* scene = Scene::load_form(scene_file, config_file);
    
    std::cout << "Scene file : " << scene_file << std::endl;
    std::cout << "output file : " << output_file << std::endl;
    
    switch (engine_id) {
        case 0:
            engine = new RayTracer(scene);
            break;
        case 1:
            engine = new PPM(scene);
            break;
    }
    engine -> run(output_file);
    
    return 0;
}
