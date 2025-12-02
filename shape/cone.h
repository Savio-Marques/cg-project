#ifndef CONE_H
#define CONE_H

#include "../struct/objeto.h"

class Cone : public Objeto {
public:
    Vec3 base;
    Vec3 eixo;
    double raio;
    double altura;

    Cone(const Vec3& b, const Vec3& axis, double r, double h, const Material& m);

    virtual bool intersecta(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
};

#endif