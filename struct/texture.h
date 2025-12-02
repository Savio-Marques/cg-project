#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>
#include "vec3.h"

// Biblioteca para leitura de imagens
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture {
public:
    unsigned char* data;
    int width, height, channels;

    Texture() : data(nullptr), width(0), height(0), channels(0) {}

    // Carrega a imagem do disco
    bool load(const std::string& filepath) {
        data = stbi_load(filepath.c_str(), &width, &height, &channels, 3);
        
        if (!data) {
            std::cout << "ERRO: Falha ao carregar textura: " << filepath << std::endl;
            return false;
        }
        std::cout << "Textura carregada: " << filepath << " (" << width << "x" << height << ")" << std::endl;
        return true;
    }

    // Pega a cor baseada nas coordenadas
    Vec3 getColor(double u, double v) const {
        if (!data) return {1, 0, 1}; // Retorna Rosa se não tiver textura

        u = u - floor(u);
        v = v - floor(v);

        v = 1.0 - v;

        int i = static_cast<int>(u * (width - 1));
        int j = static_cast<int>(v * (height - 1));

        if (i < 0) i = 0;
        if (j < 0) j = 0;
        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        int index = (j * width + i) * 3;

        double r = data[index] / 255.0;
        double g = data[index + 1] / 255.0;
        double b = data[index + 2] / 255.0;

        return {r, g, b};
    }

    ~Texture() {
        if (data) stbi_image_free(data);
    }
};

#endif