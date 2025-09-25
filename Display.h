#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include "CImg.h"

class Medicao;

class Display {
public:
    Display();
    
    void desenharMostradorJPEG(Medicao& medicao, cimg_library::CImg<unsigned char>& imagem);

protected:
    std::string caminhoBaseImagem;
};

#endif 