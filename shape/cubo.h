#ifndef CUBO_H
#define CUBO_H

#include "../struct/objeto.h"
#include "triangulo.h"
#include <vector>

class Cubo : public Objeto {
public:
    std::vector<Triangulo> triangulos;

    // O cubo será criado centralizado em (0,0,0)
    Cubo(double aresta, const Material& m);

    // Sobrescreve a função local
    virtual bool intersectaLocal(const Ray& r_local, double t_min, double t_max, HitRecord& rec) const override;

    std::string getTipo() const override { return "Cubo"; }
};

#endif