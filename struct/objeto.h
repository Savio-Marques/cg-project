#ifndef OBJETO_H
#define OBJETO_H

#include "ray.h"
#include "material.h"

// Classe que define um objeto genérico
class Objeto {
public:
    Material mat;
    virtual bool intersecta(const Ray& raio, double t_min, double t_max, HitRecord& rec) const = 0;
    virtual ~Objeto() {}
};

#endif