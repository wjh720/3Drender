#ifndef Photontracer_h
#define Photontracer_h

struct Photon;
class Scene;
class Hit_point_map;

class PhotonTracer {
private:
    Hit_point_map* pt_hit_point_map;
    Scene* pt_scene;
    
    void pt_photon_tracing(Photon &photon, int depth);
    
public:
    // 用于 PPM
    PhotonTracer(Scene* scene, Hit_point_map* map) {pt_hit_point_map = map, pt_scene = scene;}
    
    // 发射光子
    void emit_photons(int number_photons);
};


#endif // Photontracer_h
