#include "plano.h"
#include <cmath>

Plano::Plano(const Material& m) { 
    mat = m; 
}

bool Plano::intersectaLocal(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    
    double denominador = r.direcao.y;

    // Se denominador for muito pequeno, raio é paralelo ao chão
    if (std::abs(denominador) < 1e-6) return false;

    // Fórmula: t = (ponto_plano - origem) . normal / denominador
    // ponto_plano é (0,0,0) e normal (0,1,0):
    // t = (0 - origem.y) / direcao.y
    double t = -r.origem.y / r.direcao.y;

    if (t < t_max && t > t_min) {
        rec.t = t;
        rec.ponto = r.origem + r.direcao * rec.t;
        
        // Normal padrão aponta pra cima
        rec.normal = Vec3{0.0, 1.0, 0.0};
        
        // Back-face culling
        // normal tem que apontar pra baixo pra ver o chão
        if (denominador > 0) {
            rec.normal = Vec3{0.0, -1.0, 0.0};
        }

        // Cálculo UV
        // plano local é infinito em X e Z, mapeia direto
        double escala = 0.003; 
        
        // u e v dependem apenas de X e Z locais
        rec.u = rec.ponto.x * escala;
        rec.v = rec.ponto.z * escala;
        
        rec.mat = mat;
        return true;
    }

    return false;
}