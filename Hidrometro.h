#ifndef HIDROMETRO_H
#define HIDROMETRO_H

#include "Entrada.h"
#include "Saida.h"
#include "Display.h"
#include "Medicao.h"
#include <chrono>
#include "CImg.h" 

class Hidrometro {
public:
    Hidrometro();
    void iniciar();
    void parar();

    Entrada input;
    Saida output;
    Display display;
    Medicao medicao;

protected:
    void loop();

    bool rodando;
    double volumeAcumuladoParaImagem;
    std::chrono::time_point<std::chrono::high_resolution_clock> ultimoTempo;
    
    cimg_library::CImgDisplay janelaPrincipal;
};

#endif 