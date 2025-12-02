#ifndef PLANO_H
#define PLANO_H

#include "../struct/objeto.h"

class Plano : public Objeto {
public:
    Vec3 ponto_plano;
    Vec3 normal;

    Plano(const Vec3& p, const Vec3& n, const Material& m);

    virtual bool intersecta(const Ray& raio, double t_min, double t_max, HitRecord& rec) const override;
};

#endif