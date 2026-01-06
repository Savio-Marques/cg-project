#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"
#include <cmath>

// Define os tipos de luz
enum LightType {
    POINT,      // Lâmpada comum (tem posição, irradia para todo lado)
    DIRECTIONAL,// Sol (tem direção, raios paralelos, posição infinita)
    SPOT        // Lanterna (tem posição, direção e um cone de abertura)
};

struct Light {
    LightType type;     // O tipo da luz
    Vec3 position;      // Usado por POINT e SPOT
    Vec3 direction;     // Usado por DIRECTIONAL e SPOT (para onde aponta?)
    Vec3 intensity;     // Cor da luz
    
    // Para o SPOT: O cosseno do ângulo de abertura.
    // Ex: Se o ângulo é 30 graus, guardamos cos(30).
    double cutoff;      
};

#endif