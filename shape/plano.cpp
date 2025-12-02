#include "plano.h"
#include <cmath>

Plano::Plano(const Vec3& p, const Vec3& n, const Material& m) 
    : ponto_plano(p), normal(n.normalize()) { 
    mat = m; 
}

// Retorna true caso tenha intersecção com o plano
bool Plano::intersecta(const Ray& raio, double t_min, double t_max, HitRecord& rec) const {
    double denominador = normal.dot(raio.direcao);
    if (std::abs(denominador) > 1e-6) {
        double t = (ponto_plano - raio.origem).dot(normal) / denominador;
        if (t < t_max && t > t_min) {
            rec.t = t;
            rec.ponto = raio.origem + raio.direcao * rec.t;
            rec.normal = normal;
            
            // Back-face culling
            if (denominador > 0) rec.normal = -normal; 

            // Define quantas vezes a textura se repete
            double escala = 0.003; // Ajustar o valor (0.01 = grande, 0.1 = pequeno)
            
            rec.u = rec.ponto.x * escala;
            rec.v = rec.ponto.z * escala;
            // ---------------------------

            rec.mat = mat;
            return true;
        }
    }
    return false;
}