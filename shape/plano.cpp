#include "plano.h"
#include <cmath>

Plano::Plano(const Material& m) { 
    mat = m; 
}

bool Plano::intersectaLocal(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    // Intersecção com o plano Y = 0 (Normal 0, 1, 0)
    
    // O denominador é o quanto o raio está subindo ou descendo (direcao.y)
    double denominador = r.direcao.y;

    // Se denominador for muito pequeno, raio é paralelo ao chão
    if (std::abs(denominador) < 1e-6) return false;

    // Fórmula: t = (ponto_plano - origem) . normal / denominador
    // Como ponto_plano é (0,0,0) e normal é (0,1,0):
    // t = (0 - origem.y) / direcao.y
    double t = -r.origem.y / r.direcao.y;

    if (t < t_max && t > t_min) {
        rec.t = t;
        rec.ponto = r.origem + r.direcao * rec.t;
        
        // Normal padrão aponta pra cima
        rec.normal = Vec3{0.0, 1.0, 0.0};
        
        // Back-face culling: Se o raio vem de baixo pra cima (denom > 0),
        // a normal tem que apontar pra baixo pra gente ver o chão
        if (denominador > 0) {
            rec.normal = Vec3{0.0, -1.0, 0.0};
        }

        // --- CÁLCULO DE UV (Textura) ---
        // Como o plano local é infinito em X e Z, mapeamos direto
        double escala = 0.003; 
        
        // u e v dependem apenas de X e Z locais
        rec.u = rec.ponto.x * escala;
        rec.v = rec.ponto.z * escala;
        
        rec.mat = mat;
        return true;
    }

    return false;
}