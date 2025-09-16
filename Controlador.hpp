#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include <map>
#include <string>
#include <atomic>
#include <thread>
#include <iostream>
#include "Hidrometro.hpp"

class Controlador {
private:
    std::map<std::string, std::string> configuracao;
    Hidrometro h1;
    std::atomic<bool> rodando_;
    std::thread threadInput_;
    
    static std::string trim(const std::string& s);
    void aplicarConfiguracao();
    void threadInputHandler();

public:
    Controlador() : h1(Entrada(), Medicao()), rodando_(true) {}
    ~Controlador();

    void executaIninterrupta(float intervaloSegundos = 1.0f);
    bool carregarConfig(const std::string& arquivo);
    void parar();
};

#endif
