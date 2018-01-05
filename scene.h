#ifndef Scene_h
#define Scene_h

#include "light.h"
#include "object.h"

#include <vector>

class Camera;

class Scene {
private:
    Camera* s_camera;       // 相机
    Color s_ambient_color;  // 环境光
    
    std::vector<Light*> s_lights;
    std::vector<Object*> s_objects;
    
    static std::string s_scene_file_dir;   // 场景文件路径
    
    void s_init();
    
public:
    Scene(Camera* camera, const Color& ambient = Color());
    Scene(std::ifstream &fin, std::string &config_file);
    
    Camera* get_camera() const {return s_camera;}
    Color get_ambient_color() const {return s_ambient_color;}
    Light* get_light(int i) const {return s_lights[i];}
    Object* get_object(int i) const {return s_objects[i];}
    
    void add_light(Light* light) {s_lights.push_back(light);}
    void add_object(Object* object) {s_objects.push_back(object);}
    
    std::vector<Light*>::const_iterator lights_begin() const {return s_lights.begin();}
    std::vector<Light*>::const_iterator lights_end() const {return s_lights.end();}
    std::vector<Object*>::const_iterator objects_begin() const {return s_objects.begin();}
    std::vector<Object*>::const_iterator objects_end() const {return s_objects.end();}
    
    // 寻找视线最先碰到的物体或光源
    Collision find_nearest_collision(const Ray &ray) const;
    
    // 从文件导入场景
    static Scene* load_form(std::string &file, std::string &config_file);
};


#endif // Scene_h
