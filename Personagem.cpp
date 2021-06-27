#include "Personagem.h"
void Personagem::avancaPosicao(Ponto quantidadeAvanco){
    if(this->nitro){
        quantidadeAvanco = quantidadeAvanco + quantidadeAvanco;
    }
    if(this->combustivel > 0){
        this->Posicao = this->Posicao + quantidadeAvanco;
        // this->combustivel -= 1;
    }
}

void Personagem::calculaDir(){

}