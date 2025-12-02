#ifndef CONE_H
#define CONE_H

#include "../struct/objeto.h"

class Cone : public Objeto {
public:
    double raio;
    double altura;

    // Construtor simplificado (sem base/eixo)
    Cone(double r, double h, const Material& m);

    // Sobrescreve a função local
    virtual bool intersectaLocal(const Ray& r_local, double t_min, double t_max, HitRecord& rec) const override;
};

#endif