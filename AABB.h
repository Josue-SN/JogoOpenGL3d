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
} ;



#endif /* AABB_hpp */

