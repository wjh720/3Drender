#include "engine.h"
#include "camera.h"

Engine::Engine(Scene* scene) {
    e_scene = scene, e_camera = nullptr, e_w = 0, e_h = 0;
    if (scene) {
        e_camera = e_scene -> get_camera();
        e_w = e_camera -> get_w();
        e_h = e_camera -> get_h();
    }
}
