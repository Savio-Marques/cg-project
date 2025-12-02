#include "cone.h"
#include <cmath>

Cone::Cone(double r, double h, const Material& m) 
    : raio(r), altura(h) { 
    mat = m; 
}

bool Cone::intersectaLocal(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    bool hit = false;
    double t_closest = t_max;

    // Constante K baseada na inclinação do cone (r/h)
    double k = raio / altura;
    double k2 = k * k; // k ao quadrado

    // --- 1. LATERAL DO CONE ---
    // Equação Algébrica: x² + z² = k² * (y - h)²
    // Isso gera um cone duplo (ampulheta) com vértice em (0, h, 0)
    
    // Coeficientes da Bhaskara (expandindo a equação com o Raio P = O + td)
    // Termos X e Z são positivos, termo Y (com k²) passa subtraindo
    double A = r.direcao.x*r.direcao.x + r.direcao.z*r.direcao.z - k2 * r.direcao.y*r.direcao.y;
    
    double B = 2.0 * (r.origem.x*r.direcao.x + r.origem.z*r.direcao.z - k2 * (r.origem.y - altura) * r.direcao.y);
    
    double C = r.origem.x*r.origem.x + r.origem.z*r.origem.z - k2 * (r.origem.y - altura)*(r.origem.y - altura);

    double delta = B*B - 4*A*C;

    if (delta >= 0) {
        double sqrtDelta = sqrt(delta);
        double t1 = (-B - sqrtDelta) / (2.0 * A);
        double t2 = (-B + sqrtDelta) / (2.0 * A);

        // Lambda para testar a altura (Corte do cone infinito)
        auto check_cone = [&](double t) {
            if (t > t_min && t < t_closest) {
                double y = r.origem.y + r.direcao.y * t;
                
                // O cone válido vai de Y=0 (base) até Y=altura (vértice)
                if (y >= 0 && y <= altura) {
                    t_closest = t;
                    hit = true;
                    rec.t = t;
                    rec.ponto = r.origem + r.direcao * t;
                    rec.mat = mat;

                    // Cálculo da Normal Lateral (Gradiente)
                    // A normal aponta para fora e inclina conforme a rampa
                    // Fórmula derivada do gradiente da superfície implícita
                    double y_diff = rec.ponto.y - altura;
                    rec.normal = Vec3{rec.ponto.x, -k2 * y_diff, rec.ponto.z}.normalize();
                }
            }
        };

        check_cone(t1);
        check_cone(t2);
    }

    // --- 2. BASE DO CONE (Disco em Y=0) ---
    // Igual ao cilindro, normal aponta para baixo (0, -1, 0)
    
    // O denominador é apenas a direção Y
    double denom = r.direcao.y;
    
    // Só testamos se não for paralelo
    if (std::abs(denom) > 1e-6) {
        // t = (0 - origem.y) / direcao.y
        double t_base = -r.origem.y / r.direcao.y;

        if (t_base > t_min && t_base < t_closest) {
            // Verifica se está dentro do raio
            Vec3 p = r.origem + r.direcao * t_base;
            double distSq = p.x*p.x + p.z*p.z;
            
            if (distSq <= raio * raio) {
                t_closest = t_base;
                hit = true;
                rec.t = t_closest;
                rec.ponto = p;
                rec.normal = Vec3{0, -1, 0}; // Para baixo
                rec.mat = mat;
            }
        }
    }

    return hit;
}