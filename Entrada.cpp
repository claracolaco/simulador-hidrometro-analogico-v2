#include "Entrada.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>

static std::string toLower(std::string s) {
    for (auto &ch : s) 
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return s;
}

Entrada::Entrada(float bitola, float vazao, std::string sentido, float arPercentual)
    : bitola(bitola), vazao(vazao), sentido(std::move(sentido)), arPercentual(arPercentual) {}

void Entrada::configurar(float bitola, float vazao, std::string sentido, float arPercentual) {
    this->bitola = bitola;
    this->vazao = vazao;
    this->sentido = std::move(sentido);
    this->arPercentual = arPercentual;
    
    std::cout << "[ENTRADA] Configurado - Vazao: " << this->vazao << " m³/min, Sentido: " 
              << this->sentido << ", AR: " << this->arPercentual << "%" << std::endl;
}

float Entrada::amostrarFluxo(float minutos) const {
    if (minutos <= 0.0f) return 0.0f;

    const std::string s = toLower(sentido);
    if (s == "reverso") return 0.0f;

    // Cálculo mais preciso do volume
    // vazao está em m³/min, minutos é o intervalo de tempo
    double volume = static_cast<double>(vazao) * static_cast<double>(minutos);
    
    // Aplica desconto por passagem de ar (se houver)
    double descontoAr = std::clamp(static_cast<double>(arPercentual) / 100.0, 0.0, 1.0);
    volume *= (1.0 - descontoAr);
    
    // Garante que o volume seja positivo
    return static_cast<float>((volume < 0.0) ? 0.0 : volume);
}

// Getters
float Entrada::getBitola() const { return bitola; }
float Entrada::getVazao() const { return vazao; }
std::string Entrada::getSentido() const { return sentido; }
float Entrada::getArPercentual() const { return arPercentual; }
