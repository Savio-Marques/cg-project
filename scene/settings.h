#ifndef SETTINGS_H
#define SETTINGS_H

#include "struct/vec3.h"

struct SceneSettings {
    // Resolução
    int nCol = 500;
    int nLin = 500;
    
    // Luz Point
    float lightPointPos[3] = {220.0f, 190.0f, 400.0f};
    float lightPointIntensity[3] = {1.0f, 1.0f, 1.0f};

    // Luz Direcional
    float lightSunIntensity[3] = {0.7f, 0.7f, 0.7f};
    float lightSunDirection[3] = {-1.0f, -1.0f, -1.0f};
    
    //Luz Spot
    float lightSpotPos[3] = {60.0f, 200.0f, 160.0f};
    float lightSpotDirection[3] = {0.0f, -1.0f, 0.0f};
    float lightSpotIntensity[3] = {0.8f, 0.8f, 0.8f};
    float lightSpotCutOff = 10.0f;
    
    float corAmbiente[3] = {0.1f, 0.1f, 0.1f};

    // Câmera
    float lookfrom[3] = {220.0f, 190.0f, 350.0f};
    float lookat[3] = {40.0f, 70.0f, 70.0f};
    float lookup[3] = {0.0f, 1.0f, 0.0f};

    float fov = 45.0f;           // Para Perspectiva (em graus)
    float orthoSize = 100.0f;     // Para Ortográfica (tamanho da janela)
    
    int tipoProjecao = 0; // 0: Perspectiva, 1: Ortográfica

    //Tronco arvore (Cilindro)
    float especCilindro[2] = {10.0f, 80.0f};

    float translateCilindro[3] = {110.0f, 70.0f, 70.0f};

    //Folhas (Cones)
    float especCone[2] = {30.0f, 45.0f};
    
    float translateCone1[3] = {110.0f, 90.0f, 70.0f};
    float translateCone2[3] = {110.0f, 105.0f, 70.0f};
    float translateCone3[3] = {110.0f, 120.0f, 70.0f};

            // Material matFolha = {{0.01, 0.04, 0.02}, {0.05, 0.22, 0.08}, {0.0, 0.0, 0.0}, 1};


    float matFolhaKa[3] ={0.01f, 0.04f, 0.02f};
    float matFolhaKd[3] ={0.05f, 0.22f, 0.08f};
    float matFolhaKe[3] ={0.0f, 0.0f, 0.0f};

    float matFolhaShiness = 1.0f;

    //Chão

    float especCubo1 = 15.0f;

    float translateCubo1[3] = {55.0f, 52.0f, 120.0f};
    float scaleCubo1[3] = {4.0f, 2.5f, 8.0f};

    float especCubo2 = 15.0f;

    float translateCubo2[3] = {107.0f, 52.0f, 85.0f};
    float scaleCubo2[3] = {3.0f, 2.5f, 3.5f};

    //Grama

    float especCubo3 = 15.0f;

    float translateCubo3[3] = {55.0f, 70.0f, 120.0f};
    float scaleCubo3[3] = {4.0f, 0.2f, 8.0f};

    float especCubo4 = 15.0f;

    float translateCubo4[3] = {107.0f, 70.0f, 85.0f};
    float scaleCubo4[3] = {3.0f, 0.2f, 3.5f};

    //Baú

    float especCubo5 = 11.0f;

    float translateCubo5[3] = {60.0f, 77.0f, 160.0f};

    float especCubo6 = 10.0f;

    float translateCubo6[3] = {60.0f, 79.0f, 160.0f};
    float scaleCubo6[3] = {1.1f, 0.1f, 1.1f};

    float especCubo7 = 10.0f;

    float translateCubo7[3] = {66.0f, 79.0f, 160.0f};
    float scaleCubo7[3] = {0.15f, 0.45f, 0.15f};

    //planos
    float translatePlano1[3] = {0.0f, 0.0f, 0.0f};

    float rotatePlano1 =90.0f;

    //Esfera
    float especEsfera1 = 5.0f;

    float translateEsfera1[3] = {110.0f, 165.0f, 70.0f};

    //Ponte
    float especCubo8 = 10.0f;

    float translateCubo8[3] = {102.0f, 72.0f, 130.0f};
    float scaleCubo8[3] = {6.0f, 0.1f, 1.0f};
    float eixoCubo8[3] = {0.0f, 1.0f, 0.0f};
    float rotateCubo8 = 45.0f;

    //Tronco torto

    float especCilindro2[2] = {3.0f, 50.0f};
    float translateCilindro2[3] = {45.0f, 70.0f, 170.0f};
    float shearCilindro2[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f};

    float especCilindro3[2] = {0.5f, 100.0f};
    float translateCilindro3[3] = {45.0f, 20.0f, 195.0f};
};

#endif