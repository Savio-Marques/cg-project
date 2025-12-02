#ifndef ILUMINACAO_H
#define ILUMINACAO_H

#include "../struct/vec3.h"
#include "../struct/ray.h"
#include "../struct/objeto.h"
#include "../struct/light.h"
#include <vector>

// Função que calcula a cor final de um pixel
Vec3 calcularIluminacao(
    const Ray& raioVisao, 
    const HitRecord& rec, 
    const std::vector<Objeto*>& cena, 
    const std::vector<Light>& luzes, 
    const Vec3& luzAmbiente,
    const Vec3& Ka, // Cor Ambiente (já processada se for textura)
    const Vec3& Kd, // Cor Difusa (já processada se for textura)
    const Vec3& Ke  // Cor Especular
);

#endif