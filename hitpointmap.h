#ifndef Hitpointmap_h
#define Hitpointmap_h

#include "photon.h"
#include "material.h"

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

struct Hit_point {
    Color hit_point_color;
    double hit_point_r2;
    int hit_point_N, hit_point_M;
    Color hit_point_flux;
    Vector3 hit_point_position, hit_point_n, hit_point_direction;
    const Material* hit_point_material;
    int hit_point_x, hit_point_y;
    
    Hit_point() {hit_point_N = 0, hit_point_M = 0, hit_point_flux = Color();}
    Hit_point(const Vector3 &position, const Vector3 &n, const Vector3 &direction, const Material* material,
              int x, int y, const Color & color, double r2);
    
    void update(const Photon &photon) {
        hit_point_M++;
        if (photon.p_direction.dot(hit_point_n) < -Const::eps)
            hit_point_flux += photon.p_power * hit_point_material ->
                                get_BRDF(-photon.p_direction, hit_point_n, hit_point_direction);
    }
};

struct Hit_point_node {
    Hit_point* point;
    double x1, x2, y1, y2, z1, z2;
    
    Hit_point_node() {point = nullptr;}
    Hit_point_node(Hit_point* p) {
        point = p;
        double r = sqrt(p -> hit_point_r2);
        Vector3 position = p -> hit_point_position;
        x1 = position.x - r, x2 = position.x + r;
        y1 = position.y - r, y2 = position.y + r;
        z1 = position.z - r, z2 = position.z + r;
    }
    
    void update_son(Hit_point_node* A) {
        x1 = std::min(x1, A -> x1), x2 = std::max(x2, A -> x2);
        y1 = std::min(y1, A -> y1), y2 = std::max(y2, A -> y2);
        z1 = std::min(z1, A -> z1), z2 = std::max(z2, A -> z2);
    }
};

class Hit_point_map {
private:    
    int hit_point_map_t;
    Hit_point_node* hit_point_map_array;
    std::vector<Hit_point> hit_point_map_points;

    void build_kd_tree(int l, int r, int k);
    void rebuild_tree(int l, int r);
    void find_nearest_hit_points(int l, int r, int k, const Photon& photon);
    
public:
    Hit_point_map();
    
    void add_hit_point(const Hit_point &point) {hit_point_map_points.push_back(point);}
    void update_by_photon(const Photon &photon);
    void build();
    void update_hit_point();
    
    std::vector<Hit_point>::const_iterator hit_point_map_points_begin() const {
        return hit_point_map_points.begin();
    }
    std::vector<Hit_point>::const_iterator hit_point_map_points_end() const {
        return hit_point_map_points.end();
    }
};


#endif // Hitpointmap_h
