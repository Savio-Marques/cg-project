#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <cmath>

// ========================================================================
// 1. INCLUDES
// ========================================================================

// Estruturas Básicas
#include "struct/vec3.h"
#include "struct/ray.h"
#include "struct/material.h" 
#include "struct/objeto.h"
#include "struct/texture.h"
#include "struct/light.h"   // <--- NOVO: Estrutura da luz

// Lógica de Renderização
#include "light/iluminacao.h"     // <--- NOVO: Onde está a matemática pesada

// Formas Geométricas
#include "shape/esfera.h"
#include "shape/plano.h"
#include "shape/cilindro.h"
#include "shape/cone.h"
#include "shape/cubo.h"

// ========================================================================
// MAIN
// ========================================================================

int main() {
    // ------------------------------------------------
    // 1. CONFIGURAÇÃO DA CÂMERA E TELA
    // ------------------------------------------------
    double wJanela = 60.0;
    double hJanela = 60.0;
    int nCol = 500;
    int nLin = 500;
    double dJanela = 30.0;
    Vec3 olho = {0, 0, 0};

    // ------------------------------------------------
    // 2. CARREGAMENTO DE TEXTURAS
    // ------------------------------------------------
    Texture texChao;
    bool carregouTextura = texChao.load("piso.png"); 
    
    // Se quiser adicionar textura para o cubo ou parede, carregue aqui...

    // ------------------------------------------------
    // 3. DEFINIÇÃO DE MATERIAIS
    // ------------------------------------------------
    
    // Material do Chão (Texturizado ou Cor Sólida)
    Material mat_piso;
    mat_piso.shininess = 10.0; 

    if (carregouTextura) {
        mat_piso.useTexture = true;
        mat_piso.texturePtr = &texChao; 
        mat_piso.Ke = {0.1, 0.1, 0.1}; // Brilho especular fraco
    } else {
        mat_piso.useTexture = false;
        mat_piso.Ka = {0.4, 0.2, 0.1}; // Marrom
        mat_piso.Kd = {0.4, 0.2, 0.1};
        mat_piso.Ke = {0.0, 0.0, 0.0};
        std::cout << "AVISO: Usando cor solida para o chao." << std::endl;
    }

    // Outros Materiais
    Vec3 corParede = {0.686, 0.933, 0.933}; // Azul claro
    Material mat_parede = {corParede, corParede, corParede, 10.0};

    Vec3 corTeto = {0.933, 0.933, 0.933};   // Branco
    Material mat_teto = {corTeto, corTeto, corTeto, 10.0};

    Vec3 corCil = {0.824, 0.706, 0.549};    // Bege
    Material mat_cilindro = {corCil, corCil, corCil, 50.0};

    Vec3 corCone = {0.0, 1.0, 0.498};       // Verde Primavera
    Material mat_cone = {corCone, corCone, corCone, 50.0};

    Vec3 corCubo = {1.0, 0.078, 0.576};     // Rosa Choque
    Material mat_cubo = {corCubo, corCubo, corCubo, 50.0};

    Vec3 corEsfera = {0.854, 0.647, 0.125}; // Dourado
    Material mat_esfera = {corEsfera, corEsfera, corEsfera, 50.0};

    // ------------------------------------------------
    // 4. MONTAGEM DA CENA (OBJETOS)
    // ------------------------------------------------
    std::vector<Objeto*> cena;

    // Paredes, Teto e Chão
    cena.push_back(new Plano({0, -150, 0}, {0, 1, 0}, mat_piso));
    cena.push_back(new Plano({200, -150, 0}, {-1, 0, 0}, mat_parede));   // Dir
    cena.push_back(new Plano({200, -150, -400}, {0, 0, 1}, mat_parede)); // Fundo
    cena.push_back(new Plano({-200, -150, 0}, {1, 0, 0}, mat_parede));   // Esq
    cena.push_back(new Plano({0, 150, 0}, {0, -1, 0}, mat_teto));

    // Objetos
    cena.push_back(new Cilindro({0, -150, -200}, {0, 1, 0}, 5.0, 90.0, mat_cilindro));
    cena.push_back(new Cone({0, -60, -200}, {0, 1, 0}, 90.0, 150.0, mat_cone));
    
    // O Cubo novo (Malha de triângulos)
    // Atenção: Certifique-se que o construtor do cubo.cpp aceita (centro, tamanho, material)
    cena.push_back(new Cubo({0, -150, -165}, 40.0, mat_cubo));
    
    cena.push_back(new Esfera({0, 95, -200}, 5.0, mat_esfera));

    // ------------------------------------------------
    // 5. MONTAGEM DA ILUMINAÇÃO (LUZES)
    // ------------------------------------------------
    std::vector<Light> luzes;
    
    // Luz Principal (Branca)
    luzes.push_back(Light{ {-100, 140, -20}, {0.7, 0.7, 0.7} });

    // Exemplo: Adicionando uma segunda luz (azulada) na direita para dar clima
    // luzes.push_back(Light{ {100, 50, -50}, {0.1, 0.1, 0.4} });

    Vec3 luzAmbiente = {0.3, 0.3, 0.3};

    // ------------------------------------------------
    // 6. RENDERIZAÇÃO
    // ------------------------------------------------
    std::vector<std::vector<Vec3>> imagem(nLin, std::vector<Vec3>(nCol));
    double Dx = wJanela / nCol;
    double Dy = hJanela / nLin;

    std::cout << "Iniciando renderizacao..." << std::endl;

    for (int l = 0; l < nLin; l++) {
        // Mostra progresso a cada 50 linhas para não ficar ansioso
        if (l % 50 == 0) std::cout << "Linha: " << l << " de " << nLin << std::endl;

        for (int c = 0; c < nCol; c++) {
            
            // Define o raio para este pixel
            double x = -wJanela / 2.0 + Dx / 2.0 + c * Dx;
            double y =  hJanela / 2.0 - Dy / 2.0 - l * Dy;
            Ray raio = {olho, (Vec3{x, y, -dJanela} - olho).normalize()};

            HitRecord rec_temp, rec_final;
            bool houve_hit = false;
            double t_mais_proximo = std::numeric_limits<double>::max();

            // Descobre qual objeto está na frente
            for (const auto& objeto : cena) {
                if (objeto->intersecta(raio, 0.001, t_mais_proximo, rec_temp)) {
                    houve_hit = true;
                    t_mais_proximo = rec_temp.t;
                    rec_final = rec_temp;
                }
            }

            if (houve_hit) {
                // Prepara as cores base (Resolver Textura vs Cor Sólida)
                Vec3 Ka_atual, Kd_atual, Ke_atual;

                if (rec_final.mat.useTexture && rec_final.mat.texturePtr != nullptr) {
                    Texture* tex = static_cast<Texture*>(rec_final.mat.texturePtr);
                    Vec3 corImagem = tex->getColor(rec_final.u, rec_final.v);
                    
                    Ka_atual = corImagem;
                    Kd_atual = corImagem;
                    Ke_atual = rec_final.mat.Ke; // Especular continua vindo do material
                } else {
                    Ka_atual = rec_final.mat.Ka;
                    Kd_atual = rec_final.mat.Kd;
                    Ke_atual = rec_final.mat.Ke;
                }

                // CHAMA A CALCULADORA DE ILUMINAÇÃO (Tudo acontece aqui dentro!)
                // Ela já calcula sombra, diffuse e specular para todas as luzes
                Vec3 corCalculada = calcularIluminacao(
                    raio, 
                    rec_final, 
                    cena, 
                    luzes, 
                    luzAmbiente, 
                    Ka_atual, Kd_atual, Ke_atual
                );

                imagem[l][c] = corCalculada * 255.0;

            } else {
                imagem[l][c] = {0, 0, 0}; // Fundo preto
            }
        }
    }
    
    // ------------------------------------------------
    // 7. SALVAR ARQUIVO
    // ------------------------------------------------
    std::cout << "Salvando imagem 'cena.ppm'..." << std::endl;
    std::ofstream arquivoPPM("cena.ppm");
    arquivoPPM << "P3\n" << nCol << " " << nLin << "\n255\n"; 
    for (int l = 0; l < nLin; l++) {
        for (int c = 0; c < nCol; c++) {
            arquivoPPM << static_cast<int>(imagem[l][c].x) << " "
                       << static_cast<int>(imagem[l][c].y) << " "
                       << static_cast<int>(imagem[l][c].z) << "\n";
        }
    }
    arquivoPPM.close();

    // Limpeza de memória
    for(auto& obj : cena) delete obj;
    std::cout << "Concluido!" << std::endl;

    return 0;
}