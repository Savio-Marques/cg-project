#ifndef CILINDRO_H
#define CILINDRO_H

#include "../struct/objeto.h"

class Cilindro : public Objeto {
public:
    double raio;
    double altura;

    // Construtor simplificado:
    // Não pede mais base nem eixo. O cilindro local nasce na origem (0,0,0) subindo no eixo Y.
    Cilindro(double r, double h, const Material& m);
    
    // Função auxiliar interna para testar as tampas
    bool intersectaDisco(const Ray& r_local, double y_plano, const Vec3& normal, double& t_out) const;

    // Sobrescreve a função local
    virtual bool intersectaLocal(const Ray& r_local, double t_min, double t_max, HitRecord& rec) const override;
};

#endif