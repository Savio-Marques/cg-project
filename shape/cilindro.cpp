#include "cilindro.h"
#include <cmath>
#include <algorithm> // Para std::min/max se precisar

Cilindro::Cilindro(double r, double h, const Material& m) 
    : raio(r), altura(h) { 
    mat = m; 
}

// Helper para as tampas (Base e Topo)
bool Cilindro::intersectaDisco(const Ray& r, double y_plano, const Vec3& normal, double& t_out) const {
    // Intersecção com plano horizontal (Y constante)
    // O denominador é simplesmente a componente Y da direção (pois normal é 0,1,0 ou 0,-1,0)
    double denom = r.direcao.dot(normal);

    if (std::abs(denom) < 1e-6) return false; // Raio paralelo à tampa

    // t = (ponto_plano - origem) . normal / denom
    // Como o plano é horizontal, isso simplifica para:
    double t = (y_plano - r.origem.y) / r.direcao.y; // Se normal for (0,1,0)
    // Nota: O sinal se ajusta sozinho na divisão.

    if (t <= 0) return false; 
    
    // Verifica se bateu dentro do círculo (Raio 2D no plano XZ)
    Vec3 p = r.origem + r.direcao * t;
    double distQuadrada = p.x*p.x + p.z*p.z; // x² + z² <= r²

    if (distQuadrada <= raio * raio) {
        t_out = t;
        return true;
    }
    return false;
}

bool Cilindro::intersectaLocal(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    bool hit = false;
    double t_closest = t_max;

    // --- 1. CORPO DO CILINDRO (Lateral) ---
    // Equação do cilindro infinito no eixo Y: x² + z² = r²
    // (Ignoramos o Y na equação quadrática)
    double a = r.direcao.x * r.direcao.x + r.direcao.z * r.direcao.z;
    double b = 2.0 * (r.origem.x * r.direcao.x + r.origem.z * r.direcao.z);
    double c = r.origem.x * r.origem.x + r.origem.z * r.origem.z - raio * raio;

    double delta = b*b - 4*a*c;

    if (delta >= 0) {
        double sqrtDelta = sqrt(delta);
        double t1 = (-b - sqrtDelta) / (2.0 * a);
        double t2 = (-b + sqrtDelta) / (2.0 * a);

        // Lambda para testar a altura (Corte do cilindro finito)
        auto check_altura = [&](double t) {
            if (t > t_min && t < t_closest) {
                double y = r.origem.y + r.direcao.y * t;
                // O cilindro vai de Y=0 até Y=altura
                if (y >= 0 && y <= altura) {
                    t_closest = t;
                    hit = true;
                    rec.t = t;
                    rec.ponto = r.origem + r.direcao * t;
                    
                    // Normal na lateral: aponta para fora no plano XZ
                    rec.normal = Vec3{rec.ponto.x, 0, rec.ponto.z}.normalize();
                    rec.mat = mat;
                }
            }
        };

        check_altura(t1);
        check_altura(t2);
    }

    // --- 2. TAMPAS (Discos) ---
    double t_cap;

    // Base (Y = 0, Normal aponta para baixo 0,-1,0)
    if (intersectaDisco(r, 0.0, Vec3{0, -1, 0}, t_cap)) {
        if (t_cap > t_min && t_cap < t_closest) {
            t_closest = t_cap;
            hit = true;
            rec.t = t_cap;
            rec.ponto = r.origem + r.direcao * t_cap;
            rec.normal = Vec3{0, -1, 0};
            rec.mat = mat;
        }
    }

    // Topo (Y = altura, Normal aponta para cima 0,1,0)
    if (intersectaDisco(r, altura, Vec3{0, 1, 0}, t_cap)) {
        if (t_cap > t_min && t_cap < t_closest) {
            t_closest = t_cap;
            hit = true;
            rec.t = t_cap;
            rec.ponto = r.origem + r.direcao * t_cap;
            rec.normal = Vec3{0, 1, 0};
            rec.mat = mat;
        }
    }

    return hit;
}