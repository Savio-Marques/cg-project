#ifndef MOTOR_RENDER_H
#define MOTOR_RENDER_H

#include <vector>
#include <algorithm>
#include <cmath>
#include "struct/vec3.h"
#include "struct/ray.h"
#include "scene/scene.h"
#include "scene/settings.h"
#include "light/iluminacao.h"
#include "struct/texture.h"

void executarRayTracing(std::vector<unsigned char>& buffer, const SceneSettings& config, const Scene& cena) {
    //Configuração de Câmera
    Vec3 eye = {config.lookfrom[0], config.lookfrom[1], config.lookfrom[2]};
    Vec3 at  = {config.lookat[0],   config.lookat[1],   config.lookat[2]};
    Vec3 up  = {config.lookup[0],   config.lookup[1],   config.lookup[2]};
    
    Mat4 camToWorld = Mat4::lookAt(eye, at, up).inverse();
    Vec3 direcaoFrente = (at - eye).normalize();

    // Cálculos de FOV e Proporção (Olhar pick na main)
    float aspect = (float)config.nCol / config.nLin;
    float theta = config.fov * M_PI / 180.0f;
    float h = tan(theta / 2.0f);

    for (int l = 0; l < config.nLin; l++) {
        for (int c = 0; c < config.nCol; c++) {
            double nx = (2.0 * (c + 0.5) / config.nCol - 1.0);
            double ny = (1.0 - 2.0 * (l + 0.5) / config.nLin);
            
            Ray raio;
            if (config.tipoProjecao == 0) {
                // PERSPECTIVA
                double x = nx * h * aspect;
                double y = ny * h;
                raio = { eye, camToWorld.vector(Vec3{x, y, -1.0}).normalize() };
            } else {
                // ORTOGRÁFICA
                float s = config.orthoSize;
                Vec3 origemD = eye + camToWorld.vector(Vec3{nx * s * aspect, ny * s, 0.0});
                raio = { origemD, direcaoFrente };
            }

            HitRecord rec_final;
            bool houve_hit = false;
            double t_proximo = 1e30;

            //Teste de Interseção
            for (auto obj : cena.objetos) {
                HitRecord rec_temp;
                if (obj->intersecta(raio, 0.001, t_proximo, rec_temp)) {
                    houve_hit = true;
                    t_proximo = rec_temp.t;
                    rec_final = rec_temp;
                }
            }

            //Processamento de Cor e Textura
            int idx = (l * config.nCol + c) * 3;
            if (houve_hit) {
                // Cores base do material
                Vec3 Ka_final = rec_final.mat.Ka;
                Vec3 Kd_final = rec_final.mat.Kd;

                // Se houver, substituímos as cores base pela cor da imagem
                if (rec_final.mat.useTexture && rec_final.mat.texturePtr != nullptr) {
                    Vec3 corTextura = ((Texture*)rec_final.mat.texturePtr)->getColor(rec_final.u, rec_final.v);
                    Kd_final = corTextura;
                    Ka_final = corTextura * 0.2;
                }

                // Cálculo de Iluminação passando as cores (fixas ou textura)
                Vec3 cor = calcularIluminacao(
                    raio, 
                    rec_final, 
                    cena.objetos, 
                    cena.luzes, 
                    cena.corAmbiente, 
                    Ka_final, 
                    Kd_final, 
                    rec_final.mat.Ke
                );
                
                buffer[idx]   = (unsigned char)(std::min(1.0, cor.x) * 255);
                buffer[idx+1] = (unsigned char)(std::min(1.0, cor.y) * 255);
                buffer[idx+2] = (unsigned char)(std::min(1.0, cor.z) * 255);
            } else {
                buffer[idx] = buffer[idx+1] = buffer[idx+2] = 30; // Fundo
            }
        }
    }
}
#endif