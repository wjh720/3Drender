#include "light.h"
#include "collision.h"
#include "object.h"

#include <iostream>

Collision::Collision() {
    co_distance = 1e9;
}

Collision::Collision(const Ray& r, double t, const Vector3 &nn, const Object* ob, bool in_flag, uint64 label) {
    co_object = ob, co_light = nullptr, co_ray = r, co_p = r.get(t),
    co_n = nn.normal(), co_distance = t * r.ray_direction.length(),
    co_is_internal = in_flag, co_object_label = label ? label : ob -> get_label();
}

Collision::Collision(const Ray& r, double t, const double &uu, const double &vv,
          const Vector3 &nn, const Object* ob, bool in_flag, uint64 label) {
    co_object = ob, co_light = nullptr, co_ray = r, co_p = r.get(t),
    co_n = nn.normal(), co_distance = t * r.ray_direction.length(),
    co_u = uu, co_v = vv,
    co_is_internal = in_flag, co_object_label = label ? label : ob -> get_label();
}

Collision::Collision(const Ray& r, double t, const Light* l) {
    co_object = nullptr, co_light = l, co_ray = r, co_p = r.get(t),
    co_distance = t * r.ray_direction.length(),
    co_is_internal = false, co_object_label = l -> get_label();
}
