#include "Controlador.h"
#include <iostream>
#include <csignal>


Controlador meuControlador;


void signalHandler(int signum) {
    std::cout << "\nSinal de interrupcao (Ctrl+C) recebido. Encerrando a simulacao..." << std::endl;
    meuControlador.parar();
}

int main() {

    signal(SIGINT, signalHandler);

    meuControlador.carregarConfig("config.txt");

    meuControlador.executar();

    std::cout << "Simulacao encerrada. Ate logo!" << std::endl;

    return 0;
}