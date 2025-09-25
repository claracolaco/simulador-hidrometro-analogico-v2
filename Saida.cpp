#include "Saida.h"
#include "Display.h"  
#include "Medicao.h"  
#include <iostream>

Saida::Saida() {
    this->caminhoImagem = "output/hidrometro.jpg";
    this->taxaImg_m3 = 1.0f; // por padrão gera uma imagem a cada 1 m³
}

void Saida::configurar(const std::string& caminho, float taxa) {
    this->caminhoImagem = caminho;
    this->taxaImg_m3 = taxa;
}

void Saida::gerarJPEG(Medicao& m, Display& d) {

}
  