#ifndef HIDROMETRO_HPP
#define HIDROMETRO_HPP

#include "Entrada.hpp"
#include "Medicao.hpp"

class Hidrometro {
private:
    Entrada input;     // fluxo de água
    Medicao medicao;   // estado da medição

public:
    Hidrometro() = default;
    Hidrometro(const Entrada& in, const Medicao& med = Medicao());

    // V2: simulação contínua
    void simularIninterrupto(float intervaloSegundos = 1.0f);

    // V1 (mantido): medir por um intervalo definido de minutos
    void medir(double minutos = 1.0);

    // apresentação da leitura (terminal e display)
    void apresentacaoMedicao();

    // Getters e setters
    const Medicao& getMedicao() const;
    Medicao& getMedicao();

    const Entrada& getEntrada() const;
    Entrada& getEntrada();
};

#endif
