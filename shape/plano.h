#ifndef PLANO_H
#define PLANO_H

#include "../struct/objeto.h"

class Plano : public Objeto {
public:
    // Construtor
    Plano(const Material& m);

    virtual bool intersectaLocal(const Ray& r_local, double t_min, double t_max, HitRecord& rec) const override;

    std::string getTipo() const override { return "Plano"; }
};

#endif