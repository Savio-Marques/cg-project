#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <cmath>

// ========================================================================
// INCLUDES ORGANIZADOS POR PASTA
// ========================================================================

// Estruturas Básicas
#include "struct/vec3.h"
#include "struct/ray.h"
#include "struct/material.h" 
#include "struct/objeto.h"
#include "struct/texture.h"

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
    // Parâmetros da Câmera
    double wJanela = 60.0;
    double hJanela = 60.0;
    int nCol = 500;
    int nLin = 500;
    double dJanela = 30.0;
    Vec3 olho = {0, 0, 0};

    // ------------------------------------------------
    // 2. CARREGAMENTO DA TEXTURA
    // ------------------------------------------------
    Texture texChao;
    // Certifique-se de que "piso.png" está na mesma pasta do executável ou do main.cpp
    bool carregouTextura = texChao.load("piso.png"); 

    std::vector<Objeto*> cena;

    // ------------------------------------------------
    // DEFINIÇÃO DOS MATERIAIS
    // ------------------------------------------------
    
    // Material do Chão (Com Lógica de Textura)
    Material mat_piso;
    mat_piso.shininess = 10.0; 

    if (carregouTextura) {
        mat_piso.useTexture = true;
        mat_piso.texturePtr = &texChao; // Aponta para a textura carregada
        // Ka e Kd base serão substituídos pela cor da imagem no loop
        mat_piso.Ke = {0.1, 0.1, 0.1}; // Especular baixo (pouco brilho)
    } else {
        // Fallback: se não achar a imagem, usa cor sólida (Ex: Marrom)
        mat_piso.useTexture = false;
        mat_piso.Ka = {0.4, 0.2, 0.1}; 
        mat_piso.Kd = {0.4, 0.2, 0.1};
        mat_piso.Ke = {0.0, 0.0, 0.0};
        std::cout << "AVISO: Usando cor solida para o chao (piso.png nao encontrado)." << std::endl;
    }

    // Paredes
    Vec3 corParede = {0.686, 0.933, 0.933};
    Material mat_parede = {corParede, corParede, corParede, 10.0};

    // Teto
    Vec3 corTeto = {0.933, 0.933, 0.933};
    Material mat_teto = {corTeto, corTeto, corTeto, 10.0};

    // Cilindro
    Vec3 corCil = {0.824, 0.706, 0.549};
    Material mat_cilindro = {corCil, corCil, corCil, 50.0};

    // Cone
    Vec3 corCone = {0.0, 1.0, 0.498};
    Material mat_cone = {corCone, corCone, corCone, 50.0};

    // Cubo
    Vec3 corCubo = {1.0, 0.078, 0.576};
    Material mat_cubo = {corCubo, corCubo, corCubo, 50.0};

    // Esfera
    Vec3 corEsfera = {0.854, 0.647, 0.125};
    Material mat_esfera = {corEsfera, corEsfera, corEsfera, 50.0};

    // ------------------------------------------------
    // CRIAÇÃO DOS OBJETOS
    // ------------------------------------------------

    // Chão (Usa o material com textura configurado acima)
    cena.push_back(new Plano({0, -150, 0}, {0, 1, 0}, mat_piso));

    // Paredes
    cena.push_back(new Plano({200, -150, 0}, {-1, 0, 0}, mat_parede));    // Direita
    cena.push_back(new Plano({200, -150, -400}, {0, 0, 1}, mat_parede));  // Fundo
    cena.push_back(new Plano({-200, -150, 0}, {1, 0, 0}, mat_parede));    // Esquerda
    
    // Teto
    cena.push_back(new Plano({0, 150, 0}, {0, -1, 0}, mat_teto));

    // Objetos na cena
    cena.push_back(new Cilindro({0, -150, -200}, {0, 1, 0}, 5.0, 90.0, mat_cilindro));
    cena.push_back(new Cone({0, -60, -200}, {0, 1, 0}, 90.0, 150.0, mat_cone));
    cena.push_back(new Cubo({0, -150, -165}, 40.0, mat_cubo));
    cena.push_back(new Esfera({0, 95, -200}, 5.0, mat_esfera));

    // ------------------------------------------------
    // LUZES
    // ------------------------------------------------
    Vec3 IF_Pontual = {0.7, 0.7, 0.7};
    Vec3 P_F = {-100, 140, -20};
    Vec3 IF_Ambiente = {0.3, 0.3, 0.3};

    // ------------------------------------------------
    // RENDERIZAÇÃO
    // ------------------------------------------------
    std::vector<std::vector<Vec3>> imagem(nLin, std::vector<Vec3>(nCol));
    double Dx = wJanela / nCol;
    double Dy = hJanela / nLin;

    std::cout << "Renderizando cena com textura..." << std::endl;

    for (int l = 0; l < nLin; l++) {
        for (int c = 0; c < nCol; c++) {
            
            double x = -wJanela / 2.0 + Dx / 2.0 + c * Dx;
            double y =  hJanela / 2.0 - Dy / 2.0 - l * Dy;

            Ray raio = {olho, (Vec3{x, y, -dJanela} - olho).normalize()};

            HitRecord rec_temp, rec_final;
            bool houve_hit = false;
            double t_mais_proximo = std::numeric_limits<double>::max();

            // Verifica intersecção com todos os objetos
            for (const auto& objeto : cena) {
                if (objeto->intersecta(raio, 0.001, t_mais_proximo, rec_temp)) {
                    houve_hit = true;
                    t_mais_proximo = rec_temp.t;
                    rec_final = rec_temp;
                }
            }

            if (houve_hit) {
                Vec3 Ka_atual, Kd_atual, Ke_atual;
                
                // 3. LÓGICA DE SELEÇÃO DE COR (TEXTURA vs COR SÓLIDA)
                if (rec_final.mat.useTexture && rec_final.mat.texturePtr != nullptr) {
                    // Recupera o ponteiro da textura
                    Texture* tex = static_cast<Texture*>(rec_final.mat.texturePtr);
                    
                    // Busca a cor na imagem usando as coordenadas UV (calculadas no Plano::intersecta)
                    Vec3 corImagem = tex->getColor(rec_final.u, rec_final.v);
                    
                    Ka_atual = corImagem;
                    Kd_atual = corImagem;
                    Ke_atual = rec_final.mat.Ke; // Mantém o especular do material

                } else {
                    // Material Comum
                    Ka_atual = rec_final.mat.Ka;
                    Kd_atual = rec_final.mat.Kd;
                    Ke_atual = rec_final.mat.Ke;
                }

                Vec3 corFinal = {0,0,0};

                // Componente Ambiente
                Vec3 I_amb = IF_Ambiente.hadamard(Ka_atual);
                corFinal = corFinal + I_amb;

                // Sombra
                Vec3 vetorLuz = (P_F - rec_final.ponto).normalize();
                double distanciaLuz = (P_F - rec_final.ponto).norm();
                
                Ray raioSombra = {rec_final.ponto + rec_final.normal * 1e-3, vetorLuz};
                bool emSombra = false;

                for (const auto& objeto_sombra : cena) {
                    if (objeto_sombra->intersecta(raioSombra, 0.001, distanciaLuz, rec_temp)) {
                        emSombra = true;
                        break;
                    }
                }

                if (!emSombra) {
                    // Componente Difusa
                    double dot_l_n = std::max(0.0, vetorLuz.dot(rec_final.normal));
                    Vec3 I_dif = IF_Pontual.hadamard(Kd_atual) * dot_l_n;
                    corFinal = corFinal + I_dif;

                    // Componente Especular
                    Vec3 v = (olho - rec_final.ponto).normalize();
                    Vec3 r_ref = (rec_final.normal * (2.0 * vetorLuz.dot(rec_final.normal)) - vetorLuz).normalize();
                    double dot_v_r = std::max(0.0, v.dot(r_ref));
                    double dot_v_r_m = std::pow(dot_v_r, rec_final.mat.shininess);
                    
                    Vec3 I_spec = IF_Pontual.hadamard(Ke_atual) * dot_v_r_m;
                    corFinal = corFinal + I_spec;
                }
                imagem[l][c] = corFinal.clamp(0.0, 1.0) * 255.0;
            } else {
                imagem[l][c] = {0, 0, 0}; // Fundo preto
            }
        }
    }
    
    std::cout << "Cena gerada com sucesso! Salvando arquivo..." << std::endl;

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

    return 0;
}