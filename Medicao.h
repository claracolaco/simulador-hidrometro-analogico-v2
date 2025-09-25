#ifndef MEDICAO_H
#define MEDICAO_H

#include <string>

class Medicao {
public:

    Medicao();

    void contabilizar(double volume_m3);

    std::string leituraTexto(); 

protected:
    double volumeTotal_m3;   
    int limiteOdometro;      
};

#endif 