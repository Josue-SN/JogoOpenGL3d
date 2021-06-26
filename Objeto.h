//  Objeto.h
//
//

#ifndef Objeto_hpp
#define Objeto_hpp

using namespace std;
#include "AABB.h"
class Objeto {
    
public:
AABB BoundingBox;
Ponto Posicao;
bool destruido;
static int calculaColisaoObjetos(AABB E1, Objeto* ArrayObjetos, int size);
} ;



#endif /* Objeto_hpp */