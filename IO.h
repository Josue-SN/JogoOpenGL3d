#include <string>
#include <fstream>
#include <iostream>
#include "Elemento.h"
using namespace std;

class IO{

public:
    static void lerEntrada(int largura, string nome, Elemento matriz[100][100]);
private:
    static void split(string &linha, string split[]);
};