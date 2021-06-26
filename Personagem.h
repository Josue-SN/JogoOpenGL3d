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
AABB BoundingBox;
Ponto Posicao;
// em graus, virando para a esquerda
int direcao;
int combustivel;
bool emMovimento = false;
void avancaPosicao(Ponto quantidadeAvanco);
} ;



#endif /* Personagem_hpp */
