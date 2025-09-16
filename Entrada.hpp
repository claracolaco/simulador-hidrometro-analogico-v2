#ifndef ENTRADA_HPP
#define ENTRADA_HPP

#include <string>

class Entrada {
private:
    float bitola;        // largura do cano (opcional para cálculos futuros)
    float vazao;         // volume de água em m³/min (considerando entrada real)
    std::string sentido; // "Direto" ou "Reverso"
    float arPercentual;  // % de ar presente (0..100)

public:
    // Construtor
    Entrada(float bitola = 0.0f, float vazao = 0.0f,
            std::string sentido = "Direto", float arPercentual = 0.0f);

    // Configura os parâmetros da entrada
    void configurar(float bitola, float vazao, std::string sentido, float arPercentual);

    // Retorna o volume que passou no intervalo de tempo (em minutos)
    // Exemplo: minutos = 0.25/60 -> intervalo de 0.25 segundos
    float amostrarFluxo(float minutos) const;

    // Getters
    float getBitola() const;
    float getVazao() const;
    std::string getSentido() const;
    float getArPercentual() const;
};

#endif
