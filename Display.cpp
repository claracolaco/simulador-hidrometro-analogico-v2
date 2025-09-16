#include "Display.hpp"
#include <iostream>

// Construtor
Display::Display(std::string formato, std::string caminhoImagemBase)
    : formato(std::move(formato)), caminhoImagemBase(std::move(caminhoImagemBase)) {}

void Display::desenharMostrador(const Medicao& medicao) const {
    if (formato == "texto") {
        sobreporTexto(medicao.leituraTexto());
    }
    else if (formato == "imagem") {
        sobreporImagem(medicao);
    }
    else {
        std::cerr << "[ERRO] Formato de display desconhecido: " << formato << std::endl;
    }
}

void Display::sobreporTexto(const std::string& texto) const {
    if (!texto.empty()) {
        std::cout << "[DISPLAY - TEXTO] " << texto << std::endl;
    }
}

// ⚠️ Versão inicial: só um placeholder, depois integrará com GDI+ ou outra lib
void Display::sobreporImagem(const Medicao& medicao) const {
    // Aqui vamos simular a saída em imagem
    // Exemplo: no futuro usaremos GDI+ ou OpenCV para abrir a imagem base
    // e escrever os dígitos sobre ela.
    std::cout << "[DISPLAY - IMAGEM] Sobrepondo leitura na imagem base: "
              << caminhoImagemBase << std::endl;
    std::cout << "Leitura atual -> " << medicao.leituraTexto() << std::endl;

    // Exemplo futuro:
    // 1. Abrir a imagem base (hidrômetro_base.png).
    // 2. Escrever números do odômetro (m³ em preto, últimas 2 casas em vermelho).
    // 3. Escrever litros e decilitros nos mostradores extras.
    // 4. Mostrar na tela ou salvar em arquivo .jpeg/.png.
}

std::string Display::getFormato() const {
    return formato;
}

std::string Display::getCaminhoImagemBase() const {
    return caminhoImagemBase;
}
