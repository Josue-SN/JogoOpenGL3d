#include "AABB.h"
#include "Objeto.h"

void AABB::calculaAABB(Ponto Max, Ponto Min, Ponto Posicao){
    this->Centro = Ponto((Max.x + Min.x)/2, (Max.y + Min.y)/2, (Max.z + Min.z)/2);
    this->MeiaLarg = Ponto(Max.x - Centro.x, Max.y - Centro.y, Max.z - Centro.z);
    Centro.x += Posicao.x;
    Centro.y += Posicao.y;
    Centro.z += Posicao.z;
}

bool AABB::calculaColisaoAABB(AABB E1, AABB E2){
    if(abs(E1.Centro.x - E2.Centro.x) > (E1.MeiaLarg.x + E2.MeiaLarg.x)) {
        return false; // nao ha colisao
    }
    if(abs(E1.Centro.y - E2.Centro.y) > (E1.MeiaLarg.y + E2.MeiaLarg.y)) {
        return false; // nao ha colisao
    }
    if(abs(E1.Centro.z - E2.Centro.z) > (E1.MeiaLarg.z + E2.MeiaLarg.z)) {
        return false; // nao ha colisao
    }
    return true; // ha colisao
}

bool AABB::calculaColisaoAABB(AABB E1, AABB* ArrayAABB, int size){
    for (int i = 0; i < size; i++)
    {
        AABB E2 = ArrayAABB[i];
        if(abs(E1.Centro.x - E2.Centro.x) > (E1.MeiaLarg.x + E2.MeiaLarg.x)) {
            continue; // nao ha colisao
        }
        if(abs(E1.Centro.y - E2.Centro.y) > (E1.MeiaLarg.y + E2.MeiaLarg.y)) {
            continue; // nao ha colisao
        }
        if(abs(E1.Centro.z - E2.Centro.z) > (E1.MeiaLarg.z + E2.MeiaLarg.z)) {
            continue; // nao ha colisao
        }
        return true; // ha colisao
    }
    return false;
}

