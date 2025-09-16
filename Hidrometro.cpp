#include "Hidrometro.hpp"
#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>

Hidrometro::Hidrometro(const Entrada& in, const Medicao& med)
    : input(in), medicao(med) {}

void Hidrometro::medir(double minutos) {
    if (minutos <= 0.0) return;
    
    // Usa precisão dupla para cálculos mais exatos
    double volume = input.amostrarFluxo(static_cast<float>(minutos));
    if (volume > 0.0) {
        medicao.contabilizar(volume);
    }
}

void Hidrometro::apresentacaoMedicao() {
    // Apresentação simples no terminal
    std::cout << "[HIDROMETRO] " << medicao.leituraTexto() << std::endl;
}

// Getters/Setters
const Medicao& Hidrometro::getMedicao() const { return medicao; }
Medicao& Hidrometro::getMedicao() { return medicao; }

const Entrada& Hidrometro::getEntrada() const { return input; }
Entrada& Hidrometro::getEntrada() { return input; }
