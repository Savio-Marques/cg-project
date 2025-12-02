#include "triangulo.h"
#include <cmath>

Triangulo::Triangulo(const Vec3& _v0, const Vec3& _v1, const Vec3& _v2, const Material& m)
    : v0(_v0), v1(_v1), v2(_v2) {
    mat = m;
    
    // Passo A: Calcular a normal automaticamente usando Cross Product
    // Aresta 1: vai do v0 para v1
    // Aresta 2: vai do v0 para v2
    Vec3 aresta1 = v1 - v0;
    Vec3 aresta2 = v2 - v0;
    normal = aresta1.cross(aresta2).normalize();
}

bool Triangulo::intersecta(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    const double EPSILON = 1e-6;
    Vec3 aresta1 = v1 - v0;
    Vec3 aresta2 = v2 - v0;
    
    // Calcula o vetor P (Produto vetorial da direção do raio com aresta2)
    Vec3 h = r.direcao.cross(aresta2);
    double a = aresta1.dot(h);

    // Se 'a' for muito próximo de 0, o raio é paralelo ao triângulo
    if (a > -EPSILON && a < EPSILON) return false;

    double f = 1.0 / a;
    Vec3 s = r.origem - v0;
    
    // Calcula coordenada baricêntrica U
    double u = f * s.dot(h);
    if (u < 0.0 || u > 1.0) return false;

    Vec3 q = s.cross(aresta1);
    
    // Calcula coordenada baricêntrica V
    double v = f * r.direcao.dot(q);
    if (v < 0.0 || u + v > 1.0) return false;

    // Calcula T (distância) onde ocorreu a intersecção
    double t = f * aresta2.dot(q);

    if (t > t_min && t < t_max) {
        rec.t = t;
        rec.ponto = r.origem + r.direcao * rec.t;
        rec.normal = normal; // A normal é constante em todo o triângulo plano
        
        // Se quiser Back-face culling (opcional):
        // if (rec.normal.dot(r.direcao) > 0) rec.normal = -rec.normal;

        rec.mat = mat;
        return true;
    }

    return false;
}