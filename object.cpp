#include "object.h"
#include "plane.h"
#include "sphere.h"
#include "cylinder.h"
#include "rotationbody.h"

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
        else if (s == "Cylinder")
            return new Cylinder(fin);
        else if (s == "Rotation_body")
            return new Rotation_body(fin);
        else return nullptr;
    } else {
        std::cerr << "Light with no type!" << std::endl;
        return nullptr;
    }
}

Object::Object(const Material* m) {
    o_material = m == nullptr ? new Material() : m, o_label = Const::Rand_uint64();
}
