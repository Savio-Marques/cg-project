#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <cmath>

// ========================================================================
// 1. INCLUDES
// ========================================================================

#include "struct/vec3.h"
#include "struct/ray.h"
#include "struct/material.h" 
#include "struct/objeto.h"
#include "struct/texture.h"
#include "struct/light.h"
#include "struct/mat4.h"    // <--- IMPORTANTE: Incluir as matrizes

#include "light/iluminacao.h" 

#include "shape/esfera.h"
#include "shape/plano.h"
#include "shape/cilindro.h"
#include "shape/cone.h"
#include "shape/cubo.h"

// ========================================================================
// MAIN
// ========================================================================

int main() {
    // 1. CONFIGURAÇÃO DA CÂMERA
    double wJanela = 60.0;
    double hJanela = 60.0;
    int nCol = 500;
    int nLin = 500;
    double dJanela = 30.0;
    Vec3 olho = {0, 0, -5};

    // 2. TEXTURAS
    Texture texChao;
    bool carregouTextura = texChao.load("piso.png"); 

    // 3. MATERIAIS
    Material mat_piso;
    mat_piso.shininess = 10.0; 
    if (carregouTextura) {
        mat_piso.useTexture = true;
        mat_piso.texturePtr = &texChao; 
        mat_piso.Ke = {0.1, 0.1, 0.1};
    } else {
        mat_piso.useTexture = false;
        mat_piso.Ka = {0.4, 0.2, 0.1}; 
        mat_piso.Kd = {0.4, 0.2, 0.1};
        mat_piso.Ke = {0.0, 0.0, 0.0};
    }

    Vec3 corParede = {0.686, 0.933, 0.933};
    Material mat_parede = {corParede, corParede, corParede, 10.0};

    Vec3 corTeto = {0.933, 0.933, 0.933};
    Material mat_teto = {corTeto, corTeto, corTeto, 10.0};

    Vec3 corCil = {0.824, 0.706, 0.549};
    Material mat_cilindro = {corCil, corCil, corCil, 50.0};

    Vec3 corCone = {0.0, 1.0, 0.498};
    Material mat_cone = {corCone, corCone, corCone, 50.0};

    Vec3 corCubo = {1.0, 0.078, 0.576};
    Material mat_cubo = {corCubo, corCubo, corCubo, 50.0};

    Vec3 corEsfera = {0.854, 0.647, 0.125};
    Material mat_esfera = {corEsfera, corEsfera, corEsfera, 50.0};

    // ------------------------------------------------
    // 4. MONTAGEM DA CENA (COM MATRIZES DE TRANSFORMAÇÃO)
    // ------------------------------------------------
    std::vector<Objeto*> cena;

    // --- CHÃO ---
    // Cria plano padrão (XZ) na origem
    Plano* chao = new Plano(mat_piso);
    // Translada para y = -150
    chao->setTransform(Mat4::translate(0, -150, 0));
    cena.push_back(chao);

    // --- TETO ---
    Plano* teto = new Plano(mat_teto);
    // Move pra cima e gira 180 no X para a normal apontar pra baixo
    Mat4 trTeto = Mat4::translate(0, 150, 0) * Mat4::rotateX(180); 
    teto->setTransform(trTeto);
    cena.push_back(teto);

    // --- PAREDE FUNDO ---
    Plano* fundo = new Plano(mat_parede);
    // Gira 90 no X (fica em pé) e move para trás
    Mat4 trFundo = Mat4::translate(0, 0, -400) * Mat4::rotateX(90);
    fundo->setTransform(trFundo);
    cena.push_back(fundo);

    // --- PAREDE DIREITA ---
    Plano* dir = new Plano(mat_parede);
    // Gira 90 no Z e move
    Mat4 trDir = Mat4::translate(200, 0, 0) * Mat4::rotateZ(90);
    dir->setTransform(trDir);
    cena.push_back(dir);

    // --- PAREDE ESQUERDA ---
    Plano* esq = new Plano(mat_parede);
    Mat4 trEsq = Mat4::translate(-200, 0, 0) * Mat4::rotateZ(-90);
    esq->setTransform(trEsq);
    cena.push_back(esq);


    // --- CILINDRO ---
    // Cria na origem (raio 5, altura 90)
    Cilindro* cil = new Cilindro(5.0, 90.0, mat_cilindro);
    // Apenas move para posição final
    cil->setTransform(Mat4::translate(0, -150, -200));
    cena.push_back(cil);


    // --- CONE ---
    Cone* cone = new Cone(90.0, 150.0, mat_cone);
    // Move para posição
    cone->setTransform(Mat4::translate(0, -60, -200));
    cena.push_back(cone);


    // --- CUBO (MALHA DE TRIÂNGULOS) ---
    // Cria centrado na origem com aresta 40
    Cubo* cubo = new Cubo(40.0, mat_cubo);
    
    // AULA 15: Composição de Transformações
    // Vamos girar o cubo E mover ele.
    // 1. Matriz de Rotação (45 graus no Y)

    Mat4 rotCubo = Mat4::rotateY(0);
    // 2. Matriz de Translação
    Mat4 movCubo = Mat4::translate(0, -150, -165);
    
    // Aplica: PRIMEIRO gira, DEPOIS move (multiplicação da direita pra esquerda na teoria, mas aqui depende da implementação do operador*)
    // Na nossa Mat4::operator*, a ordem de leitura é: (MatrizPai * MatrizFilho)
    cubo->setTransform(movCubo * rotCubo); 
    cena.push_back(cubo);


    // --- ESFERA ---
    Esfera* esfera = new Esfera(5.0, mat_esfera);
    esfera->setTransform(Mat4::translate(0, 95, -200));
    cena.push_back(esfera);


    // 5. LUZES
    std::vector<Light> luzes;
    luzes.push_back(Light{ {-100, 140, -20}, {0.7, 0.7, 0.7} });
    Vec3 luzAmbiente = {0.3, 0.3, 0.3};


    // 6. RENDERIZAÇÃO
    std::vector<std::vector<Vec3>> imagem(nLin, std::vector<Vec3>(nCol));
    double Dx = wJanela / nCol;
    double Dy = hJanela / nLin;

    std::cout << "Iniciando renderizacao..." << std::endl;

    for (int l = 0; l < nLin; l++) {
        if (l % 50 == 0) std::cout << "Linha: " << l << " de " << nLin << std::endl;

        for (int c = 0; c < nCol; c++) {
            double x = -wJanela / 2.0 + Dx / 2.0 + c * Dx;
            double y =  hJanela / 2.0 - Dy / 2.0 - l * Dy;
            Ray raio = {olho, (Vec3{x, y, -dJanela} - olho).normalize()};

            HitRecord rec_temp, rec_final;
            bool houve_hit = false;
            double t_mais_proximo = std::numeric_limits<double>::max();

            for (const auto& objeto : cena) {
                // AQUI A MÁGICA ACONTECE: 
                // O método intersecta do Objeto aplica a transformação inversa no raio
                if (objeto->intersecta(raio, 0.001, t_mais_proximo, rec_temp)) {
                    houve_hit = true;
                    t_mais_proximo = rec_temp.t;
                    rec_final = rec_temp;
                }
            }

            if (houve_hit) {
                Vec3 Ka_atual, Kd_atual, Ke_atual;

                if (rec_final.mat.useTexture && rec_final.mat.texturePtr != nullptr) {
                    Texture* tex = static_cast<Texture*>(rec_final.mat.texturePtr);
                    Vec3 corImagem = tex->getColor(rec_final.u, rec_final.v);
                    Ka_atual = corImagem;
                    Kd_atual = corImagem;
                    Ke_atual = rec_final.mat.Ke; 
                } else {
                    Ka_atual = rec_final.mat.Ka;
                    Kd_atual = rec_final.mat.Kd;
                    Ke_atual = rec_final.mat.Ke;
                }

                Vec3 corCalculada = calcularIluminacao(
                    raio, rec_final, cena, luzes, luzAmbiente, 
                    Ka_atual, Kd_atual, Ke_atual
                );

                imagem[l][c] = corCalculada * 255.0;
            } else {
                imagem[l][c] = {0, 0, 0};
            }
        }
    }
    
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

    for(auto& obj : cena) delete obj;
    std::cout << "Concluido!" << std::endl;

    return 0;
}