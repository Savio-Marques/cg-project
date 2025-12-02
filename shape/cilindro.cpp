#include "cilindro.h"
#include <cmath>

Cilindro::Cilindro(const Vec3& b, const Vec3& axis, double r, double h, const Material& m) 
    : base(b), eixo(axis.normalize()), raio(r), altura(h) { 
    mat = m; 
}

bool Cilindro::intersectaDisco(const Ray& r, const Vec3& centroDisco, const Vec3& normalDisco, double& t_out) const {
    double denom = normalDisco.dot(r.direcao);
    if (std::abs(denom) < 1e-6) return false;
    double t = (centroDisco - r.origem).dot(normalDisco) / denom;
    if (t <= 0) return false;
    
    Vec3 p = r.origem + r.direcao * t;
    if ((p - centroDisco).norm() <= raio) {
        t_out = t;
        return true;
    }
    return false;
}

bool Cilindro::intersecta(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    bool hit = false;
        double t_closest = t_max;

        Vec3 oc = r.origem - base;

        double rd_dot_a = r.direcao.dot(eixo);

        Vec3 d_perp = r.direcao - eixo * rd_dot_a;

        double oc_dot_a = oc.dot(eixo);

        Vec3 oc_perp = oc - eixo * oc_dot_a;

        double A = d_perp.dot(d_perp);
        double B = 2 * d_perp.dot(oc_perp);
        double C = oc_perp.dot(oc_perp) - raio * raio;
        double delta = B*B - 4*A*C;

        if (delta >= 0) {
            double sqrtDelta = sqrt(delta);
            double t1 = (-B - sqrtDelta) / (2 * A);
            double t2 = (-B + sqrtDelta) / (2 * A);

            if (t1 > t_min && t1 < t_closest) {
                double h = (r.origem + r.direcao * t1 - base).dot(eixo);

                if (h >= 0 && h <= altura) {
                    t_closest = t1;
                    hit = true;
                    Vec3 p = r.origem + r.direcao * t1;
                    Vec3 v = p - base;
                    double proj = v.dot(eixo);
                    rec.normal = (v - eixo * proj).normalize();
                }
            }

            if (t2 > t_min && t2 < t_closest) {
                double h = (r.origem + r.direcao * t2 - base).dot(eixo);
                if (h >= 0 && h <= altura) {
                    t_closest = t2;
                    hit = true;
                    Vec3 p = r.origem + r.direcao * t2;
                    Vec3 v = p - base;
                    double proj = v.dot(eixo);
                    rec.normal = (v - eixo * proj).normalize();
                }
            }
        }

        double t_base;

        if (intersectaDisco(r, base, -eixo, t_base)) {
            if (t_base > t_min && t_base < t_closest) {
                t_closest = t_base;
                rec.normal = -eixo;
                hit = true;
            }
        }

        Vec3 topo = base + eixo * altura;

        double t_topo;
        if (intersectaDisco(r, topo, eixo, t_topo)) {
            if (t_topo > t_min && t_topo < t_closest) {
                t_closest = t_topo;
                rec.normal = eixo;
                hit = true;
            }
        }

        if (hit) {
            rec.t = t_closest;
            rec.ponto = r.origem + r.direcao * rec.t;
            rec.mat = mat;
            return true;
        }

        return false;
}