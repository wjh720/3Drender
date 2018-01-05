#include "object.h"
#include "plane.h"
#include "sphere.h"

#include <iostream>

Object::Object(const Material* m) {
    o_material = m == nullptr ? new Material() : m, o_label = Const::Rand_uint64();
}

Object* Object::load_ifstream(std::ifstream &fin) {
    std::string s = "", string_type = "";
    fin >> s;
    fin >> s;
    std::cerr << "Object name : " << s << std::endl;
    fin >> s;
    if (s == "type") {
        fin >> s;
        if (s == "Plane")
            return new Plane(fin);
        else if (s == "Sphere")
            return new Sphere(fin);
        else return nullptr;
    } else {
        std::cerr << "Light with no type!" << std::endl;
        return nullptr;
    }
}
