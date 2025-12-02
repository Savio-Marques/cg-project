#ifndef TRIANGULO_H
#define TRIANGULO_H

#include "../struct/objeto.h"

class Triangulo : public Objeto {
public:
    Vec3 v0, v1, v2; // Os 3 vértices
    Vec3 normal;     // Normal da face (calculada automaticamente)

    Triangulo(const Vec3& _v0, const Vec3& _v1, const Vec3& _v2, const Material& m);

    virtual bool intersecta(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
};

#endif