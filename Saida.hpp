#ifndef SAIDA_HPP
#define SAIDA_HPP

#include <string>
#include "Medicao.hpp"
#include "Display.hpp"

class Saida {
private:
    std::string caminhoArquivo_;
    std::string saidaDir_;
    std::string baseImagem_;
    float taxaImg_m3_;
    bool salvarTexto_;
    bool salvarImagem_;
    std::string suapId_;
    mutable int contadorImagem_;

public:
    Saida(std::string caminho = "saida.txt", float taxa = 1.0f)
        : caminhoArquivo_(std::move(caminho)), saidaDir_("./imagens"), 
          taxaImg_m3_(taxa), salvarTexto_(true), salvarImagem_(false),
          suapId_("000000000000"), contadorImagem_(0) {}

    void configurarDiretorio(const std::string& prefixo, const std::string& suapId);
    void salvarSequencialM3(const Medicao& m, const Display& d);

    // Getters
    std::string getCaminhoArquivo_() const { return caminhoArquivo_; }
    float getTaxaImg_m3_() const { return taxaImg_m3_; }

    // Setters
    void setCaminhoArquivo_(const std::string& caminho) { caminhoArquivo_ = caminho; }
    void setTaxaImg_m3_(float taxa) { taxaImg_m3_ = taxa; }
    void setSalvarTexto_(bool v) { salvarTexto_ = v; }
    void setSalvarImagem_(bool v) { salvarImagem_ = v; }
    void setBaseImagem_(const std::string& p) { baseImagem_ = p; }
    void setSaidaDir_(const std::string& d) { saidaDir_ = d; }
    void setSuapId_(const std::string& s) { suapId_ = s; }

private:
    void salvarJPEG(const Medicao& m, const std::string& caminhoCompleto) const;
};

#endif
