#include "cone.h"
#include <cmath>
#include <algorithm> // Para std::min, std::max se necessário

Cone::Cone(const Vec3& b, const Vec3& axis, double r, double h, const Material& m) 
    : base(b), eixo(axis.normalize()), raio(r), altura(h) { 
    mat = m; 
}

bool Cone::intersecta(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    bool hit = false;
    double t_closest = t_max;
    Vec3 vertice = base + eixo * altura;
    Vec3 X = r.origem - vertice;

    double cos_theta_sq = (altura*altura) / (raio*raio + altura*altura);
    
    double dv = r.direcao.dot(eixo);
    double xv = X.dot(eixo);
    double dd = r.direcao.dot(r.direcao);
    double xx = X.dot(X);
    double xd = X.dot(r.direcao);
    
    double cos2 = cos_theta_sq;
    
    double a = dv*dv - cos2*dd;
    double b = 2 * (dv*xv - cos2*xd);
    double c = xv*xv - cos2*xx;
    
    double delta = b*b - 4*a*c;
    
    if (delta >= 0) {
        double sqrtDelta = sqrt(delta);
        double t1 = (-b - sqrtDelta) / (2*a);
        double t2 = (-b + sqrtDelta) / (2*a);

        // Lambda para verificar as intersecções laterais
        auto check_cone = [&](double t_cand) {
            if (t_cand > t_min && t_cand < t_closest) {
                Vec3 p = r.origem + r.direcao * t_cand;
                double h = (p - base).dot(eixo);
                if (h >= 0 && h <= altura) {
                    t_closest = t_cand;
                    hit = true;
                    
                    Vec3 cp = p - vertice;
                    Vec3 v_proj = eixo * (cp.dot(eixo));
                    Vec3 radial = cp - v_proj;
                    Vec3 tangent = (eixo.cross(radial)).normalize();
                    Vec3 bitangent = (cp.cross(tangent)).normalize();
                    rec.normal = bitangent;
                }
            }
        };
        check_cone(t1);
        check_cone(t2);
    }

    // Base do cone (Disco)
    double denom = (-eixo).dot(r.direcao);
    if (std::abs(denom) > 1e-6) {
        double t = (base - r.origem).dot(-eixo) / denom;
        if (t > t_min && t < t_closest) {
            Vec3 p = r.origem + r.direcao * t;
            if ((p - base).norm() <= raio) {
                t_closest = t;
                hit = true;
                rec.normal = -eixo;
            }
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