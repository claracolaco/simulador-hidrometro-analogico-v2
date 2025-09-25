#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "Hidrometro.h"
#include <string>
#include <map>


class Controlador {
public:

    Controlador();


    void carregarConfig(const std::string& arquivo);


    void executar();


    void parar();

protected:

    void aplicarConfig();

    Hidrometro h1; 
    std::map<std::string, std::string> configuracao; // armazena as chaves/valores do config.txt
};

#endif 