#include "config.h"
#include "const.h"
#include "rectlight.h"
#include "object.h"
#include "plane.h"
#include "sphere.h"
#include "camera.h"
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
#include <unistd.h>

Scene::Scene(std::ifstream &fin, std::string &config_file) {
    std::string s = "";
    Config::load_form(config_file);
    for (;fin >> s;) {
        if (s[0] == '-')
            continue;
        std::cerr << s << std::endl;
        //       std::cerr << CLOCKS_PER_SEC << std::endl;
        //        sleep(0.2);
        if (s == "ambient_color")
            s_ambient_color = Color(fin);
        else if (s == "camera")
            s_camera = new Camera(fin);
        else if (s == "lights") {
            int n;
            fin >> n;
            for (int i = 0; i < n; i++) {
                Light* light = Light::load_ifstream(fin);
                if (light) s_lights.push_back(light);
            }
        } else if (s == "objects") {
            int n;
            fin >> n;
            for (int i = 0; i < n; i++) {
                Object* object = Object::load_ifstream(fin);
                if (object) s_objects.push_back(object);
            }
        } else std::cerr << " Scene error!" << std::endl;
    }
    sleep(1);
}

std::string Scene::s_scene_file_dir = "";

inline bool cc1(const Object* A, const Object* B) {
    return A -> get_material() -> compare(B -> get_material());
}

void Scene::s_init() {
    sort(s_objects.begin(), s_objects.end(), cc1);
}

Scene::Scene(Camera* camera, const Color& ambient) {
    s_camera = camera, s_ambient_color = ambient;
    s_init();
}

Collision Scene::find_nearest_collision(const Ray &ray) const {
    Collision colli;
    for (auto i : s_lights) {
        Collision co = i -> collide(ray);
        if (co.is_intersecting() && co.co_distance + Const::eps < colli.co_distance)
            colli = co;
    }
    for (auto i : s_objects) {
        Collision co = i -> collide_ray(ray);
        if (co.is_intersecting() && co.co_distance + Const::eps < colli.co_distance)
            colli = co;
    }
    return colli;
}

Scene* Scene::load_form(std::string &file, std::string &config_file) {
    std::ifstream fin(file.c_str());
    if (!fin) {
        std::cerr << "No scene file!" << std::endl;
        return nullptr;
    }
    s_scene_file_dir = file.substr(0, file.find_last_of('/'));
    Scene* scene = new Scene(fin, config_file);
    fin.close();
    return scene;
}
