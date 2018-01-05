#include "bmp.h"
#include "config.h"
#include "const.h"
#include "ray.h"
#include "camera.h"

#include <algorithm>
#include <iostream>
#include <unistd.h>

void Camera::c_init() {
    c_dw = (c_direction * c_up).normal() * tan(c_field_angle / 2) * (1. * c_w / c_h);
    c_dh = (c_direction * c_dw).normal() * tan(c_field_angle / 2);
    c_color = new Color*[c_w];
    for (int i = 0; i < c_w; i++) c_color[i] = new Color[c_h];
}

Camera::Camera(const Vector3 &eye, const Vector3 &look_at, const Vector3 &up, int w, int h,
               double field_angle, double aperture, double focal_length) {
    c_eye = eye, c_look_at = look_at, c_direction = (look_at - eye).normal(),
    c_up = up, c_w = w, c_h = h,
    c_field_angle = field_angle * Const::pi / 180,
    c_aperture = aperture, c_focal_length = (look_at - eye).length();
    if (focal_length > Const::eps) c_focal_length = focal_length;
    c_init();
}

Camera::Camera(std::ifstream &fin) {
    int n;
    std::string s = "";
    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> s;
        if (s == "c_aperture")
            fin >> c_aperture;
        else if (s == "c_eye")
            c_eye = Vector3(fin);
        else if (s == "c_focal_length")
            fin >> c_focal_length;
        else if (s == "c_field_angle")
            fin >> c_field_angle;
        else if (s == "c_h")
            fin >> c_h;
        else if (s == "c_look_at")
            c_look_at = Vector3(fin);
        else if (s == "c_up")
            c_up = Vector3(fin);
        else if (s == "c_w")
            fin >> c_w;
        else std::cerr << "Camera error!" << std::endl;
    }
    *this = Camera(c_eye, c_look_at, c_up, c_w, c_h, c_field_angle, c_aperture, c_focal_length);
}

bool Camera::bool_depth_of_field() {
    return c_aperture > Const::eps && Config::depth_of_field_samples > 0;
}

Ray Camera::emit(double x, double y) {
    return Ray(c_eye, c_direction + c_dw * (2. * x / c_w - 1) + c_dh * (2. * y / c_h - 1));
}

Ray Camera::depth_of_field_emit(double x, double y) {
    Vector3 focal_point = c_eye + emit(x, y).ray_direction.normal() * c_focal_length;
    double dx, dy;
    for (;;) {
        dx = Const::Rand_double() * 2 - 1;
        dy = Const::Rand_double() * 2 - 1;
        if (dx * dx + dy * dy <= 1) break;
    }
    Vector3 start = c_eye + c_dw.normal() * c_aperture * dx + c_dh.normal() * c_aperture * dy;
    return Ray(start, focal_point - start);
}

std::vector<std::pair<int ,int > > Camera::detect_edge() {
    std::vector<std::pair<int ,int > > edge_list;
    for (int i = 0; i < c_w; i++)
        for (int j = 0; j < c_h; j++) {
            // Roberts cross operator
            Color gx = get_color(i, j) - get_color(i + 1, j + 1);
            Color gy = get_color(i + 1, j) - get_color(i, j + 1);
            double Energy = gx.length2() + gy.length2();
            if (Energy > Config::anti_aliasing_edge_threshold) {
                edge_list.push_back(std::make_pair(i, j));
                if (i + 1 < c_w) edge_list.push_back(std::make_pair(i + 1, j));
                if (j + 1 < c_h) edge_list.push_back(std::make_pair(i, j + 1));
                if (i + 1 < c_w && j + 1 < c_h) edge_list.push_back(std::make_pair(i + 1, j + 1));
            }
        }
    sort(edge_list.begin(), edge_list.end());
    auto iter = unique(edge_list.begin(), edge_list.end());
    edge_list.erase(iter, edge_list.end());
    return edge_list;
}

void Camera::print(const std::string &file_path) {
    Bmp* picture = new Bmp(c_w, c_h);
    for (int i = 0; i < c_w; i++)
        for (int j = 0; j < c_h; j++)
            picture -> setColor(i, j, c_color[i][j].confine());
    picture -> save(file_path);
    delete picture;
}

Bmp* Camera::copy_picture() {
    Bmp* picture = new Bmp(c_w, c_h);
    for (int i = 0; i < c_w; i++)
        for (int j = 0; j < c_h; j++)
            picture -> setColor(i, j, get_color(i, j));
    return picture;
}

void Camera::set_bmp(const Bmp* picture) {
    for (int i = 0; i < c_w; i++)
        for (int j = 0; j < c_h; j++)
            set_color(i, j, picture -> getColor(i, j));
}
