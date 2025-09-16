#include "Medicao.hpp"
#include <iomanip>
#include <sstream>
#include <cmath>
#include <iostream>

Medicao::Medicao(int limite) {
    volumeTotal_m3 = 0.0;
    mm = 0;
    limiteOdometro = limite;
}

void Medicao::contabilizar(double volume_m3) {
    // soma o incremento de volume com maior precisão
    volumeTotal_m3 += volume_m3;

    // Aplica limite do odômetro se necessário
    if (volumeTotal_m3 >= limiteOdometro) {
        volumeTotal_m3 = fmod(volumeTotal_m3, limiteOdometro);
    }

    // Cálculo preciso dos litros (mm)
    // 1 m³ = 1000 litros
    // Converte o volume total para litros e pega apenas a parte fracionária dos m³
    double volumeEmLitros = volumeTotal_m3 * 1000.0;
    double parteInteira_m3;
    double parteFracionaria_m3 = modf(volumeTotal_m3, &parteInteira_m3);
    
    // mm representa os litros da parte fracionária (0-999 litros)
    mm = static_cast<int>(round(parteFracionaria_m3 * 1000.0)) % 1000;
    
    // Garante que mm esteja no intervalo correto
    if (mm < 0) mm = 0;
    if (mm >= 1000) mm = 999;

    // Debug ocasional para verificar cálculos (a cada 150 iterações = 5 minutos)
    static int debugCount = 0;
    if (++debugCount % 150 == 0) {
        std::cout << "[DEBUG] " << std::fixed << std::setprecision(6) << volumeTotal_m3 
                  << " m³ total | " << mm << " L fracionários | " 
                  << static_cast<int>(volumeTotal_m3) << " m³ inteiros" << std::endl;
    }
}

std::string Medicao::leituraTexto() const {
    std::ostringstream oss;

    // Exibe a leitura formatada para log no terminal
    // Odômetro de até 6 dígitos (inclui os litros na mesma linha, útil p/ depuração)
    oss << std::setw(6) << std::setfill('0') << static_cast<int>(volumeTotal_m3)
        << " m³ | "
        << std::setw(3) << std::setfill('0') << mm
        << " L";

    return oss.str();
}
