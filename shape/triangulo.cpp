#include "triangulo.h"
#include <cmath>

Triangulo::Triangulo(const Vec3& _v0, const Vec3& _v1, const Vec3& _v2, const Material& m)
    : v0(_v0), v1(_v1), v2(_v2) {
    mat = m;
    
    // Calculamos a normal LOCAL uma vez só.
    // Como a deformação do objeto é feita via Matriz no "pai", 
    // essa normal local permanece válida para o algoritmo de intersecção.
    Vec3 aresta1 = v1 - v0;
    Vec3 aresta2 = v2 - v0;
    normal = aresta1.cross(aresta2).normalize();
}

bool Triangulo::intersectaLocal(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    const double EPSILON = 1e-6;
    Vec3 aresta1 = v1 - v0;
    Vec3 aresta2 = v2 - v0;
    
    // --- Algoritmo de Möller–Trumbore ---
    // (A matemática aqui usa os dados locais r, v0, v1, v2)

    Vec3 h = r.direcao.cross(aresta2);
    double a = aresta1.dot(h);

    if (a > -EPSILON && a < EPSILON) return false; // Raio paralelo

    double f = 1.0 / a;
    Vec3 s = r.origem - v0;
    
    double u = f * s.dot(h);
    if (u < 0.0 || u > 1.0) return false;

    Vec3 q = s.cross(aresta1);
    
    double v = f * r.direcao.dot(q);
    if (v < 0.0 || u + v > 1.0) return false;

    double t = f * aresta2.dot(q);

    if (t > t_min && t < t_max) {
        rec.t = t;
        
        // Ponto de intersecção no espaço LOCAL
        rec.ponto = r.origem + r.direcao * rec.t;
        
        // Normal no espaço LOCAL
        // (A classe Objeto vai transformar isso para normal do Mundo automaticamente depois)
        rec.normal = normal; 
        
        // Se quiser Back-face culling local (opcional):
        // if (rec.normal.dot(r.direcao) > 0) rec.normal = -rec.normal;

        // Nota: Se quiser textura no triângulo, aqui seria o lugar de interpolar os UVs
        // usando as coordenadas baricêntricas (u, v) que calculamos acima.
        
        rec.mat = mat;
        return true;
    }

    return false;
}