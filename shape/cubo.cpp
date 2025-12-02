#include "cubo.h"

Cubo::Cubo(const Vec3& c, double a, const Material& m) {
    mat = m;
    double meia = a / 2.0;

    // 1. Definir os 8 vértices (Cantos)
    // Base (y = 0 se centroBase for no chão)
    Vec3 v0 = {c.x - meia, c.y,       c.z + meia}; // Frente Esquerda Baixo
    Vec3 v1 = {c.x + meia, c.y,       c.z + meia}; // Frente Direita Baixo
    Vec3 v2 = {c.x + meia, c.y,       c.z - meia}; // Trás Direita Baixo
    Vec3 v3 = {c.x - meia, c.y,       c.z - meia}; // Trás Esquerda Baixo

    // Topo (y = altura)
    Vec3 v4 = {c.x - meia, c.y + a,   c.z + meia}; // Frente Esquerda Cima
    Vec3 v5 = {c.x + meia, c.y + a,   c.z + meia}; // Frente Direita Cima
    Vec3 v6 = {c.x + meia, c.y + a,   c.z - meia}; // Trás Direita Cima
    Vec3 v7 = {c.x - meia, c.y + a,   c.z - meia}; // Trás Esquerda Cima

    // 2. Criar as 6 faces (2 triângulos por face)
    // Ordem anti-horária (Regra da mão direita para a normal apontar para fora)

    // Face da Frente (Z positivo)
    triangulos.push_back(Triangulo(v0, v1, v5, m));
    triangulos.push_back(Triangulo(v0, v5, v4, m));

    // Face da Direita (X positivo)
    triangulos.push_back(Triangulo(v1, v2, v6, m));
    triangulos.push_back(Triangulo(v1, v6, v5, m));

    // Face de Trás (Z negativo) - Cuidado com a ordem aqui!
    triangulos.push_back(Triangulo(v2, v3, v7, m));
    triangulos.push_back(Triangulo(v2, v7, v6, m));

    // Face da Esquerda (X negativo)
    triangulos.push_back(Triangulo(v3, v0, v4, m));
    triangulos.push_back(Triangulo(v3, v4, v7, m));

    // Face de Cima (Y positivo)
    triangulos.push_back(Triangulo(v4, v5, v6, m));
    triangulos.push_back(Triangulo(v4, v6, v7, m));

    // Face de Baixo (Y negativo)
    triangulos.push_back(Triangulo(v3, v2, v1, m));
    triangulos.push_back(Triangulo(v3, v1, v0, m));
}

bool Cubo::intersecta(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    bool acertou_algum = false;
    double t_mais_proximo = t_max;
    HitRecord temp_rec;

    // Testa o raio contra CADA UM dos 12 triângulos
    for (const auto& tri : triangulos) {
        if (tri.intersecta(r, t_min, t_mais_proximo, temp_rec)) {
            acertou_algum = true;
            t_mais_proximo = temp_rec.t; // Atualiza o limite máximo para o próximo teste
            rec = temp_rec;              // Salva o registro vencedor
        }
    }

    return acertou_algum;
}