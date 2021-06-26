//
//  AABB.h
//
//

#ifndef AABB_hpp
#define AABB_hpp

#include "Ponto.h"
using namespace std;

class AABB {
 
public:
Ponto Centro;
Ponto MeiaLarg;
void calculaAABB(Ponto Max, Ponto Min, Ponto Posicao);
bool calculaColisaoAABB(AABB BB1, AABB BB2);
bool calculaColisaoAABB(AABB BB1, AABB* BB2, int size);
} ;



#endif /* AABB_hpp */

