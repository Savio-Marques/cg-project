# Variáveis de Compilação
CXX = g++
CXXFLAGS = -std=c++17 -O2 -D_USE_MATH_DEFINES -DIMGUI_DISABLE_WIN32_FUNCTIONS
INCLUDES = -I. -I./imgui -I./imgui_backends -I./externo/include -I./externo/include/SDL2
LDFLAGS = -L. -lmingw32 -lSDL2main -lSDL2 -lole32 -loleaut32 -limm32 -lwinmm -lgdi32 -lsetupapi -lversion -luuid -static-libgcc -static-libstdc++

# Listagem de todos os arquivos fonte (.cpp)
SOURCES = main.cpp \
          imgui/imgui.cpp struct/texture_impl.o imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui_tables.cpp \
          imgui_backends/imgui_impl_sdl2.cpp imgui_backends/imgui_impl_sdlrenderer2.cpp \
          shape/cilindro.cpp shape/cubo.cpp shape/plano.cpp shape/esfera.cpp \
          shape/cone.cpp shape/triangulo.cpp \
          light/iluminacao.cpp

# Transforma a lista de .cpp em uma lista de .o (objetos)
OBJECTS = $(SOURCES:.cpp=.o)

# Nome do Executável
TARGET = raytracer.exe

# Regra principal
all: $(TARGET)

# Linkagem final: Une todos os .o no .exe
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Regra para compilar cada .cpp em um .o individualmente
# Se o .cpp ou QUALQUER .h mudar, ele recompila esse objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Regra para compilar e abrir automaticamente
run: $(TARGET)
	./$(TARGET)

# Limpeza profunda (apaga o exe e todos os arquivos .o)
clean:
	@if exist $(TARGET) del $(TARGET)
	@del /s /q *.o 2>nul