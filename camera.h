#ifndef Camera_h
#define Camera_h

#include "vector3.h"
#include "color.h"

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

class Bmp;

class Camera {
private:
    Vector3 c_eye, c_look_at, c_direction, c_up;
    int c_w, c_h;
    double c_field_angle;
    double c_aperture, c_focal_length;
    Vector3 c_dw, c_dh;
    Color** c_color;
    
    void c_init();
    
public:
    Camera(const Vector3 &eye, const Vector3 &look_at, const Vector3 &up, int w, int h,
           double field_angle, double aperture = 0, double focal_length = 0);
    Camera(std::ifstream &fin);
    
    int get_w() {return c_w;}
    int get_h() {return c_h;}
    Vector3 get_eye() {return c_eye;}
    
    bool bool_depth_of_field();
    
    //  x, y \in [0, 1]
    Ray emit(double x, double y);
    Ray depth_of_field_emit(double x, double y);
    
    Color get_color(int x, int y) {
        return 0 <= x && x < c_w && 0 <= y && y < c_h ? c_color[x][y] : Color();
    }
    
    void set_color(int x, int y, Color cc) {c_color[x][y] = cc;}
    std::vector<std::pair<int ,int > > detect_edge();
    void print(const std::string &file_path);
    Bmp* copy_picture();
    void set_bmp(const Bmp* picture);
};


#endif // Camera_h
