#include "Objeto.h"

//retorna -1 se nao houver colisao, retorna o indice do objeto com o qual houve a colisao caso haja colisao
int Objeto::calculaColisaoObjetos(AABB E1, Objeto* ArrayObjetos, int size){
    for (int i = 0; i < size; i++)
    {
        AABB E2 = ArrayObjetos[i].BoundingBox;
        if(abs(E1.Centro.x - E2.Centro.x) > (E1.MeiaLarg.x + E2.MeiaLarg.x)) {
            continue; // nao ha colisao
        }
        if(abs(E1.Centro.y - E2.Centro.y) > (E1.MeiaLarg.y + E2.MeiaLarg.y)) {
            continue; // nao ha colisao
        }
        if(abs(E1.Centro.z - E2.Centro.z) > (E1.MeiaLarg.z + E2.MeiaLarg.z)) {
            continue; // nao ha colisao
        }
        return i; // ha colisao
    }
    return -1;
}