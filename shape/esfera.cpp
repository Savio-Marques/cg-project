#include "esfera.h"

Esfera::Esfera(const Vec3& c, double r, const Material& m) : centro(c), raio(r) { 
    mat = m; 
}

// Equação de intersecção da esfera
bool Esfera::intersecta(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    Vec3 oc = r.origem - centro;

    double a = r.direcao.dot(r.direcao);
    double b = 2.0 * oc.dot(r.direcao);
    double c_val = oc.dot(oc) - raio*raio;

    // Calcula a raiz para saber os pontos de acerto
    double discriminante = b*b - 4*a*c_val;
    if (discriminante > 0) {
        double temp = (-b - sqrt(discriminante)) / (2.0*a);
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.ponto = r.origem + r.direcao * rec.t;
            rec.normal = (rec.ponto - centro).normalize();
            rec.mat = mat;
            return true;
        }

        temp = (-b + sqrt(discriminante)) / (2.0*a);
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.ponto = r.origem + r.direcao * rec.t;
            rec.normal = (rec.ponto - centro).normalize();
            rec.mat = mat;
            return true;
        }
    }
    return false;
}