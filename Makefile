all:
	g++ main.cpp shape/*.cpp -o renderizador
	./renderizador
	./viewer