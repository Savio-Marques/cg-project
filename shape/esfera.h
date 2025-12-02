#ifndef ESFERA_H
#define ESFERA_H

#include "../struct/objeto.h"

class Esfera : public Objeto {
public:

    double raio;

    Esfera(double r, const Material& m);
    virtual bool intersectaLocal(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
};

#endif