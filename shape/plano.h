#ifndef PLANO_H
#define PLANO_H

#include "../struct/objeto.h"

class Plano : public Objeto {
public:
    // Construtor simples (só precisa do material)
    // O plano é sempre o plano XZ (y=0) no espaço local
    Plano(const Material& m);

    virtual bool intersectaLocal(const Ray& r_local, double t_min, double t_max, HitRecord& rec) const override;
};

#endif