#include "light.h"
#include "rectlight.h"
#include "pointlight.h"

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

Light* Light::load_ifstream(std::ifstream &fin) {
    std::string s = "", string_type = "";
    fin >> s;
    fin >> s;
    std::cerr << "Light name : " << s << std::endl;
    fin >> s;
    if (s == "type") {
        fin >> s;
        if (s == "Rectlight")
            return new Rectlight(fin);
        else if (s == "Pointlight")
            return new Pointlight(fin);
        else return nullptr;
    } else {
        std::cerr << "Light with no type!" << std::endl;
        return nullptr;
    }
}

Light::Light(const Color &color, double power) {
    l_color = color, l_power = power, l_label = Const::Rand_uint64();
}


