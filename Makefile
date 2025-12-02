all:
	g++ main.cpp shape/*.cpp light/*.cpp -o renderizador
	./renderizador
	./viewer