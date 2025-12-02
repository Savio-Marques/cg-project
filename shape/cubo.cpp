#include "cubo.h"
#include <cmath>
#include <limits>
#include <algorithm> // Necessário para std::min e std::max

Cubo::Cubo(const Vec3& centroBase, double aresta, const Material& m) {
    mat = m;
    double meio = aresta / 2.0;
    // Base está em Y = centroBase.y. Topo está em Y = centroBase.y + aresta
    min_pt = {centroBase.x - meio, centroBase.y, centroBase.z - meio};
    max_pt = {centroBase.x + meio, centroBase.y + aresta, centroBase.z + meio};
}

bool Cubo::intersecta(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    // Algoritmo Slabs para AABB
    double t0 = t_min, t1 = t_max;
    
    // Eixo X
    double tx1 = (min_pt.x - r.origem.x) / r.direcao.x;
    double tx2 = (max_pt.x - r.origem.x) / r.direcao.x;
    
    // Tratamento de infinito para raio paralelo ao eixo
    if (std::isnan(tx1)) tx1 = -std::numeric_limits<double>::infinity(); 
    if (std::isnan(tx2)) tx2 = std::numeric_limits<double>::infinity();

    double tmin_curr = std::min(tx1, tx2);
    double tmax_curr = std::max(tx1, tx2);
    t0 = std::max(t0, tmin_curr);
    t1 = std::min(t1, tmax_curr);
    if (t0 > t1) return false;

    // Eixo Y
    double ty1 = (min_pt.y - r.origem.y) / r.direcao.y;
    double ty2 = (max_pt.y - r.origem.y) / r.direcao.y;
    
    tmin_curr = std::min(ty1, ty2);
    tmax_curr = std::max(ty1, ty2);
    t0 = std::max(t0, tmin_curr);
    t1 = std::min(t1, tmax_curr);
    if (t0 > t1) return false;

    // Eixo Z
    double tz1 = (min_pt.z - r.origem.z) / r.direcao.z;
    double tz2 = (max_pt.z - r.origem.z) / r.direcao.z;

    tmin_curr = std::min(tz1, tz2);
    tmax_curr = std::max(tz1, tz2);
    t0 = std::max(t0, tmin_curr);
    t1 = std::min(t1, tmax_curr);
    if (t0 > t1) return false;

    // Se chegou aqui, houve intersecção
    rec.t = t0; // t0 é a entrada
    rec.ponto = r.origem + r.direcao * rec.t;
    rec.mat = mat;

    // Calcular Normal (baseada em qual face foi atingida)
    double eps = 1e-4;
    if (std::abs(rec.ponto.x - min_pt.x) < eps) rec.normal = {-1, 0, 0};
    else if (std::abs(rec.ponto.x - max_pt.x) < eps) rec.normal = {1, 0, 0};
    else if (std::abs(rec.ponto.y - min_pt.y) < eps) rec.normal = {0, -1, 0};
    else if (std::abs(rec.ponto.y - max_pt.y) < eps) rec.normal = {0, 1, 0};
    else if (std::abs(rec.ponto.z - min_pt.z) < eps) rec.normal = {0, 0, -1};
    else if (std::abs(rec.ponto.z - max_pt.z) < eps) rec.normal = {0, 0, 1};
    else rec.normal = {0, 0, 1}; 

    return true;
}