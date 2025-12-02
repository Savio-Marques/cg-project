#ifndef OBJETO_H
#define OBJETO_H

#include "ray.h"
#include "material.h"
#include "mat4.h" // Inclui nossa nova matemática

class Objeto {
public:
    Material mat;
    Mat4 transform;         // Leva do Local -> Mundo
    Mat4 inverseTransform;  // Leva do Mundo -> Local (Pré-calculada)

    Objeto() {
        setTransform(Mat4::identity());
    }

    // Configura a matriz e já calcula a inversa para não perder tempo no render
    void setTransform(const Mat4& t) {
        transform = t;
        inverseTransform = t.inverse();
    }

    // A função intersecta agora é CONCRETA (não é mais pure virtual = 0).
    // Ela faz o trabalho sujo de converter o raio antes de chamar a forma.
    virtual bool intersecta(const Ray& r_mundo, double t_min, double t_max, HitRecord& rec) const {
        
        // 1. TRUQUE DO RAIO INVERSO (Mundo -> Local)
        Ray r_local;
        r_local.origem = inverseTransform.point(r_mundo.origem);
        
        // IMPORTANTE: Não normalizamos a direção aqui para manter o 't' correto
        r_local.direcao = inverseTransform.vector(r_mundo.direcao);

        // 2. Chama a função geométrica da forma (Esfera, Cubo, etc)
        if (!intersectaLocal(r_local, t_min, t_max, rec)) {
            return false;
        }

        // 3. SE BATEU: Converter dados de volta (Local -> Mundo)
        
        // O Ponto volta com a transformada normal
        rec.ponto = transform.point(rec.ponto);

        // A Normal volta com a TRANSPOSTA DA INVERSA (Correção para Cisalhamento/Escala)
        // Matematica: N_mundo = (M^-1)^T * N_local
        Mat4 normalMatrix = inverseTransform.transpose();
        rec.normal = normalMatrix.vector(rec.normal).normalize();

        return true;
    }

    virtual ~Objeto() {}

protected:
    // NOVA FUNÇÃO PURA: As formas agora devem implementar ESTA função
    // Elas vão assumir que estão sempre na origem (0,0,0) sem rotação.
    virtual bool intersectaLocal(const Ray& r_local, double t_min, double t_max, HitRecord& rec) const = 0;
};

#endif