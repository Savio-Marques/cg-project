// struct/light.h
#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"

struct Light {
    Vec3 position;
    Vec3 intensity; // Cor e brilho da luz (ex: 0.7, 0.7, 0.7)
};

#endif