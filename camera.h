#ifndef Camera_h
#define Camera_h

#include "vector3.h"
#include "color.h"

#include <vector>

class Bmp;

class Camera {
private:
    Vector3 c_eye, c_look_at, c_direction, c_up;    // 相机位置、相机视线上任意一点、视线方向、上方向
    int c_w, c_h;                                   // 分辨率 c_w * c_h
    double c_field_angle;                           // 相机视野张角 相机到胶片距离为1
    double c_aperture, c_focal_length;              // 光圈大小，焦距(默认为 m_eye 和 m_look_at 的距离)
    Vector3 c_dw, c_dh;                             // 视平面坐标系方向
    Color** c_color;                                // 颜色缓存
    
    void c_init();
    
public:
    Camera(const Vector3 &eye, const Vector3 &look_at, const Vector3 &up, int w, int h,
           double field_angle, double aperture = 0, double focal_length = 0);
    Camera(std::ifstream &fin);
    
    int get_w() {return c_w;}
    int get_h() {return c_h;}
    Vector3 get_eye() {return c_eye;}
    
    bool bool_depth_of_field();
    
    // 像素点对应的光线方向 x, y \in [0, 1]
    Ray emit(double x, double y);
    
    // 处理景深时的发射光线
    Ray depth_of_field_emit(double x, double y);
    
    Color get_color(int x, int y) {
        return 0 <= x && x < c_w && 0 <= y && y < c_h ? c_color[x][y] : Color();
    }
    
    // 置像素点 (x, y) 的颜色为 color
    void set_color(int x, int y, Color cc) {c_color[x][y] = cc;}
    
    // 边缘检测
    std::vector<std::pair<int ,int > > detect_edge();
    
    // 打印照片到 file 文件
    void print(const std::string &file_path);
    
    // 复制底片
    Bmp* copy_picture();
    
    // 重置底片为 BMP 文件
    void set_bmp(const Bmp* picture);
};


#endif // Camera_h
