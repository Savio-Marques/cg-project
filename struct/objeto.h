#ifndef OBJETO_H
#define OBJETO_H

#include "ray.h"
#include "material.h"
#include "mat4.h"

class Objeto {
public:
    Material mat;
    Mat4 transform;         
    Mat4 inverseTransform; 

    virtual std::string getTipo() const = 0;

    Objeto() {
        setTransform(Mat4::identity());
    }

    // Configura a matriz e já calcula a inversa
    void setTransform(const Mat4& t) {
        transform = t;
        inverseTransform = t.inverse();
    }

    // Converte o raio antes de chamar a forma
    virtual bool intersecta(const Ray& r_mundo, double t_min, double t_max, HitRecord& rec) const {
        
        // Raio inverso
        Ray r_local;
        r_local.origem = inverseTransform.point(r_mundo.origem);
        
        // Não normalizado a direção para manter o 't' correto
        r_local.direcao = inverseTransform.vector(r_mundo.direcao);

        // Chama a função geométrica do objeto
        if (!intersectaLocal(r_local, t_min, t_max, rec)) {
            return false;
        }

        // Se bateu converte dados de volta
        
        // O Ponto volta com a transformada normal
        rec.ponto = transform.point(rec.ponto);

        // A Normal volta com a transposta inversa para corrigir Cisalhamento/Escala
        Mat4 normalMatrix = inverseTransform.transpose();
        rec.normal = normalMatrix.vector(rec.normal).normalize();

        return true;
    }

    virtual ~Objeto() {}

protected:
    virtual bool intersectaLocal(const Ray& r_local, double t_min, double t_max, HitRecord& rec) const = 0;
};

#endif