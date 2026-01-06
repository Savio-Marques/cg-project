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
#include "struct/mat4.h"    
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
    // --------------------------------------------------
    // CONFIGURAÇÕES
    // --------------------------------------------------
    int nCol = 500; 
    int nLin = 500;
    
    // TIPO DE PROJEÇÃO (Requisito 3)
    // 0 = Perspectiva (Padrão)
    // 1 = Ortográfica
    // 2 = Oblíqua (NOVO)
    int tipoProjecao = 0; 

    // CÂMERA (Requisito 1.2 e 2.1)
    // Posicionada no 1º Octante (X,Y,Z positivos) olhando para o centro da "mesa"
    Vec3 lookfrom = {120, 100, 200};   
    Vec3 lookat   = {50, 40, 50};      
    Vec3 vup      = {0, 1, 0};      
    
    double distToFocus = 10.0;     
    double fov = 45.0; 

    // --------------------------------------------------
    
    // Matrizes de Câmera
    Mat4 viewMatrix = Mat4::lookAt(lookfrom, lookat, vup);
    Mat4 camToWorld = viewMatrix.inverse();

    // Cálculo da Janela
    double hJanela, wJanela;
    if (tipoProjecao != 0) { 
        hJanela = 150.0; // Tamanho fixo (em metros) para Orto/Obliqua
    } else {
        hJanela = 2.0 * distToFocus * tan((fov / 2.0) * M_PI / 180.0);
    }
    wJanela = hJanela;

    // ==================================================
    // 2. MATERIAIS
    // ==================================================
    
    Texture texChao;
    bool carregou = texChao.load("piso.png"); 
    Material mat_piso;
    mat_piso.shininess = 30.0; 
    mat_piso.Ke = {0.2, 0.2, 0.2};
    if (carregou) { mat_piso.useTexture = true; mat_piso.texturePtr = &texChao; }
    else { mat_piso.Ka = {0.5, 0.5, 0.5}; mat_piso.Kd = {0.6, 0.6, 0.6}; }

    Material mat_ouro; // Esfera
    mat_ouro.Ka = {0.24, 0.19, 0.07};
    mat_ouro.Kd = {0.75, 0.60, 0.22};
    mat_ouro.Ke = {0.62, 0.55, 0.36};
    mat_ouro.shininess = 51.2;

    Material mat_azul; // Cone
    mat_azul.Ka = {0.0, 0.0, 0.1};
    mat_azul.Kd = {0.0, 0.0, 0.8};
    mat_azul.Ke = {1.0, 1.0, 1.0};
    mat_azul.shininess = 20.0;

    Material mat_rubi; // Pilar
    mat_rubi.Ka = {0.17, 0.01, 0.01};
    mat_rubi.Kd = {0.61, 0.04, 0.04};
    mat_rubi.Ke = {0.72, 0.62, 0.62};
    mat_rubi.shininess = 76.8;

    // ==================================================
    // 3. CENA (Coerência Temática e Positiva)
    // ==================================================
    std::vector<Objeto*> cena;

    // 1. O Chão (Plano XZ em Y=0)
    Plano* chao = new Plano(mat_piso);
    chao->setTransform(Mat4::translate(0, 0, 0)); 
    cena.push_back(chao);

    // 2. Mesa (Cubo esticado)
    Cubo* pedestal = new Cubo(20.0, mat_piso); 
    // X=50, Y=10, Z=50
    Mat4 trPedestal = Mat4::translate(50, 10, 50) * Mat4::scale(4.0, 1.0, 2.0); 
    pedestal->setTransform(trPedestal);
    cena.push_back(pedestal);

    // 3. Esfera de Ouro
    Esfera* bola = new Esfera(15.0, mat_ouro);
    bola->setTransform(Mat4::translate(50, 35, 50));
    cena.push_back(bola);

    // 4. Cone Azul (Rotação Arbitrária aqui!)
    Cone* cone = new Cone(10.0, 30.0, mat_azul);
    // REQUISITO 1.4.2: Rotaciona 45 graus no eixo diagonal (1, 0, 1)
    Mat4 trCone = Mat4::translate(20, 40, 50) * Mat4::rotate(45.0, {1, 0, 1}); 
    cone->setTransform(trCone);
    cena.push_back(cone);

    // 5. Pilar de Rubi (Cilindro)
    Cilindro* pilar = new Cilindro(5.0, 100.0, mat_rubi);
    pilar->setTransform(Mat4::translate(100, 0, 0)); 
    cena.push_back(pilar);

    // ==================================================
    // 4. LUZES
    // ==================================================
    std::vector<Light> luzes;

    // Pontual
    Light lampada;
    lampada.type = POINT;
    lampada.position = {50, 100, 50}; 
    lampada.intensity = {0.6, 0.6, 0.6};
    luzes.push_back(lampada);

    // // Spot
    // Light spot;
    // spot.type = SPOT;
    // spot.position = {80, 80, 80};      
    // spot.direction = {-1, -1, -1}; 
    // spot.intensity = {1.0, 0.8, 0.0}; 
    // spot.cutoff = 15; 
    // luzes.push_back(spot);

    // // Direcional
    // Light sol;
    // sol.type = DIRECTIONAL;
    // sol.direction = {-1, -0.5, 0};   
    // sol.intensity = {0.3, 0.3, 0.4}; 
    // luzes.push_back(sol);
    
    Vec3 luzAmbiente = {0.2, 0.2, 0.2};

    // ==================================================
    // 5. RENDERIZAÇÃO
    // ==================================================
    std::vector<std::vector<Vec3>> imagem(nLin, std::vector<Vec3>(nCol));
    double Dx = wJanela / nCol;
    double Dy = hJanela / nLin;

    std::cout << "Iniciando render..." << std::endl;
    if (tipoProjecao == 1) std::cout << "Modo: ORTOGRAFICO" << std::endl;
    else if (tipoProjecao == 2) std::cout << "Modo: OBLIQUO" << std::endl;
    else std::cout << "Modo: PERSPECTIVA" << std::endl;

    for (int l = 0; l < nLin; l++) {
        if (l % 50 == 0) std::cout << "Linha: " << l << std::endl;
        for (int c = 0; c < nCol; c++) {
            
            double x_cam = -wJanela / 2.0 + Dx / 2.0 + c * Dx;
            double y_cam =  hJanela / 2.0 - Dy / 2.0 - l * Dy;
            
            Vec3 origem_local, direcao_local;

            // --- LÓGICA DE PROJEÇÃO ---
            if (tipoProjecao == 1) { // Ortográfica
                origem_local = {x_cam, y_cam, 0};
                direcao_local = {0, 0, -1};
            } 
            else if (tipoProjecao == 2) { // Oblíqua (Bonus)
                origem_local = {x_cam, y_cam, 0};
                // Direção inclinada: mantemos -1 em Z, mas deslocamos X e Y
                // Isso faz as laterais dos objetos aparecerem
                direcao_local = {-0.5, -0.5, -1.0}; 
            }
            else { // Perspectiva
                origem_local = {0, 0, 0};
                direcao_local = Vec3{x_cam, y_cam, -distToFocus}.normalize();
            }

            Ray raio;
            raio.origem = camToWorld.point(origem_local);    
            raio.direcao = camToWorld.vector(direcao_local).normalize();         

            HitRecord rec_final;
            bool houve_hit = false;
            double t_mais_proximo = std::numeric_limits<double>::max();

            for (const auto& objeto : cena) {
                HitRecord rec_temp;
                if (objeto->intersecta(raio, 0.001, t_mais_proximo, rec_temp)) {
                    houve_hit = true;
                    t_mais_proximo = rec_temp.t;
                    rec_final = rec_temp;
                }
            }

            if (houve_hit) {
                Vec3 Ka = rec_final.mat.Ka;
                Vec3 Kd = rec_final.mat.Kd;
                if (rec_final.mat.useTexture && rec_final.mat.texturePtr) {
                    Texture* tex = static_cast<Texture*>(rec_final.mat.texturePtr);
                    Vec3 texColor = tex->getColor(rec_final.u, rec_final.v);
                    Ka = texColor; Kd = texColor;
                }
                
                Vec3 cor = calcularIluminacao(raio, rec_final, cena, luzes, luzAmbiente, Ka, Kd, rec_final.mat.Ke);
                
                cor = cor * 255.0;
                cor.x = std::min(255.0, std::max(0.0, cor.x)); 
                cor.y = std::min(255.0, std::max(0.0, cor.y)); 
                cor.z = std::min(255.0, std::max(0.0, cor.z)); 
                imagem[l][c] = cor;
            } else {
                imagem[l][c] = {0, 0, 0}; 
            }
        }
    }
    
    std::ofstream arquivoPPM("cena.ppm");
    arquivoPPM << "P3\n" << nCol << " " << nLin << "\n255\n"; 
    for (int l = 0; l < nLin; l++) {
        for (int c = 0; c < nCol; c++) {
            arquivoPPM << (int)imagem[l][c].x << " " << (int)imagem[l][c].y << " " << (int)imagem[l][c].z << "\n";
        }
    }
    arquivoPPM.close();
    std::cout << "Imagem salva!\n";

    for(auto& obj : cena) delete obj;
    return 0;
}