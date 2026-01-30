#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui_backends/imgui_impl_sdl2.h"
#include "imgui_backends/imgui_impl_sdlrenderer2.h"

#include "scene/scene_builder.h"
#include "motor_render.h"

Scene cenaAtiva;
int objetoSelecionadoID = -1; // Armazena o ID do objeto clicado

// Função auxiliar para o Pick
int realizarPick(int mx, int my, int screenW, int screenH, const SceneSettings &config, const Scene &cena)
{
    // Posiçãdo do mouse em pixels (coordenadas normalizadas)
    double nx = (2.0 * (mx + 0.5) / screenW - 1.0);
    double ny = (1.0 - 2.0 * (my + 0.5) / screenH);

    Vec3 eye = {config.lookfrom[0], config.lookfrom[1], config.lookfrom[2]};
    Vec3 at = {config.lookat[0], config.lookat[1], config.lookat[2]};
    Vec3 up = {config.lookup[0], config.lookup[1], config.lookup[2]};
    Mat4 camToWorld = Mat4::lookAt(eye, at, up).inverse();

    Ray raio;
    if (config.tipoProjecao == 0)
    {
        // Perspectiva
        float aspect = (float)screenW / screenH;
        float theta = config.fov * M_PI / 180.0f;
        // 'h' representa a abertura da janela, quando o FOV diminui 'h' diminui.
        // equivalente a aumentar a distância 'd'.
        float h = tan(theta / 2.0f);
        // A direção do raio combina nx/ny com 'h' e aponta para a profundidade fixa -1.0
        raio = {eye, camToWorld.vector(Vec3{nx * h * aspect, ny * h, -1.0}).normalize()};
    }
    else
    {
        // Ortografica
        // Distância 'd' é infinita, muda 's' tamanho da janela.
        float s = config.orthoSize;
        float aspect = (float)screenW / screenH;
        Vec3 direcaoFrente = (at - eye).normalize();
        // O raio nasce na janela redimensionada (nx * s) e segue reto
        raio = {eye + camToWorld.vector(Vec3{nx * s * aspect, ny * s, 0.0}), direcaoFrente};
    }

    // Teste de intersecção
    int hitID = -1;
    double t_min = 1e30;
    HitRecord rec;
    for (int i = 0; i < (int)cena.objetos.size(); i++)
    {
        if (cena.objetos[i]->intersecta(raio, 0.001, t_min, rec))
        {
            t_min = rec.t;
            hitID = i;
        }
    }
    return hitID;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Ray Tracer - Funcao Pick", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    SceneSettings renderSettings;
    std::vector<unsigned char> pixelBuffer(renderSettings.nCol * renderSettings.nLin * 3, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, renderSettings.nCol, renderSettings.nLin);

    SDL_Rect destRect = {350, 100, 600, 600};
    bool running = true;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                running = false;

            // Lógica do pick ao clicar
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                int mx = event.button.x;
                int my = event.button.y;

                // Verifica se o clique foi dentro da imagem
                if (mx >= destRect.x && mx <= destRect.x + destRect.w &&
                    my >= destRect.y && my <= destRect.y + destRect.h)
                {

                    int relX = mx - destRect.x;
                    int relY = my - destRect.y;
                    objetoSelecionadoID = realizarPick(relX, relY, destRect.w, destRect.h, renderSettings, cenaAtiva);
                }
            }
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Painel de Controle");

        // Barra de Abas Principal
        if (ImGui::BeginTabBar("MenuPrincipal"))
        {
            // (Câmera e Lente)
            if (ImGui::BeginTabItem("Visualização"))
            {
                if (ImGui::CollapsingHeader("Câmera", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::InputFloat3("Posição (Eye)", renderSettings.lookfrom);
                    ImGui::InputFloat3("Foco (At)", renderSettings.lookat);
                    ImGui::InputFloat3("Cima (Up)", renderSettings.lookup);
                }

                if (ImGui::CollapsingHeader("Projeção e Lente", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::RadioButton("Perspectiva", &renderSettings.tipoProjecao, 0);
                    ImGui::SameLine();
                    ImGui::RadioButton("Ortográfica", &renderSettings.tipoProjecao, 1);

                    if (renderSettings.tipoProjecao == 0)
                        ImGui::SliderFloat("FOV", &renderSettings.fov, 10.0f, 120.0f);
                    else
                        ImGui::InputFloat("Tamanho Lente", &renderSettings.orthoSize, 0.5f);
                }

                ImGui::EndTabItem();
            }

            // (Phong e Sombras)
            if (ImGui::BeginTabItem("Iluminação"))
            {
                if (ImGui::CollapsingHeader("Configurações Globais", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::ColorEdit3("Luz Ambiente", renderSettings.corAmbiente);
                }

                if (ImGui::CollapsingHeader("Luz: Point"))
                {
                    ImGui::InputFloat3("Posicão ##Point", renderSettings.lightPointPos);
                    ImGui::ColorEdit3("Intensidade ##Point", renderSettings.lightPointIntensity);
                }

                if (ImGui::CollapsingHeader("Luz: Direcional"))
                {
                    ImGui::InputFloat3("Direção ##Direcional", renderSettings.lightSunDirection);
                    ImGui::ColorEdit3("Intensidade ##Direcional", renderSettings.lightSunIntensity);
                }

                if (ImGui::CollapsingHeader("Luz: Spot"))
                {
                    ImGui::InputFloat3("Posição ##Spot", renderSettings.lightSpotPos);
                    ImGui::InputFloat3("Direção ##Spot", renderSettings.lightSpotDirection);
                    ImGui::SliderFloat("Ângulo ##Spot", &renderSettings.lightSpotCutOff, 0.0f, 90.0f);
                }
                ImGui::EndTabItem();
            }

            // (Todos os Objetos)
            if (ImGui::BeginTabItem("Cenário"))
            {
                if (ImGui::TreeNode("Vegetação"))
                {
                    if (ImGui::CollapsingHeader("Pinheiro (Cone/Cilindro)"))
                    {
                        ImGui::InputFloat2("Raio/Alt Folhas", renderSettings.especCone);
                        ImGui::InputFloat3("Posição Folha 1", renderSettings.translateCone1);
                        ImGui::InputFloat3("Posição Folha 2", renderSettings.translateCone2);
                        ImGui::InputFloat3("Posição Folha 3", renderSettings.translateCone3);
                        ImGui::Separator();
                        ImGui::InputFloat3("Material Ka", renderSettings.matFolhaKa);
                        ImGui::InputFloat3("Material Kd", renderSettings.matFolhaKd);
                        ImGui::InputFloat3("Material Ke", renderSettings.matFolhaKe);
                        ImGui::InputFloat("Material Shiness", &renderSettings.matFolhaShiness);
                        ImGui::Separator();
                        ImGui::InputFloat2("Raio/Alt Tronco", renderSettings.especCilindro);
                        ImGui::InputFloat3("Posição Tronco", renderSettings.translateCilindro);
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Ilha e Grama"))
                {
                    if (ImGui::CollapsingHeader("Plataforma 1"))
                    {
                        ImGui::InputFloat("Aresta Terra 1", &renderSettings.especCubo1);
                        ImGui::InputFloat3("Posição Terra 1", renderSettings.translateCubo1);
                        ImGui::InputFloat3("Escala Terra 1", renderSettings.scaleCubo1);
                        ImGui::Separator();
                        ImGui::InputFloat3("Posição Grama 1", renderSettings.translateCubo3);
                        ImGui::InputFloat3("Escala Grama 1", renderSettings.scaleCubo3);
                    }
                    if (ImGui::CollapsingHeader("Plataforma 2"))
                    {
                        ImGui::InputFloat("Aresta Terra 2", &renderSettings.especCubo2);
                        ImGui::InputFloat3("Posição Terra 2", renderSettings.translateCubo2);
                        ImGui::InputFloat3("Escala Terra 2", renderSettings.scaleCubo2);
                        ImGui::Separator();
                        ImGui::InputFloat3("Posição Grama 2", renderSettings.translateCubo4);
                        ImGui::InputFloat3("Escala Grama 2", renderSettings.scaleCubo4);
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Objetos de Cena"))
                {
                    if (ImGui::CollapsingHeader("Baú"))
                    {
                        ImGui::InputFloat3("Posição Caixa", renderSettings.translateCubo5);
                        ImGui::InputFloat("Aresta Caixa", &renderSettings.especCubo5);
                        ImGui::Separator();
                        ImGui::InputFloat3("Posição Tampa", renderSettings.translateCubo6);
                        ImGui::InputFloat("Aresta Tampa", &renderSettings.especCubo6);
                        ImGui::InputFloat3("Escala Tampa", renderSettings.scaleCubo6);
                        ImGui::Separator();
                        ImGui::InputFloat3("Posição Tranca", renderSettings.translateCubo7);
                        ImGui::InputFloat("Aresta Tranca", &renderSettings.especCubo7);
                        ImGui::InputFloat3("Escala Tranca", renderSettings.scaleCubo7);
                    }
                    if (ImGui::CollapsingHeader("Ponte (Eixo Arbitrário)"))
                    {
                        ImGui::InputFloat3("Translação", renderSettings.translateCubo8);
                        ImGui::SliderFloat("Rotação (Graus)", &renderSettings.rotateCubo8, -180.0f, 180.0f);
                        ImGui::InputFloat3("Eixo de Rotação", renderSettings.eixoCubo8);
                        ImGui::InputFloat3("Escala", renderSettings.scaleCubo8);
                    }
                    if (ImGui::CollapsingHeader("Tronco/Corda (Shear)"))
                    {
                        ImGui::InputFloat2("Dimensões Tronco", renderSettings.especCilindro2);
                        ImGui::InputFloat3("Posição Tronco", renderSettings.translateCilindro2);
                        ImGui::InputFloat3("Cisalhamento A", &renderSettings.shearCilindro2[0]);
                        ImGui::InputFloat3("Cisalhamento B", &renderSettings.shearCilindro2[3]);
                        ImGui::Separator();
                        ImGui::InputFloat2("Dimensões Corda", renderSettings.especCilindro3);
                        ImGui::InputFloat3("Posição Corda", renderSettings.translateCilindro3);
                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Céu"))
                {
                    if (ImGui::CollapsingHeader("Plano (Rotação em eixo X)"))
                    {
                        ImGui::InputFloat3("Posição Céu", renderSettings.translatePlano1);
                        ImGui::InputFloat("Rotação Céu", &renderSettings.rotatePlano1);
                    }
                    ImGui::TreePop();
                }
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        // (Seleção e Botão)
        ImGui::Separator();
        if (objetoSelecionadoID != -1)
        {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Objeto Ativo: %s (ID: %d)",
                               cenaAtiva.objetos[objetoSelecionadoID]->getTipo().c_str(), objetoSelecionadoID);
        }
        else
        {
            ImGui::Text("Nenhum objeto selecionado (Use o Pick)");
        }

        if (ImGui::Button("RENDERIZAR CENA", ImVec2(-1, 50)))
        {
            SceneBuilder::montarCenaEsfera(cenaAtiva, renderSettings);
            executarRayTracing(pixelBuffer, renderSettings, cenaAtiva);
            SDL_UpdateTexture(texture, NULL, pixelBuffer.data(), renderSettings.nCol * 3);
        }

        ImGui::End();

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    cenaAtiva.limpar();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}