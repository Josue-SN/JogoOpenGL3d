//
//  Personagem.h
//
//

#ifndef Personagem_hpp
#define Personagem_hpp

#include "Ponto.h"
#include "AABB.h"
using namespace std;

class Personagem {
    
public:
AABB BoudingBox;
Ponto Posicao;
// em graus, virando para a esquerda
int direcao;
bool emMovimento = false;
void avancaPosicao(Ponto quantidadeAvanco);
} ;



#endif /* Personagem_hpp */
