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
int Direcao;
void avancaPosicao(Ponto quantidadeAvanco);
} ;



#endif /* Personagem_hpp */
