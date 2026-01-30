#include "esfera.h"
#include <cmath>

Esfera::Esfera(double r, const Material& m) : raio(r) { 
    mat = m; 
}

bool Esfera::intersectaLocal(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    // A esfera está centrada na origem (0,0,0)
    // Então o vetor "Origem - Centro" é "Origem"
    Vec3 oc = r.origem;

    double a = r.direcao.dot(r.direcao);
    double b = 2.0 * oc.dot(r.direcao);
    double c_val = oc.dot(oc) - raio*raio;

    double discriminante = b*b - 4*a*c_val;

    if (discriminante > 0) {
        double sqrtDelta = sqrt(discriminante);

        // Raiz menor (entrada na esfera)
        double temp = (-b - sqrtDelta) / (2.0*a);
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.ponto = r.origem + r.direcao * rec.t;
            
            // (Ponto - 0,0,0) normalizado
            rec.normal = rec.ponto.normalize(); 
            rec.mat = mat;
            
            return true;
        }

        // Raiz maior (saída da esfera)
        temp = (-b + sqrtDelta) / (2.0*a);
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.ponto = r.origem + r.direcao * rec.t;
            rec.normal = rec.ponto.normalize();
            rec.mat = mat;
            return true;
        }
    }
    return false;
}