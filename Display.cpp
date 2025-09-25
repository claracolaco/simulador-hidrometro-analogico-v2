#include "Display.h"
#include "Medicao.h"
#include <iostream>


#include "CImg.h"

Display::Display() {
    this->caminhoBaseImagem = "base_hidrometro.bmp";
}

void Display::desenharMostradorJPEG(Medicao& medicao, cimg_library::CImg<unsigned char>& imagem) {
    using namespace cimg_library;

    // pega a leitura 
    std::string leitura = medicao.leituraTexto();

    std::string partePreta = leitura.substr(0, 4);
    std::string parteVermelha = leitura.substr(4, 2);

    // define as cores
    const unsigned char preto[] = { 0, 0, 0 };
    const unsigned char vermelho[] = { 200, 0, 0 };

    // lógica de desenho
    int pos_x_preto = 270;
    int pos_y_preto = 296;
    int fonte_preto = 57;
    int espaco_preto = 44; 

    for (char const& caractere : partePreta) {
        std::string digito_str(1, caractere);
        imagem.draw_text(pos_x_preto, pos_y_preto, digito_str.c_str(), 
                         preto, 0, 1.0f, fonte_preto);
        pos_x_preto += espaco_preto;
    }

    int pos_x_vermelho = 447;
    int pos_y_vermelho = 296;
    int fonte_vermelho = 57;
    int espaco_vermelho = 44;

    for (char const& caractere : parteVermelha) {
        std::string digito_str(1, caractere);
        imagem.draw_text(pos_x_vermelho, pos_y_vermelho, digito_str.c_str(), 
                         vermelho, 0, 1.0f, fonte_vermelho);
        pos_x_vermelho += espaco_vermelho;
    }
}