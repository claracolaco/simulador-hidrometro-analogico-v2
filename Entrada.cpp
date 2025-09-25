#include "Entrada.h"

Entrada::Entrada() {
    this->bitola = 0.0f;
    this->vazao = 0.0f;
    this->sentido = "Direto";
    this->arPercentual = 0.0f;
}

void Entrada::configurar(float vazao, std::string sentido, float arPercentual, float bitola) {
    this->vazao = vazao;
    this->sentido = sentido;
    this->arPercentual = arPercentual;
    this->bitola = bitola;
}