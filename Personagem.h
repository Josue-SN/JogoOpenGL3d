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
Ponto Dir;
bool nitro;
// em graus, virando para a esquerda
int direcao;
int direcaoCamera;
int combustivel;
bool emMovimento = false;
void avancaPosicao(Ponto quantidadeAvanco);
void calculaDir();
} ;



#endif /* Personagem_hpp */
