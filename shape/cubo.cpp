#include "cubo.h"

Cubo::Cubo(double a, const Material& m) {
    mat = m;
    double meia = a / 2.0;

    // Vértices centrados na origem (0,0,0)
    // X, Y e Z vão de -meia até +meia
    Vec3 v0 = {-meia, -meia,  meia}; // Frente Esq Baixo
    Vec3 v1 = { meia, -meia,  meia}; // Frente Dir Baixo
    Vec3 v2 = { meia, -meia, -meia}; // Trás Dir Baixo
    Vec3 v3 = {-meia, -meia, -meia}; // Trás Esq Baixo

    Vec3 v4 = {-meia,  meia,  meia}; // Frente Esq Cima
    Vec3 v5 = { meia,  meia,  meia}; // Frente Dir Cima
    Vec3 v6 = { meia,  meia, -meia}; // Trás Dir Cima
    Vec3 v7 = {-meia,  meia, -meia}; // Trás Esq Cima

    // --- CRIAÇÃO DAS FACES (12 Triângulos) ---
    // Frente
    triangulos.push_back(Triangulo(v0, v1, v5, m));
    triangulos.push_back(Triangulo(v0, v5, v4, m));

    // Direita
    triangulos.push_back(Triangulo(v1, v2, v6, m));
    triangulos.push_back(Triangulo(v1, v6, v5, m));

    // Trás
    triangulos.push_back(Triangulo(v2, v3, v7, m));
    triangulos.push_back(Triangulo(v2, v7, v6, m));

    // Esquerda
    triangulos.push_back(Triangulo(v3, v0, v4, m));
    triangulos.push_back(Triangulo(v3, v4, v7, m));

    // Cima
    triangulos.push_back(Triangulo(v4, v5, v6, m));
    triangulos.push_back(Triangulo(v4, v6, v7, m));

    // Baixo
    triangulos.push_back(Triangulo(v3, v2, v1, m));
    triangulos.push_back(Triangulo(v3, v1, v0, m));
}

bool Cubo::intersectaLocal(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    bool acertou_algum = false;
    double t_mais_proximo = t_max;
    HitRecord temp_rec;

    // Testamos contra todos os triângulos da lista
    for (const auto& tri : triangulos) {
        
        // DICA IMPORTANTE:
        // Como 'tri' é um Objeto, ele tem sua própria matriz. Mas aqui dentro do cubo, 
        // a matriz dele é Identidade (padrão). Então chamar 'intersectaLocal' direto
        // é mais rápido e matematicamente correto, pois o raio 'r' já é local do Cubo.
        if (tri.intersectaLocal(r, t_min, t_mais_proximo, temp_rec)) {
            acertou_algum = true;
            t_mais_proximo = temp_rec.t;
            rec = temp_rec;
        }
    }

    return acertou_algum;
}