#include "Hidrometro.h"
#include <iostream>

Hidrometro::Hidrometro() : janelaPrincipal(800, 800, "Simulador de Hidrometro") {
    this->rodando = false;
    this->volumeAcumuladoParaImagem = 0.0;
}

void Hidrometro::iniciar() {
    this->rodando = true;
    this->ultimoTempo = std::chrono::high_resolution_clock::now();
    std::cout << "Simulador de Hidrometro iniciado." << std::endl;
    this->loop();
}

void Hidrometro::parar() {
    this->rodando = false;
    std::cout << "Simulador de Hidrometro parado." << std::endl;
}

void Hidrometro::loop() {
    cimg_library::CImg<unsigned char> imagemBase("base_hidrometro.bmp");
    
    // variável para contar qual imagem ta gerando 
    long long contadorDeLeituras = 1;

    while (this->rodando && !janelaPrincipal.is_closed()) {
        auto tempoAtual = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> tempoDelta = tempoAtual - this->ultimoTempo;
        this->ultimoTempo = tempoAtual;

        // lógica de cálculo de vazão
        double vazao_L_por_s = this->input.vazao / 60.0;
        double volumeNoIntervalo_L = vazao_L_por_s * tempoDelta.count();
        double volumeNoIntervalo_m3 = volumeNoIntervalo_L / 1000.0;
        
        double volumeReal_m3 = 0.0;
        if (this->input.sentido == "Direto") {
            double fatorAr = 1.0 - (this->input.arPercentual / 100.0);
            volumeReal_m3 = volumeNoIntervalo_m3 * fatorAr;
        }

        this->medicao.contabilizar(volumeReal_m3);
        this->volumeAcumuladoParaImagem += volumeReal_m3;

        // lógica de geração de imagem 
        if (this->volumeAcumuladoParaImagem >= this->output.taxaImg_m3) {
            
            Medicao medicaoParaDisplay;
            medicaoParaDisplay.contabilizar((double)contadorDeLeituras * this->output.taxaImg_m3);

            cimg_library::CImg<unsigned char> imagemParaExibir = imagemBase;
            this->display.desenharMostradorJPEG(medicaoParaDisplay, imagemParaExibir);
            janelaPrincipal.display(imagemParaExibir);

            std::cout << "-> Atualizando display para a leitura: " << medicaoParaDisplay.leituraTexto() << std::endl;
            
            this->volumeAcumuladoParaImagem -= this->output.taxaImg_m3;
            
            contadorDeLeituras++;
        }
        
        janelaPrincipal.wait(1);
    }
}