#ifndef SCENE_BUILDER_H
#define SCENE_BUILDER_H

#include "scene.h"
#include "settings.h"
#include "shape/cilindro.h"
#include "shape/cubo.h"
#include "shape/plano.h"
#include "shape/esfera.h"
#include "shape/cone.h"
#include "shape/triangulo.h"

#include "struct/texture.h"
#include "struct/vec3.h"

namespace SceneBuilder
{
    void montarCenaEsfera(Scene &cena, const SceneSettings &config)
    {
        cena.limpar();

        Light point;
        point.type = POINT;
        point.position = {config.lightPointPos[0], config.lightPointPos[1], config.lightPointPos[2]};
        point.intensity = {config.lightPointIntensity[0], config.lightPointIntensity[1], config.lightPointIntensity[2]};
        cena.adicionarLuz(point);

        Light sol;
        sol.type = DIRECTIONAL;
        sol.direction = {config.lightSunDirection[0], config.lightSunDirection[1], config.lightSunDirection[2]};
        sol.intensity = {config.lightSunIntensity[0], config.lightSunIntensity[1], config.lightSunIntensity[2]};
        cena.adicionarLuz(sol);

        Light spot;
        spot.type = SPOT;
        spot.position = {config.lightSpotPos[0], config.lightSpotPos[1], config.lightSpotPos[2]};
        spot.direction = {config.lightSpotDirection[0], config.lightSpotDirection[1], config.lightSpotDirection[2]};
        spot.intensity = {config.lightSpotIntensity[0], config.lightSpotIntensity[1], config.lightSpotIntensity[2]};
        spot.cutoff = config.lightSpotCutOff;
        cena.adicionarLuz(spot);

        cena.corAmbiente = {config.corAmbiente[0], config.corAmbiente[1], config.corAmbiente[2]};

        // Plano
        Material mat_piso;
        Texture *texChao = new Texture();
        if (texChao->load("nuvens.png"))
        {
            mat_piso.useTexture = true;
            mat_piso.texturePtr = texChao;
            cena.texturas.push_back(texChao);
        }

        Plano *plano = new Plano(mat_piso);
        plano->setTransform(Mat4::translate(config.translatePlano1[0], config.translatePlano1[1], config.translatePlano1[2]) * Mat4::rotateX(config.rotatePlano1));
        cena.adicionarObjeto(plano);

        // Folhas
        Material matFolha = {{config.matFolhaKa[0], config.matFolhaKa[1], config.matFolhaKa[2]}, 
                            {config.matFolhaKd[0], config.matFolhaKd[1], config.matFolhaKd[2]}, 
                            {config.matFolhaKe[0], config.matFolhaKe[1], config.matFolhaKe[2]}, 
                            config.matFolhaShiness};

        // Material matFolha = {{0.01, 0.04, 0.02}, {0.05, 0.22, 0.08}, {0.0, 0.0, 0.0}, 1};
        Cone *cone1 = new Cone(config.especCone[0], config.especCone[1], matFolha);
        cone1->setTransform(Mat4::translate(config.translateCone1[0], config.translateCone1[1], config.translateCone1[2]));
        cena.adicionarObjeto(cone1);

        Cone *cone2 = new Cone(config.especCone[0], config.especCone[1], matFolha);
        cone2->setTransform(Mat4::translate(config.translateCone2[0], config.translateCone2[1], config.translateCone2[2]));
        cena.adicionarObjeto(cone2);

        Cone *cone3 = new Cone(config.especCone[0], config.especCone[1], matFolha);
        cone3->setTransform(Mat4::translate(config.translateCone3[0], config.translateCone3[1], config.translateCone3[2]));
        cena.adicionarObjeto(cone3);

        Material matTopo = {{0.24, 0.20, 0.07}, {0.75, 0.60, 0.23}, {0.75, 0.60, 0.23}, 150};

        Esfera *esfera1 = new Esfera(config.especEsfera1, matTopo);
        esfera1->setTransform(Mat4::translate(config.translateEsfera1[0], config.translateEsfera1[1], config.translateEsfera1[2]));
        cena.adicionarObjeto(esfera1);

        // Tronco
        Material matTronco = {{0.15, 0.08, 0.04}, {0.45, 0.25, 0.12}, {0.45, 0.25, 0.12}, 5};

        Cilindro *tronco = new Cilindro(config.especCilindro[0], config.especCilindro[1], matTronco);
        tronco->setTransform(Mat4::translate(config.translateCilindro[0], config.translateCilindro[1], config.translateCilindro[2]));
        cena.adicionarObjeto(tronco);

        // Terra
        Material matTerra = {{0.08, 0.04, 0.02}, {0.35, 0.18, 0.08}, {0.02, 0.02, 0.02}, 3};

        Cubo *cubo1 = new Cubo(config.especCubo1, matTerra);
        cubo1->setTransform(Mat4::translate(config.translateCubo1[0], config.translateCubo1[1], config.translateCubo1[2]) * Mat4::scale(config.scaleCubo1[0], config.scaleCubo1[1], config.scaleCubo1[2]));
        cena.adicionarObjeto(cubo1);

        Cubo *cubo2 = new Cubo(config.especCubo2, matTerra);
        cubo2->setTransform(Mat4::translate(config.translateCubo2[0], config.translateCubo2[1], config.translateCubo2[2]) * Mat4::scale(config.scaleCubo2[0], config.scaleCubo2[1], config.scaleCubo2[2]));
        cena.adicionarObjeto(cubo2);

        // Grama
        Material matGrama = {{0.05, 0.15, 0.05}, {0.1, 0.45, 0.1}, {0.02, 0.02, 0.02}, 5};

        Cubo *cubo3 = new Cubo(config.especCubo3, matGrama);
        cubo3->setTransform(Mat4::translate(config.translateCubo3[0], config.translateCubo3[1], config.translateCubo3[2]) * Mat4::scale(config.scaleCubo3[0], config.scaleCubo3[1], config.scaleCubo3[2]));
        cena.adicionarObjeto(cubo3);

        Cubo *cubo4 = new Cubo(config.especCubo4, matGrama);
        cubo4->setTransform(Mat4::translate(config.translateCubo4[0], config.translateCubo4[1], config.translateCubo4[2]) * Mat4::scale(config.scaleCubo4[0], config.scaleCubo4[1], config.scaleCubo4[2]));
        cena.adicionarObjeto(cubo4);

        //Baú

        Material matBau = {{0.18, 0.12, 0.06}, {0.55, 0.38, 0.18}, {0.15, 0.15, 0.15}, 25};
        
        Cubo *cubo5 = new Cubo(config.especCubo5, matBau);
        cubo5->setTransform(Mat4::translate(config.translateCubo5[0], config.translateCubo5[1], config.translateCubo5[2]));
        cena.adicionarObjeto(cubo5);

        Material matBauSec = {{0.01, 0.01, 0.01}, {0.05, 0.05, 0.05}, {0.05, 0.05, 0.05}, 1};

        Cubo *cubo6 = new Cubo(config.especCubo6, matBauSec);
        cubo6->setTransform(Mat4::translate(config.translateCubo6[0], config.translateCubo6[1], config.translateCubo6[2]) * Mat4::scale(config.scaleCubo6[0], config.scaleCubo6[1], config.scaleCubo6[2]));
        cena.adicionarObjeto(cubo6);

        Material matFechadura = {{0.1, 0.1, 0.1}, {0.4, 0.4, 0.4}, {0.8, 0.8, 0.8}, 150};

        Cubo *cubo7 = new Cubo(config.especCubo7, matFechadura);
        cubo7->setTransform(Mat4::translate(config.translateCubo7[0], config.translateCubo7[1], config.translateCubo7[2]) * Mat4::scale(config.scaleCubo7[0], config.scaleCubo7[1], config.scaleCubo7[2]));
        cena.adicionarObjeto(cubo7);

        //ponte
        Cubo *cubo8 = new Cubo(config.especCubo8, matTronco);

        Vec3 eixoInclinado = Vec3{config.eixoCubo8[0], config.eixoCubo8[1], config.eixoCubo8[2]}.normalize();

        cubo8->setTransform(Mat4::translate(config.translateCubo8[0], config.translateCubo8[1], config.translateCubo8[2])
        * Mat4::rotate(config.rotateCubo8, eixoInclinado)
        * Mat4::scale(config.scaleCubo8[0], config.scaleCubo8[1], config.scaleCubo8[2]));
        cena.adicionarObjeto(cubo8);

        //Tronco e corda
        Cilindro* troncoTorto = new Cilindro(config.especCilindro2[0], config.especCilindro2[1], matTronco);
        troncoTorto->setTransform(Mat4::translate(config.translateCilindro2[0], config.translateCilindro2[1], config.translateCilindro2[2]) 
                                 * Mat4::shear(config.shearCilindro2[0], config.shearCilindro2[1], config.shearCilindro2[2], config.shearCilindro2[3], config.shearCilindro2[4], config.shearCilindro2[5]));
        cena.adicionarObjeto(troncoTorto);

        Material matCorda = {{0.15, 0.12, 0.08}, {0.60, 0.50, 0.35}, {0.0, 0.0, 0.0}, 5};

        Cilindro* corda = new Cilindro(config.especCilindro3[0], config.especCilindro3[1], matCorda);
        corda->setTransform(Mat4::translate(config.translateCilindro3[0], config.translateCilindro3[1], config.translateCilindro3[2]));
        cena.adicionarObjeto(corda);
    }
}

#endif