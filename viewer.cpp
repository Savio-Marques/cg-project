// Este código é para SDL2

#include <SDL2/SDL.h> // MUDANÇA: Incluindo SDL2
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

bool carregarPPM(const std::string& caminho, std::vector<unsigned char>& pixels, int& largura, int& altura) {
    std::ifstream f(caminho);
    if (!f.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << caminho << std::endl;
        return false;
    }
    std::string magicNumber;
    f >> magicNumber;
    if (magicNumber != "P3") {
        std::cerr << "Erro: Formato de PPM invalido. Esperado 'P3'." << std::endl;
        return false;
    }
    f >> largura >> altura;
    int maxVal;
    f >> maxVal; 
    pixels.resize(largura * altura * 3);
    int r, g, b;
    for (int i = 0; i < largura * altura; ++i) {
        if (!(f >> r >> g >> b)) {
            std::cerr << "Erro: Falha ao ler dados de pixel." << std::endl;
            return false;
        }
        pixels[i * 3 + 0] = static_cast<unsigned char>(r); // R
        pixels[i * 3 + 1] = static_cast<unsigned char>(g); // G
        pixels[i * 3 + 2] = static_cast<unsigned char>(b); // B
    }
    f.close();
    std::cout << "PPM carregado com sucesso (" << largura << "x" << altura << ")." << std::endl;
    return true;
}

int main(int argc, char* argv[]) {
    std::vector<unsigned char> pixels;
    int largura = 0, altura = 0;
    if (!carregarPPM("cena.ppm", pixels, largura, altura)) {
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // Agora o SDL_GetError() VAI funcionar
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // MUDANÇA: Criando a janela no estilo SDL2 (com posição)
    SDL_Window* window = SDL_CreateWindow("Visualizador PPM",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          largura, altura,
                                          0);
    if (!window) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // MUDANÇA: Usando a flag original do SDL2
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // Se a aceleração falhar, tente o "software" (Plano B do SDL2)
    if (!renderer) {
        std::cout << "Aviso: Renderer acelerado falhou. Tentando software." << std::endl;
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    }

    if (!renderer) {
        std::cerr << "Erro ao criar renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_RGB24,
                                             SDL_TEXTUREACCESS_STATIC,
                                             largura, altura);
    if (!texture) {
        std::cerr << "Erro ao criar textura: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_UpdateTexture(texture, NULL, pixels.data(), largura * 3);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            // MUDANÇA: Usando o evento de Sair do SDL2
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_RenderClear(renderer);
        // MUDANÇA: Usando a função de cópia do SDL2
        SDL_RenderCopy(renderer, texture, NULL, NULL); 
        SDL_RenderPresent(renderer);

        SDL_Delay(16); 
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}