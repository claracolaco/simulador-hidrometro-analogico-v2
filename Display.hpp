#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <string>
#include "Medicao.hpp"

class Display {
private:
    std::string formato;    // "texto" ou "imagem"
    std::string caminhoImagemBase; // arquivo base do hidrômetro (PNG/JPEG)

public:
    // Construtor
    Display(std::string formato = "texto", std::string caminhoImagemBase = "hidrômetro_base.png");

    // Exibe o mostrador (dependendo do formato escolhido)
    void desenharMostrador(const Medicao& medicao) const;

    // Saída em texto puro (para depuração ou fallback)
    void sobreporTexto(const std::string& texto) const;

    // Saída gráfica: sobrepõe os dígitos na imagem base
    void sobreporImagem(const Medicao& medicao) const;

    // Getters
    std::string getFormato() const;
    std::string getCaminhoImagemBase() const;
};

#endif
