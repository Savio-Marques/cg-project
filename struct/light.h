#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"
#include <cmath>

// Define os tipos de luz
enum LightType {
    POINT,      // Lâmpada comum
    DIRECTIONAL,// Sol
    SPOT        // Lanterna
};

struct Light {
    LightType type;     // O tipo da luz
    Vec3 position;      // Usado por POINT e SPOT
    Vec3 direction;     // Usado por DIRECTIONAL e SPOT
    Vec3 intensity;     // Cor da luz
    
    double cutoff;      // Para o SPOT      
};

#endif