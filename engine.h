#ifndef Engine_h
#define Engine_h

#include "scene.h"

class RayTracer;
class PhotonTracer;

class Engine {
protected:
    Scene* e_scene;
    Camera* e_camera;
    int e_w, e_h;
    
    Engine(Scene* scene);
    
public:
    virtual void run(const std::string &file_name) = 0;
};

#endif // Engine_h
