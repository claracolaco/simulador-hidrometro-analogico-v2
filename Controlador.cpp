#include "Controlador.h"
#include <iostream>
#include <fstream>   
#include <sstream>   

Controlador::Controlador() {
    // o construtor está vazio pois h1 é inicializado por padrão
}

void Controlador::carregarConfig(const std::string& arquivo) {
    std::ifstream configFile(arquivo);
    if (!configFile.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo de configuracao: " << arquivo << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(configFile, linha)) {
        std::stringstream ss(linha);
        std::string chave, valor;

        // pega a parte antes do ':' como a chave
        if (std::getline(ss, chave, ':')) {
            // pega o resto da linha como valor
            if (std::getline(ss, valor)) {
                // armazena no map
                this->configuracao[chave] = valor;
            }
        }
    }

    configFile.close();
    std::cout << "Configuracao carregada com sucesso." << std::endl;
    aplicarConfig();
}

void Controlador::aplicarConfig() {
    try {
        // tenta ler cada configuração do map e converter para o tipo certo
        float vazao = std::stof(this->configuracao.at("VAM"));
        float ar = std::stof(this->configuracao.at("PCT_AR"));
        float taxaImg = std::stof(this->configuracao.at("TAXA_IMG"));
        std::string caminho = this->configuracao.at("CAMINHO");

        // configura os objetos dentro do hidrômetro
        this->h1.input.configurar(vazao, "Direto", ar, 0.0f); 
        this->h1.output.configurar(caminho, taxaImg);

        std::cout << "Configuracao aplicada ao simulador." << std::endl;

    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: Uma ou mais chaves de configuracao estao faltando no arquivo config.txt" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Erro: Valor invalido para uma das configuracoes no arquivo config.txt" << std::endl;
    }
}

void Controlador::executar() {
    this->h1.iniciar();
}

void Controlador::parar() {
    this->h1.parar();
}