#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include <cmath>

//Guarda as propriedades do material
struct Material {
    // Modelo phong de iluminação
    Vec3 Ka, Kd, Ke;
    // "Dureza" do brilho
    double shininess;
    
    bool useTexture = false;
    void* texturePtr = nullptr; 
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