# 🎨 Engine de Ray Tracing 3D em C++

Um **Motor de Ray Tracing 3D interativo** desenvolvido do zero em C++17 para a disciplina de Computação Gráfica. O projeto combina conceitos avançados da computação gráfica — como equações de interseção analítica, transformações matriciais em coordenadas homogêneas, modelo de iluminação Phong/Blinn-Phong, mapeamento de textura UV, sombras rígidas e seleções por ray picking — com uma interface gráfica em tempo real baseada em **Dear ImGui** e **SDL2**.

---

## 📸 Recursos e Destaques

- 🔺 **Primitivas Geométricas Analíticas:** Suporte completo para Esferas, Cubos/Paralelepípedos, Cilindros, Cones, Planos e Triângulos.
- 🔄 **Transformações Geométricas 3D de Matrizes 4x4:** Translação, Escala, Rotação nos eixos X, Y e Z, Rotação em Eixo Arbitrário e Cisalhamento (*Shear*).
- 💡 **Modelo de Iluminação Phong/Blinn-Phong:** Cálculo de componentes Ambiente ($K_a$), Difusa ($K_d$), Especular ($K_s$), Emissiva ($K_e$) e brilho (*Shininess*).
- 🔦 **Múltiplas Fontes de Luz:**
  - **Luz Ambiente Global**
  - **Luz Pontual (*Point Light*)**
  - **Luz Direcional (*Directional / Sun Light*)**
  - **Luz Focal (*Spot Light*) com ângulo de corte (*Cut-Off*)**
- 🌑 **Sombras Rígidas (*Hard Shadows*):** Lançamento de *Shadow Rays* em direção às fontes de iluminação com verificação de oclusão.
- 🖼️ **Mapeamento de Textura UV:** Carregamento de imagens (PNG, JPG) via `stb_image` com amostragem de cores por coordenadas UV em primitivas.
- 📹 **Sistema de Câmera Flexível:**
  - Matriz de visualização *LookAt* (posição `eye`, foco `at`, vetor `up`).
  - Projeção Perspectiva (com ajuste dinâmico de FOV).
  - Projeção Ortográfica (com controle de escala de abertura *Ortho Size*).
- 🎯 **Ray Picking Interativo:** Clique com o botão esquerdo do mouse diretamente no *viewport* para selecionar e identificar objetos no espaço 3D.
- 🎛️ **Interface de Controle em Tempo Real:** Painel construído com **Dear ImGui** acoplado ao renderizador SDL2.

---

## 🛠️ Tecnologias Utilizadas

| Tecnologia | Descrição |
| :--- | :--- |
| **C++17** | Linguagem principal do projeto, empregando Orientação a Objetos e modelos matemáticos vetoriais. |
| **SDL2** | Gerenciamento de janelas, contexto de renderização 2D do buffer de pixels e tratamento de eventos. |
| **Dear ImGui** | Interface gráfica de usuário (GUI) imediata para alteração de parâmetros da cena em tempo real. |
| **STB Image** | Biblioteca *single-header* para carregamento e decodificação de texturas de imagem. |
| **MinGW-w64 / GCC** | Cadeia de ferramentas de compilação em ambiente Windows. |
| **Make** | Automação da compilação e linkagem do executável através de `Makefile`. |

---

## 📁 Estrutura de Arquivos e Módulos

```text
cg-project/
├── light/                       # Módulo de iluminação e sombras
│   ├── iluminacao.h             # Protótipo das funções de cálculo de luz
│   └── iluminacao.cpp           # Implementação do modelo Phong e Shadow Rays
├── scene/                       # Gerenciamento e construção do cenário
│   ├── scene.h                  # Classe contendo a lista de objetos, luzes e texturas
│   ├── scene_builder.h          # Montagem procedurativa da cena 3D e atribuição de materiais
│   └── settings.h               # Estrutura contendo todas as variáveis e parâmetros ajustáveis
├── shape/                       # Primitivas e superfícies geométricas 3D
│   ├── cilindro.h / .cpp        # Geometria e interseção com Cilindro
│   ├── cone.h / .cpp            # Geometria e interseção com Cone
│   ├── cubo.h / .cpp            # Geometria e interseção com Cubo (Box)
│   ├── esfera.h / .cpp          # Geometria e interseção com Esfera
│   ├── plano.h / .cpp           # Geometria e interseção com Plano + Coordenadas UV
│   └── triangulo.h / .cpp       # Geometria e interseção com Triângulo
├── struct/                      # Estruturas de dados e álgebra linear
│   ├── vec3.h                   # Vetores 3D (operações como produto escalar, vetorial e normalização)
│   ├── mat4.h                   # Matriz 4x4 (Inversa, Transposta, Translação, Rotação, Escala, Shear)
│   ├── ray.h                    # Estrutura de raio 3D: P(t) = O + t * D
│   ├── material.h               # Estrutura de propriedades ópticas do material
│   ├── objeto.h                 # Classe base abstrata para objetos intersectáveis e HitRecord
│   ├── light.h                  # Definições de fontes de iluminação (Point, Directional, Spot)
│   ├── texture.h                # Leitura e amostragem de texturas de imagem
│   └── stb_image.h              # Loader de imagens de terceiros
├── imgui/ & imgui_backends/     # Código-fonte e suporte SDL2/SDLRenderer da biblioteca Dear ImGui
├── main.cpp                     # Ponto de entrada, loop de eventos SDL, lógica de Pick e GUI
├── motor_render.h               # Algoritmo principal de Lançamento de Raios (Ray Tracing Loop)
├── Makefile                     # Script para compilação via g++ / MinGW
├── SDL2.dll                     # Bibliotecas dinâmicas do SDL2 para execução
└── nuvens.png                   # Textura de exemplo aplicada ao plano
```

