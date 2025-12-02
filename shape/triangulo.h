#ifndef TRIANGULO_H
#define TRIANGULO_H

#include "../struct/objeto.h"

class Triangulo : public Objeto {
public:
    Vec3 v0, v1, v2; // Vértices no espaço LOCAL
    Vec3 normal;     // Normal no espaço LOCAL (pré-calculada)

    // O construtor recebe os vértices como são no espaço local
    Triangulo(const Vec3& _v0, const Vec3& _v1, const Vec3& _v2, const Material& m);

    // Sobrescrevemos a função local
    virtual bool intersectaLocal(const Ray& r_local, double t_min, double t_max, HitRecord& rec) const override;
};

#endif