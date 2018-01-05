#include "config.h"
#include "const.h"
#include "hitpointmap.h"

#include <algorithm>
#include <iostream>

Hit_point::Hit_point(const Vector3 &position, const Vector3 &n, const Vector3 &direction, const Material* material,
                         int x, int y, const Color & color, double r2) {
    hit_point_position = position, hit_point_n = n, hit_point_direction = direction,
    hit_point_material = material, hit_point_x = x, hit_point_y = y,
    hit_point_color = color, hit_point_r2 = r2,
    hit_point_N = 0, hit_point_M = 0, hit_point_flux = Color();
}

Hit_point_map::Hit_point_map() {
    hit_point_map_t = 0, hit_point_map_array = nullptr;
}

void Hit_point_map::build_kd_tree(int l, int r, int k) {
    if (l > r)
        return;
    int mid = (l + r) / 2, kk = (k + 1) % 3;
    nth_element(hit_point_map_points.begin() + l, hit_point_map_points.begin() + mid,
                hit_point_map_points.begin() + r + 1, [&](const Hit_point &A, const Hit_point &B) {
                    return A.hit_point_position[k] < B.hit_point_position[k];
                });
    hit_point_map_array[mid] = Hit_point_node(&hit_point_map_points[mid]);
    if (l < mid) {
        build_kd_tree(l, mid - 1, kk);
        int ll = (l + mid - 1) / 2;
        hit_point_map_array[mid].update_son(&hit_point_map_array[ll]);
    }
    if (mid < r) {
        build_kd_tree(mid + 1, r, kk);
        int rr = (mid + 1 + r) / 2;
        hit_point_map_array[mid].update_son(&hit_point_map_array[rr]);
    }
}

void Hit_point_map::rebuild_tree(int l, int r) {
    if (l > r)
        return;
    int mid = (l + r) / 2;
    hit_point_map_array[mid] = Hit_point_node(&hit_point_map_points[mid]);
    if (l < mid) {
        rebuild_tree(l, mid - 1);
        int ll = (l + mid - 1) / 2;
        hit_point_map_array[mid].update_son(&hit_point_map_array[ll]);
    }
    if (mid < r) {
        rebuild_tree(mid + 1, r);
        int rr = (mid + 1 + r) / 2;
        hit_point_map_array[mid].update_son(&hit_point_map_array[rr]);
    }
}

inline bool judge(Vector3 &p, Hit_point_node &A) {
    return p.x < A.x1 || p.x > A.x2 || p.y < A.y1 || p.y > A.y2 || p.z < A.z1 || p.z > A.z2;
}

void Hit_point_map::find_nearest_hit_points(int l, int r, int k, const Photon& photon) {
    if (l > r)
        return;
    int mid = (l + r) / 2, kk = (k + 1) % 3;
    Vector3 p = photon.p_start;
    
    if (judge(p, hit_point_map_array[mid]))
        return;
    
    Hit_point* point = hit_point_map_array[mid].point;
    if ((p - point -> hit_point_position).length2() <= point -> hit_point_r2)
        point -> update(photon);
    
    if (p[k] < point -> hit_point_position[k]) {
        find_nearest_hit_points(l, mid - 1, kk, photon);
        find_nearest_hit_points(mid + 1, r, kk, photon);
    } else {
        find_nearest_hit_points(mid + 1, r, kk, photon);
        find_nearest_hit_points(l, mid - 1, kk, photon);
    }
}

void Hit_point_map::update_by_photon(const Photon &photon) {
    find_nearest_hit_points(0, hit_point_map_t - 1, 0, photon);
}

void Hit_point_map::build() {
    hit_point_map_t = hit_point_map_points.size();
    hit_point_map_array = new Hit_point_node[hit_point_map_t * 2];
    
    std::cout << "Total kd points : " << hit_point_map_t << std::endl;
    build_kd_tree(0, hit_point_map_t - 1, 0);
}

void Hit_point_map::update_hit_point() {
    for (auto &i : hit_point_map_points) {
        //?
        if (i.hit_point_M) {
            double sum = i.hit_point_N + i.hit_point_M * Config::ppm_alpha;
            double k = sum / (i.hit_point_N + i.hit_point_M);
            i.hit_point_r2 *= k, i.hit_point_flux *= k;
            i.hit_point_N = sum, i.hit_point_M = 0;
        }
    }
    rebuild_tree(0, hit_point_map_t - 1);
}
