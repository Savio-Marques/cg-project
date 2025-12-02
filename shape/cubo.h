#ifndef CUBO_H
#define CUBO_H

#include "../struct/objeto.h"

class Cubo : public Objeto {
public:
    Vec3 min_pt;
    Vec3 max_pt;

    // Construtor recebe centro da base e aresta
    Cubo(const Vec3& centroBase, double aresta, const Material& m);

    virtual bool intersecta(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
};

#endif