#include "IO.h"
/*
@param largura quantidade de caracteres de cada linha.
@param nomeArquivo nome do arquivo, por exemplo lista.txt.
@param preenche uma matriz de tamanho maximo [100][100].
*/
void IO::lerEntrada(int largura, string nomeArquivo, Elemento matriz[100][100]){
    string line;
    ifstream myFile(nomeArquivo);
    string linhaSplitada[largura];
    int i = 0;
    while(getline(myFile, line) && line.size() > 0){
        split(line, linhaSplitada);
        for (int j = 0; j < largura; j++)
        {
            matriz[i][j].tipo = linhaSplitada[j];
        }
        i++;
    }

    myFile.close();
}

/*
@param linha recebe a linha que var ser splitada, o separador e feito por tab.
@return split [] array que sera retornada com a linha splitada.
*/
void IO::split(string &linha, string split[]){
    int inicio = 0;
    int tamanho = 0;
    int iSplit = 0;
    for (int i = 0; i < linha.size(); i++)
    {   
        //C rn c c
       if(linha[i] != '\t'){
            tamanho++;
        }
        if(linha[i] == '\t' || i == linha.size() - 1){
            split[iSplit] = linha.substr(inicio,tamanho);
            tamanho = inicio + tamanho;
            tamanho++;
            inicio = tamanho;
            tamanho = 0;
            iSplit++;
        }
    }
}

// int main(int argc, char const *argv[])
// {
//     IO teste;
//     Elemento cidade[100][100];
//     teste.lerEntrada(30,"MAPA.txt", cidade);
//     for(int i = 0; i < 30; i++){
//         for(int j = 0; j < 30; j++){
//             cout << cidade[i][j].tipo <<  " ";
//         }
//         cout << "\n";
//     }
    
//     return 0;
// }