---

## ⚡ Como Compilar e Executar

### Pré-requisitos
- **MinGW-w64 / g++** instalado e adicionado ao `PATH` do sistema (com suporte ao padrão C++17).
- Ferramenta `make` (ou `mingw32-make`).

### Compilando o Projeto

Abra o terminal na pasta raiz do projeto e execute:

```bash
make
```

Isso gera o executável `raytracer.exe`.

### Executando

Para compilar (se necessário) e iniciar a aplicação imediatamente:

```bash
make run
```

### Limpando os Binários

Para remover o executável e todos os arquivos objeto (`.o`) gerados durante a compilação:

```bash
make clean
```

---

## 🕹️ Guia de Uso e Funcionalidades da Interface

Ao executar a aplicação, a janela exibirá um **Painel de Controle ImGui** à esquerda e o **Viewport do Ray Tracer** ao lado.

### 1. Aba "Visualização"
- **Câmera:** Configure as posições do olho (`Eye`), ponto focal (`At`) e vetor vertical (`Up`).
- **Projeção:** Alterne entre projeção **Perspectiva** (ajustando o `FOV`) ou **Ortográfica** (ajustando o `Tamanho da Lente`).

### 2. Aba "Iluminação"
- Ajuste as cores e posições das fontes de iluminação em tempo real:
  - **Luz Ambiente Global** (cor ambiente de preenchimento).
  - **Luz Pontual (Point Light):** Posição e intensidade de cor.
  - **Luz Direcional (Sun Light):** Vetor de direção do sol e intensidade.
  - **Luz Focal (Spot Light):** Posição, direção de foco e ângulo limite (*Cut-Off*).

### 3. Aba "Cenário"
- Ajuste dimensões, posições, rotações e escalas dos elementos da cena:
  - **Vegetação:** Pinheiros formados por Cones e Cilindro.
  - **Ilha e Grama:** Plataformas compostas por Cubos em camadas.
  - **Objetos e Construções:** Baú, Ponte (com rotação em **Eixo Arbitrário**) e Tronco com **Cisalhamento (Shear)**.
  - **Céu:** Plano com mapeamento de textura de nuvens.

### 4. Ray Picking (Seleção de Objeto via Clique)
- Clique com o **botão esquerdo do mouse** sobre qualquer elemento visualizado na janela de renderização.
- O sistema lançará um raio na posição do cursor e identificará o objeto clicado, exibindo seu **ID** e **Tipo** no painel inferior.

### 5. Botão "RENDERIZAR CENA"
- Após alterar qualquer parâmetro de câmera, luzes ou objetos no ImGui, clique em **RENDERIZAR CENA** para disparar o motor de ray tracing e atualizar o viewport.

---

## 🧮 Fundamentos Teóricos e Algoritmos

### 1. Equação do Raio
Um raio é representado parametricamente por:
$$P(t) = \mathbf{O} + t \cdot \mathbf{D}$$
Onde $\mathbf{O}$ é a origem do raio, $\mathbf{D}$ é o vetor direção normalizado e $t \ge 0$ é o parâmetro de distância ao longo do raio.

### 2. Transformação de Espaço de Objeto (Object Space)
Cada objeto possui uma matriz de transformação $M$. Para testar a interseção de um raio no espaço do objeto:
1. O raio é transformado para o espaço local usando a matriz inversa $M^{-1}$:
   $$\mathbf{O}_{local} = M^{-1} \mathbf{O}_{world}$$
   $$\mathbf{D}_{local} = M^{-1} \mathbf{D}_{world}$$
2. A interseção analítica é calculada no espaço local do objeto.
3. A normal calculada $\mathbf{N}_{local}$ é convertida de volta para o espaço do mundo multiplicando pela transposta da inversa:
   $$\mathbf{N}_{world} = (M^{-1})^T \mathbf{N}_{local}$$

### 3. Modelo de Iluminação Phong
Para cada ponto de interseção que não esteja oculto por sombras, a cor final é calculada por:
$$I = I_a K_a + \sum_{i} \left[ I_{d,i} K_d (\mathbf{N} \cdot \mathbf{L}_i) + I_{s,i} K_s (\mathbf{V} \cdot \mathbf{R}_i)^n \right] + K_e$$
Onde:
- $K_a, K_d, K_s, K_e$: Coeficientes do material (Ambiente, Difusa, Especular, Emissiva).
- $n$: Expoente de especularidade (*Shininess*).
- $\mathbf{N}$: Vetor normal da superfície.
- $\mathbf{L}_i$: Vetor em direção à fonte de luz $i$.
- $\mathbf{V}$: Vetor em direção à câmera.
- $\mathbf{R}_i$: Vetor de reflexão perfeita da luz.

---

## 🎓 Contexto Acadêmico

Este projeto foi desenvolvido como trabalho prático da disciplina de **Computação Gráfica**, com o objetivo de consolidar os conceitos de pipeline gráfico, álgebra linear aplicada, síntese de imagens via ray tracing e desenvolvimento de software gráfico interativo em C++.
