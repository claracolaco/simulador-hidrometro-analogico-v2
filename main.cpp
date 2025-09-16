#include "Controlador.hpp"
#include <iostream>

int main() {
    std::cout << "==============================\n";
    std::cout << "Simulador de Hidrômetro v2.0\n";
    std::cout << "==============================\n\n";
    
    try {
        Controlador ctrl;
        if (ctrl.carregarConfig("config.txt")) {
            std::cout << "Configuração carregada com sucesso!\n\n";
            ctrl.executaIninterrupta(2.0f);
        } else {
            std::cerr << "Erro ao carregar configuração!\n";
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[ERRO FATAL] " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
