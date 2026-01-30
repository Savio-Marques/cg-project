#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include <cmath>
#include "struct/texture.h"

class Texture;

//Guarda as propriedades do material
struct Material {
    // Modelo phong de iluminação
    Vec3 Ka, Kd, Ke;
    // Brilho
    double shininess;
    
    bool useTexture = false;
    Texture* texturePtr = nullptr; 
};


// Guarda todos os dados do ponto de intersecção
struct HitRecord {
    double t;
    Vec3 ponto;
    Vec3 normal;
    Material mat;
    double u = 0; 
    double v = 0;
};
#endif