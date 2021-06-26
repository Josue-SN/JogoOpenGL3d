#include "Personagem.h"
void Personagem::avancaPosicao(Ponto quantidadeAvanco){
    if(this->combustivel > 0){
        this->Posicao = this->Posicao + quantidadeAvanco;
        this->combustivel -= 1;
    }
}