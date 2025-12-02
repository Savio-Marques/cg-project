#include "esfera.h"
#include <cmath>

// Construtor: Apenas guarda raio e material
Esfera::Esfera(double r, const Material& m) : raio(r) { 
    mat = m; 
}

bool Esfera::intersectaLocal(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    // No espaço local, a esfera está sempre centrada na origem (0,0,0)
    // Então o vetor "Origem - Centro" é apenas "Origem"
    Vec3 oc = r.origem; // Equivale a: r.origem - Vec3(0,0,0)

    double a = r.direcao.dot(r.direcao);
    double b = 2.0 * oc.dot(r.direcao);
    double c_val = oc.dot(oc) - raio*raio;

    double discriminante = b*b - 4*a*c_val;

    if (discriminante > 0) {
        double sqrtDelta = sqrt(discriminante);

        // Tentativa 1: Raiz menor (entrada na esfera)
        double temp = (-b - sqrtDelta) / (2.0*a);
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.ponto = r.origem + r.direcao * rec.t;
            
            // Normal Local: (Ponto - 0,0,0) normalizado
            rec.normal = rec.ponto.normalize(); 
            rec.mat = mat;
            
            // Se precisar de textura esférica no futuro, o cálculo de UV seria aqui
            return true;
        }

        // Tentativa 2: Raiz maior (saída da esfera)
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