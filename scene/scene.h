#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "struct/objeto.h"
#include "struct/light.h"
#include "struct/texture.h" 

class Scene {
public:
    std::vector<Objeto*> objetos;
    std::vector<Light> luzes;
    std::vector<Texture*> texturas;
    Vec3 corAmbiente = {0.1f, 0.1f, 0.1f};

    // Limpa a memória para evitar vazamentos ao trocar de cena
    void limpar() {
        for (auto obj : objetos) {
            delete obj;
        }
        objetos.clear();
        luzes.clear();
    }

    void adicionarObjeto(Objeto* obj) {
        objetos.push_back(obj);
    }

    void adicionarLuz(Light luz) {
        luzes.push_back(luz);
    }

    ~Scene() { limpar(); }
};

#endif