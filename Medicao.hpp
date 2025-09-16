#ifndef MEDICAO_HPP
#define MEDICAO_HPP

#include <string>

class Medicao {
public:
    double volumeTotal_m3;   // volume acumulado em m³ (pode ter frações)
    int mm;                  // milímetros cúbicos adicionais (ou subunidade opcional)
    int limiteOdometro;      // limite máximo do odômetro (ex.: 99999)

    // Construtor (valor padrão para limiteOdometro)
    Medicao(int limite = 99999);

    // Acumula mais volume (incremento em m³)
    void contabilizar(double volume_m3);

    // Retorna uma string com a leitura formatada (para logs no terminal)
    std::string leituraTexto() const;

    // Getters de conveniência
    double getVolumeTotal_m3() const { return volumeTotal_m3; }
    int getMM() const { return mm; }
};

#endif
