#include "Medicao.h"
#include <cmath>
#include <sstream>
#include <iomanip>

Medicao::Medicao() {
    this->volumeTotal_m3 = 0.0;
    this->limiteOdometro = 10000; 
}

void Medicao::contabilizar(double volume_m3) {
    this->volumeTotal_m3 += volume_m3;

    if (this->volumeTotal_m3 >= this->limiteOdometro) {
        this->volumeTotal_m3 = fmod(this->volumeTotal_m3, (double)this->limiteOdometro);
    }
}

std::string Medicao::leituraTexto() {
    // separa a parte inteira (m³, dígitos pretos) da parte fracionária
    int metrosCubicos = static_cast<int>(floor(this->volumeTotal_m3));
    double fracao_m3 = this->volumeTotal_m3 - metrosCubicos;


    int litrosDisplay = static_cast<int>(round(fracao_m3 * 100));

    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << metrosCubicos
       << std::setw(2) << std::setfill('0') << litrosDisplay;

    return ss.str();
}