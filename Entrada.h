#ifndef ENTRADA_H
#define ENTRADA_H

#include <string>


class Entrada {
public:

    Entrada();

    void configurar(float vazao, std::string sentido, float arPercentual, float bitola);

    float bitola;       
    float vazao;          
    std::string sentido;  
    float arPercentual;   
};

#endif 