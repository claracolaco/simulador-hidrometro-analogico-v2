#include "Controlador.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <thread>
#include <chrono>
#include <cmath>
#include <iomanip>

using std::string;

static std::string to_lower(std::string s) {
    for (auto &ch : s) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return s;
}

std::string Controlador::trim(const std::string& s) {
    size_t b = 0, e = s.size();
    while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
    while (e > b && std::isspace(static_cast<unsigned char>(s[e-1]))) --e;
    return s.substr(b, e - b);
}

Controlador::~Controlador() {
    parar();
}

bool Controlador::carregarConfig(const std::string& arquivo) {
    std::ifstream in(arquivo);
    if (!in.is_open()) {
        std::cerr << "[ERRO] Não foi possível abrir: " << arquivo << "\n";
        return false;
    }

    configuracao.clear();
    std::string linha;
    while (std::getline(in, linha)) {
        std::string raw = trim(linha);
        if (raw.empty() || raw[0] == '#') continue;

        auto pos = raw.find('=');
        if (pos == std::string::npos) continue;

        std::string chave = trim(raw.substr(0, pos));
        std::string valor = trim(raw.substr(pos + 1));
        if (!chave.empty()) configuracao[chave] = valor;
    }

    aplicarConfiguracao();
    return true;
}

void Controlador::aplicarConfiguracao() {
    // Valores padrão
    float  VAM      = 20.0f;
    float  PCT_AR   = 0.0f;
    float  BITOLA   = 15.0f;
    string SENTIDO  = "Direto";

    // Carrega configurações
    if (auto it = configuracao.find("VAM"); it != configuracao.end()) {
        try { VAM = std::stof(it->second); } catch(...) {}
    }
    if (auto it = configuracao.find("PCT_AR"); it != configuracao.end()) {
        try { PCT_AR = std::stof(it->second); } catch(...) {}
    }
    if (auto it = configuracao.find("BITOLA"); it != configuracao.end()) {
        try { BITOLA = std::stof(it->second); } catch(...) {}
    }
    if (auto it = configuracao.find("SENTIDO"); it != configuracao.end()) {
        SENTIDO = it->second;
    }

    // Configura entrada
    h1.getEntrada().configurar(BITOLA, VAM, SENTIDO, PCT_AR);

    std::cout << "[CONFIG] VAM=" << VAM << " m³/min | PCT_AR=" << PCT_AR << "% | SENTIDO=" << SENTIDO << std::endl;
    std::cout << "[CONFIG] BITOLA=" << BITOLA << " mm | MODO=Terminal Only" << std::endl;
}

void Controlador::threadInputHandler() {
    std::string input;
    while (rodando_.load()) {
        if (std::cin >> input) {
            if (to_lower(input) == "q") {
                std::cout << "\n[INFO] Comando 'q' recebido. Parando sistema..." << std::endl;
                parar();
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Controlador::executaIninterrupta(float intervaloSegundos) {
    std::cout << "\n===========================================" << std::endl;
    std::cout << "    SIMULAÇÃO ININTERRUPTA INICIADA" << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "Sistema rodando continuamente..." << std::endl;
    std::cout << "Para parar: Digite 'q' e pressione Enter" << std::endl;
    std::cout << "===========================================" << std::endl;

    // Converte o intervalo de segundos para minutos (mais preciso)
    const double passoMin = static_cast<double>(intervaloSegundos) / 60.0;
    int contador = 0;
    
    std::cout << "[INICIO] Vazão: " << h1.getEntrada().getVazao() << " m³/min" << std::endl;
    std::cout << "[INICIO] Percentual de ar: " << h1.getEntrada().getArPercentual() << "%" << std::endl;
    std::cout << "[INICIO] Sentido: " << h1.getEntrada().getSentido() << std::endl;
    std::cout << "[INICIO] Intervalo: " << intervaloSegundos << "s (" << passoMin << " min)" << std::endl;

    // Inicia thread para capturar entrada do usuário
    threadInput_ = std::thread(&Controlador::threadInputHandler, this);

    // LOOP PRINCIPAL - VERDADEIRAMENTE INFINITO ATÉ COMANDO 'q'
    while (rodando_.load()) {
        try {
            // Mede o fluxo usando o intervalo correto em minutos
            h1.medir(passoMin);
            
            double volumeAtual = h1.getMedicao().getVolumeTotal_m3();
            int mmAtual = h1.getMedicao().getMM();
            
            // Status a cada iteração (com intervalo de 2s)
            ++contador;
            std::cout << "[" << std::setw(4) << (contador * static_cast<int>(intervaloSegundos)) << "s] Volume: " 
                      << std::fixed << std::setprecision(6) << volumeAtual 
                      << " m³ | Litros: " << std::setw(3) << mmAtual << " L" << std::endl;

            // Pausa conforme intervalo configurado (2 segundos por padrão)
            std::this_thread::sleep_for(std::chrono::milliseconds(
                static_cast<int>(intervaloSegundos * 1000.0f)));

        } catch (const std::exception& e) {
            std::cerr << "[ERRO] " << e.what() << " - Continuando..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        } catch (...) {
            std::cerr << "[ERRO] Exceção desconhecida - Continuando..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    // Aguarda thread de input
    if (threadInput_.joinable()) {
        threadInput_.join();
    }

    std::cout << "\n[INFO] Simulação finalizada." << std::endl;
}

void Controlador::parar() {
    rodando_.store(false);
    
    if (threadInput_.joinable()) {
        threadInput_.join();
    }
}
