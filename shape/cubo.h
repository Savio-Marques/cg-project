#ifndef CUBO_H
#define CUBO_H

#include "../struct/objeto.h"
#include "triangulo.h"
#include <vector>

class Cubo : public Objeto {
public:
    // Lista de triângulos que compõem este cubo
    std::vector<Triangulo> triangulos;

    Cubo(const Vec3& centroBase, double aresta, const Material& m);

    virtual bool intersecta(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
};

#endif