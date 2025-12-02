#ifndef CILINDRO_H
#define CILINDRO_H

#include "../struct/objeto.h"

class Cilindro : public Objeto {
public:
    Vec3 base;
    Vec3 eixo;
    double raio;
    double altura;

    Cilindro(const Vec3& b, const Vec3& axis, double r, double h, const Material& m);
    
    bool intersectaDisco(const Ray& r, const Vec3& centroDisco, const Vec3& normalDisco, double& t_out) const;

    virtual bool intersecta(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
};

#endif