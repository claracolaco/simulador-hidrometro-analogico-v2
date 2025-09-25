#ifndef SAIDA_H
#define SAIDA_H

#include <string>

class Medicao;
class Display;

class Saida {
public:

    Saida();

    void configurar(const std::string& caminho, float taxa);

    void gerarJPEG(Medicao& m, Display& d); 

    std::string caminhoImagem; 
    float taxaImg_m3;         
};

#endif 