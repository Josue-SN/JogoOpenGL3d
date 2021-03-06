// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Simulador de Cidade
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;


#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "Temporizador.h"
Temporizador T;
double AccumDeltaT=0;

#include <vector>

#include "AABB.cpp"
#include "Aviao.h"
#include "ListaDeCoresRGB.h"
#include "Objeto.cpp"
#include "Personagem.cpp"
#include "Ponto.h"
#include "Temporizador.h"
#include "IO.h"

#include "stb_image.h"

GLfloat AspectRatio, angulo=0;
GLfloat AlturaViewportDeMensagens = 0.1; // percentual em relacao � altura da tela


// Controle do modo de projecao
// 0: Projecao Paralela Ortografica; 1: Projecao Perspectiva
// A funcao "PosicUser" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'p'

int ModoDeProjecao = 1;

// Controle do modo de projecao
// 0: Wireframe; 1: Faces preenchidas
// A funcao "Init" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'w'
int ModoDeExibicao = 1;

int comTextura = 0;
GLuint texId[12];


void carregaTextura(GLuint texID, string filePath){
    unsigned char* imgData;
    int largura,altura,canais;
    stbi_set_flip_vertically_on_load(true);
    imgData = stbi_load(filePath.c_str(), &largura, &altura, &canais, 4);
    if(imgData){
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);   

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }else{
        cout << "ERRO AO ABRIR A TEXTURA " << filePath.c_str();
    }
}

void carregaTexturas(){
    string nomeTexturas[] = 
    {
    "CROSS.png",
    "DL.png",
    "DLR.png",
    "DR.png",
    "LR.png",
    "None.png",
    "UD.png",
    "UDL.png",
    "UDR.png",
    "UL.png",
    "ULR.png",
    "UR.png"};

    for(int i = 0 ; i < 12; i++){
        string filePath = "./TexturaAsfalto/"+nomeTexturas[i];
        cout << "Caminho do filePath: " << filePath;
        carregaTextura(texId[i], filePath);
    }
}
//define a posicao do observador
bool TerceiraPessoa = true;

double nFrames=0;
double TempoTotal=0;

// Qtd de ladrilhos do piso. Inicialzada na INIT
int QtdX;
int QtdZ;

Aviao Avioes[3];
vector<Objeto> Escombros;
vector<Objeto> ObjetosIntransponiveis;
vector<Objeto> Combustiveis;
int indiceCombustiveis = 0;

// Pai ta de celta
Personagem Carro;



// codigos que definem o o tipo do elemento que est� em uma c�lula
#define V 0
#define P 10
#define R 20
#define C 30
#define E 40

// Matriz que armazena informacoes sobre o que existe na cidade
Elemento Cidade[100][100];
// ***********************************************
//  void calcula_ponto(Ponto p, Ponto &out)
//
//  Esta fun��o calcula as coordenadas
//  de um ponto no sistema de refer�ncia do
//  universo (SRU), ou seja, aplica as rota��es,
//  escalas e transla��es a um ponto no sistema
//  de refer�ncia do objeto SRO.
//  Para maiores detalhes, veja a p�gina
//  https://www.inf.pucrs.br/pinho/CG/Aulas/OpenGL/Interseccao/ExerciciosDeInterseccao.html
// ***********************************************
void CalculaPonto(Ponto p, Ponto &out) {

    GLfloat ponto_novo[4];
    GLfloat matriz_gl[4][4];
    int  i;

    glGetFloatv(GL_MODELVIEW_MATRIX,&matriz_gl[0][0]);

    for(i=0; i<4; i++) {
        ponto_novo[i] = matriz_gl[0][i] * p.x +
                        matriz_gl[1][i] * p.y +
                        matriz_gl[2][i] * p.z +
                        matriz_gl[3][i];
    }
    out.x = ponto_novo[0];
    out.y = ponto_novo[1];
    out.z = ponto_novo[2];
}

// **********************************************************************
//
// **********************************************************************
void InicializaCidade(int QtdX, int QtdZ)
{
    IO leitor;
    leitor.lerEntrada(QtdX, "MAPA.txt", Cidade);
    
    // Personagem Combustivel = Personagem();
    // Combustivel.Posicao = Ponto(1, 0, 1);
    // Combustiveis.push_back(Combustivel);

    // Combustivel = Personagem();
    // Combustivel.Posicao = Ponto(19, 0, 1);
    // Combustiveis.push_back(Combustivel);
}
void InicializaAvioes()
{

    // for (int i = 0; i < 3; i++)
    // {

    // }
    Avioes[0] = Aviao();
    Avioes[0].Rota[0] = Ponto(10,   5, 2);
    Avioes[0].Rota[1] = Ponto(18,   5, 10);
    Avioes[0].Rota[2] = Ponto(10,   5, 18);
    Avioes[0].Rota[3] = Ponto(10,   5, 18);
    Avioes[0].Rota[4] = Ponto(2,    5, 10);
    Avioes[0].Rota[5] = Ponto(10,   5, 2);
    Avioes[0].posicaoNaRota = 0;
    Avioes[0].estaIndo = true;

    Avioes[1] = Aviao();
    Avioes[1].Rota[0] = Ponto(18,   5,   18);
    Avioes[1].Rota[1] = Ponto(18,   5,   2);
    Avioes[1].Rota[2] = Ponto(2,    5,   2);
    Avioes[1].Rota[3] = Ponto(2,    5,   2);
    Avioes[1].Rota[4] = Ponto(2,    5,   18);
    Avioes[1].Rota[5] = Ponto(18,   5,   18);
    Avioes[1].posicaoNaRota = 0;
    Avioes[1].estaIndo = false;

    Avioes[2] = Aviao();
    Avioes[2].Rota[0] = Ponto(21,   5, 0);
    Avioes[2].Rota[1] = Ponto(21,   5, 21);
    Avioes[2].Rota[2] = Ponto(0,    5, 21);
    Avioes[2].Rota[3] = Ponto(0,    5, 21);
    Avioes[2].Rota[4] = Ponto(0,    5, 0);
    Avioes[2].Rota[5] = Ponto(21,   5, 0);
    Avioes[2].posicaoNaRota = 0;
    Avioes[2].estaIndo = false;
}

void DefineBoundingBoxesCenario(){
    for (int i = 0; i < QtdX; i++)
    {
        for (int j = 0; j < QtdZ; j++)
        {
            if(Cidade[i][j].tipo == "P"){
                Objeto Predio = Objeto();
                Predio.Posicao = Ponto(i, 0 ,j);
                Predio.destruido = false;

                AABB BoundingBox = AABB();
                BoundingBox.calculaAABB(Ponto(1,1,1), Ponto(0,0,0), Predio.Posicao);
                Predio.BoundingBox = BoundingBox;

                ObjetosIntransponiveis.push_back(Predio);
            }else if(Cidade[i][j].tipo == "C"){
                Objeto Combustivel = Objeto();
                Combustivel.Posicao = Ponto(i, 0 ,j);
                Combustivel.destruido = false;

                AABB BoundingBox = AABB();
                BoundingBox.calculaAABB(Ponto(1,1,1), Ponto(0,0,0), Combustivel.Posicao);
                Combustivel.BoundingBox = BoundingBox;
                Combustiveis.push_back(Combustivel);
            }else if(Cidade[i][j].tipo == "V"){
                Objeto quadra = Objeto();
                quadra.Posicao = Ponto(i, 0, j);
                quadra.destruido = false;

                AABB BoundingBox = AABB();
                BoundingBox.calculaAABB(Ponto(1,1,1), Ponto(0,0,0), quadra.Posicao);
                quadra.BoundingBox = BoundingBox;

                ObjetosIntransponiveis.push_back(quadra);
            }
        }
    }
    
    // Quadras[0] = AABB();
    // Quadras[1] = AABB();
    // Quadras[2] = AABB();
    // Quadras[3] = AABB();
    // //Bounding box da quadra inferior direita (começa em 3.5, 0, 3.5)
    // Quadras[0].calculaAABB(Ponto(5,0,5), Ponto(0,0,0), Ponto(3.5,0,3.5));
    // //Inferior esquerda
    // Quadras[1].calculaAABB(Ponto(5,0,5), Ponto(0,0,0), Ponto(12.5,0,3.5));
    // //Superior direita
    // Quadras[2].calculaAABB(Ponto(5,0,5), Ponto(0,0,0), Ponto(3.5,0,12.5));
    // //Superior esquerda
    // Quadras[3].calculaAABB(Ponto(5,0,5), Ponto(0,0,0), Ponto(12.5,0,12.5));
}


// **********************************************************************
//  void init(void)
//    Inicializa os parametros globais de OpenGL
// **********************************************************************
void init(void)
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto
   
    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_CULL_FACE);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glGenTextures(12, texId);
    carregaTexturas();
    
    
    if (ModoDeExibicao) // Faces Preenchidas??
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glEnable(GL_NORMALIZE);
    
    srand((unsigned int)time(NULL));
    
    QtdX = 30;
    QtdZ = 30;

    Carro.Posicao = Ponto(10, 0, 0);
    Carro.combustivel = 100;
    
    InicializaCidade(QtdX, QtdZ);
    InicializaAvioes();
    DefineBoundingBoxesCenario();
    
}

// **********************************************************************
//
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        angulo+= 1;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        TempoTotal = 0;
        nFrames = 0;
    }
}

Temporizador ControladorAnimacao;
double tempo;
void VerificaColisoesCarro(){
    tempo = ControladorAnimacao.getDeltaT();
    if(Carro.emMovimento){
        double deslocamento = tempo * ((QtdX+QtdZ)/2)/10;

        Ponto Avanco = Ponto(0,0,deslocamento);
        Avanco.rotacionaY(Carro.direcao);
        Carro.avancaPosicao(Ponto(Avanco.x, 0, Avanco.z));
        // Carro.avancaPosicao(Ponto(QuantidadeAvanco.x, 0, QuantidadeAvanco.z));
        Carro.BoundingBox.calculaAABB(Ponto(0.75,1,0.75), Ponto(0.25,0,0.25), Carro.Posicao);
        //caso haja colisao, revertemos o movimento
        if(Objeto::calculaColisaoObjetos(Carro.BoundingBox, ObjetosIntransponiveis.data(), ObjetosIntransponiveis.size()) != -1)
            Carro.avancaPosicao(Ponto(-Avanco.x, 0, -Avanco.z));
            // Carro.avancaPosicao(Ponto(-QuantidadeAvanco.x, 0, -QuantidadeAvanco.z));
        //caso saia do mapa, tambem revertmos
        if(Carro.Posicao.x > 20 || Carro.Posicao.x < 0)
            Carro.avancaPosicao(Ponto(-Avanco.x, 0, -Avanco.z));
        if(Carro.Posicao.z > 20 || Carro.Posicao.z < 0)
            Carro.avancaPosicao(Ponto(-Avanco.x, 0, -Avanco.z));
        int indiceColisao = Objeto::calculaColisaoObjetos(Carro.BoundingBox, Combustiveis.data(), Combustiveis.size());
        if(indiceColisao != -1){
            Objeto &Combustivel = Combustiveis[indiceColisao];
            if(!Combustivel.destruido){
                //Destroi na lista de combustiveis
                Combustivel.destruido = true;

                //Apaga na cidade
                int i = Combustivel.Posicao.x;
                int j = Combustivel.Posicao.z;
                Cidade[i][j].tipo = R;

                //Adiciona combustivel ao carro
                Carro.combustivel = 100;
            }
        }
    }
}

Temporizador ControladorBombas;
double TempoEntreBombas = 0;
void DisparaBombas(){
    TempoEntreBombas += ControladorBombas.getDeltaT();
    double TempoAleatorio = rand() % 10 + 1;
    int AviaoEfetuandoDisparo = rand() % 2;
    if(TempoEntreBombas > TempoAleatorio){
        TempoEntreBombas = 0;
        Aviao &A = Avioes[AviaoEfetuandoDisparo];
        Ponto AlvoArredondado = Ponto(round(A.Posicao.x), round(A.Posicao.y), round(A.Posicao.z));
        int x = AlvoArredondado.x;
        int z = AlvoArredondado.z;
        Cidade[x][z].tipo = "E";

        Objeto Escombro = Objeto();
        Escombro.Posicao = Ponto(x, 0 ,z);
        Escombro.destruido = false;

        AABB BoundingBox = AABB();
        BoundingBox.calculaAABB(Ponto(1,1,1), Ponto(0,0,0), Escombro.Posicao);
        Escombro.BoundingBox = BoundingBox;

        ObjetosIntransponiveis.push_back(Escombro);
    }
}


// **********************************************************************
Ponto CalculaBezier3(Ponto PC[], double t)
{
    Ponto PAux;
    double UmMenosT = 1-t;
    
    PAux =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    //P.z = 5;
    return PAux;
}
// **********************************************************************
void TracaBezier3Pontos()
{
    double t=0.0;
    double DeltaT = 1.0/10;
    Ponto PAux;
    //cout << "DeltaT: " << DeltaT << endl;
    glBegin(GL_LINE_STRIP);
    
    while(t<1.0)
    {
        // P = CalculaBezier3(Curva1, t);
        glVertex3f(PAux.x, PAux.y, PAux.z);
        t += DeltaT;
       // P.imprime(); cout << endl;
    }
    // P = CalculaBezier3(Curva1, 1.0); // faz o fechamento da curva
    glVertex3f(PAux.x, PAux.y, PAux.z);
    glEnd();
}



// **********************************************************************
//  void DesenhaCubo()
//
//
// **********************************************************************
void DesenhaCubo()
{
    glBegin ( GL_QUADS );
    // Front Face
    glNormal3f(0,0,1);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    // Back Face
    glNormal3f(0,0,-1);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    // Top Face
    glNormal3f(0,1,0);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    // Bottom Face
    glNormal3f(0,-1,0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    // Right face
    glNormal3f(1,0,0);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    // Left Face
    glNormal3f(-1,0,0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
}

void DesenhaAvioes(){
    for (int i = 0; i < 3; i++)
    {
        Aviao &A = Avioes[i];
        Ponto Curva[3];
        Ponto Posicao;
        //Ida
        if(A.estaIndo){
            Curva[0] = A.Rota[0];
            Curva[1] = A.Rota[1];
            Curva[2] = A.Rota[2];
            Posicao = CalculaBezier3(Curva, A.posicaoNaRota);
        }else{ //Volta
            Curva[0] = A.Rota[3];
            Curva[1] = A.Rota[4];
            Curva[2] = A.Rota[5];
            Posicao = CalculaBezier3(Curva, A.posicaoNaRota);
        }
        A.Posicao = Posicao;
        if(A.posicaoNaRota < 1){
            A.posicaoNaRota += 0.01;
        }else{
            A.estaIndo = !A.estaIndo;
            A.posicaoNaRota = 0.01;
        }
        glPushMatrix();
        glTranslatef(Posicao.x - 0.5, Posicao.y, Posicao.z - 0.5);
        glScalef(0.2, 0.1, 1);
        DesenhaCubo();
        glPopMatrix();
    }
}

// **********************************************************************
//  Desenha um pr�dio no meio de uam c�lula
// **********************************************************************
void DesenhaPredio(float altura)
{
    glPushMatrix();
       // glTranslatef(0, -1, 0);
        glScalef(0.2, altura, 0.2);
        glTranslatef(0, 1, 0);
        DesenhaCubo();
    glPopMatrix();
    
}

// **********************************************************************
//  Desenha o personagem
// **********************************************************************
void DesenhaPersonagem()
{
    glPushMatrix();
        glTranslatef(Carro.Posicao.x, Carro.Posicao.y, Carro.Posicao.z);
        glTranslatef(-0.5, 0.3, -0.5);
        if(false)
            glRotatef(Carro.direcao, Carro.Posicao.x, Carro.Posicao.y, Carro.Posicao.z);

        glRotatef(Carro.direcao, 0,1,0);
        defineCor(Black);
        glScalef(0.1, 0.3, 0.5);
        DesenhaCubo();
        glTranslatef(0, -0.5, 0.5);
        defineCor(White);
        glScalef(0.5, 0.5, 1);
        DesenhaCubo();

    glPopMatrix();
    
}

// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma c�lula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e est� sobre o plano XZ
// **********************************************************************
void DesenhaLadrilho(int corBorda, int corDentro)
{
    
    defineCor(corBorda); // desenha QUAD preenchido
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();
    
    defineCor(corDentro);
    glBegin ( GL_LINE_STRIP ); // desenha borda do ladrilho
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

}

void DesenhaLadrilho(GLuint texId)
{
    
    //glNewList(id, GL_COMPILE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //glEndList();
    
    

}

// **********************************************************************
// DesenhaCidade(int nLinhas, int nColunas)
// QtdX: nro de c�lulas em X
// QtdZ: nro de c�lulas em Z
// Desenha elementos que compiem a cidade
// **********************************************************************
void DesenhaCidade(int QtdX, int QtdZ)
{   
    enum tRuas{CROSS, DL, DLR, DR, LR, NONE, UD, UDL, UDR, UL, ULR, UR};
    glPushMatrix();
    glTranslatef(-0.5, 0, -0.5);
    for (size_t i = 0; i < QtdX; i++)
    {
        glPushMatrix();
        for (size_t j = 0; j < QtdZ; j++)
        {
            if(Cidade[i][j].tipo == "V"){
                DesenhaLadrilho(Gray, Black);
            }else if(Cidade[i][j].tipo == "CROSS"){
                comTextura?DesenhaLadrilho(texId[CROSS]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "DL"){
                comTextura?DesenhaLadrilho(texId[DL]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "DLR"){
                comTextura?DesenhaLadrilho(texId[DLR]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "DR"){
                comTextura?DesenhaLadrilho(texId[DR]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "LR"){
                comTextura?DesenhaLadrilho(texId[LR]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "None"){
                comTextura?DesenhaLadrilho(texId[NONE]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "UD"){
                comTextura?DesenhaLadrilho(texId[UD]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "UDL"){
                comTextura?DesenhaLadrilho(texId[UDL]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "UDR"){
                comTextura?DesenhaLadrilho(texId[UDR]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "UL"){
                comTextura?DesenhaLadrilho(texId[UL]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "ULR"){
                comTextura?DesenhaLadrilho(texId[ULR]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "UR"){
                comTextura?DesenhaLadrilho(texId[UR]):DesenhaLadrilho(White,Black);
            }else if(Cidade[i][j].tipo == "P"){
                DesenhaLadrilho(Gray, Black);
                defineCor(Brown);
                DesenhaPredio(1.2);
            }else if(Cidade[i][j].tipo == "C"){
                DesenhaLadrilho(Red, Black);
                defineCor(Light_Purple);
                DesenhaPredio(0.2);
            }else if(Cidade[i][j].tipo == "E"){
                DesenhaLadrilho(Gray, Black);
                defineCor(DarkPurple);
                DesenhaPredio(0.5);
            }
            glTranslatef(0, 0, 1);
        }
        glPopMatrix();
        glTranslatef(1, 0, 0);
    }
    glPopMatrix();
}


// **********************************************************************
//  void DefineLuz(void)
// **********************************************************************
void DefineLuz(void)
{
  // Define cores para um objeto dourado
  GLfloat LuzAmbiente[]   = {0.4, 0.4, 0.4f } ;
  GLfloat LuzDifusa[]   = {0.7, 0.7, 0.7};
  GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
  GLfloat PosicaoLuz0[]  = {0.0f, 3.0f, 5.0f };  // Posi��o da Luz
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

   // ****************  Fonte de Luz 0

 glEnable ( GL_COLOR_MATERIAL );

   // Habilita o uso de ilumina��o
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da luz n�mero Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentra��oo do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado ser� o brilho. (Valores v�lidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,51);

}

// **********************************************************************
//  void PosicUser()
//
//
// **********************************************************************
Ponto PosObservador = Ponto(10.5, 18, -5);
void PosicUser()
{
    // Define os par�metros da proje��o Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define o volume de visualiza��o sempre a partir da posicao do
    // observador
    if (ModoDeProjecao == 0)
        glOrtho(-10, 15, -10, 15, 0, 20); // Projecao paralela Orthografica
    else gluPerspective(90,AspectRatio,0.01,1500); // Projecao perspectiva

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // PosObservador = Ponto(-5, 5, 2);
    if(TerceiraPessoa){
        gluLookAt(PosObservador.x, PosObservador.y, PosObservador.z,   // Posi��o do Observador
            10.5,0,10.5,     // Posi��o do Alvo
            0.0f,1.0f,0.0f); // UP
    }else{
        Ponto Alvo = Ponto(0,0,1);
        Alvo.rotacionaY(Carro.direcaoCamera);
        gluLookAt(Carro.Posicao.x - 0.5, 1, Carro.Posicao.z - 0.5,   // Posi��o do Observador
            Carro.Posicao.x + Alvo.x - 0.5, 1, Carro.Posicao.z + Alvo.z - 0.5,     // Posi��o do Alvo
            0.0f,1.0f,0.0f); // UP
    }
}
// **********************************************************************
//  void reshape( int w, int h )
//		trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
	// Evita divis�o por zero, no caso de uam janela com largura 0.
	if(h == 0) h = 1;
    // Ajusta a rela��o entre largura e altura para evitar distor��o na imagem.
    // Veja fun��o "PosicUser".
	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Seta a viewport para ocupar toda a janela
    //glViewport(0, 0, w, h);
    glViewport(0, h*AlturaViewportDeMensagens, w, h-h*AlturaViewportDeMensagens);

    //cout << "Largura" << w << endl;

	PosicUser();

}
// **********************************************************************
//
// **********************************************************************
void printString(string s, int posX, int posY, int cor)
{
    defineCor(cor);
    
    glRasterPos3i(posX, posY, 0); //define posicao na tela
    for (int i = 0; i < s.length(); i++)
    {
//GLUT_BITMAP_HELVETICA_10,
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
    
}
// **********************************************************************
//
// **********************************************************************
void DesenhaEm2D()
{
    int ativarLuz = false;
    if (glIsEnabled(GL_LIGHTING))
    {
        glDisable(GL_LIGHTING);
        ativarLuz = true;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Salva o tamanho da janela
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h*AlturaViewportDeMensagens); // a janela de mensagens fica na parte de baixo da janela

    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(0,10, 0,10, 0,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Desenha linha que Divide as �reas 2D e 3D
    defineCor(Yellow);
    glLineWidth(5);
    glBegin(GL_LINES);
        glVertex2f(0,10);
        glVertex2f(10,10);
    glEnd();
    
    if(AlturaViewportDeMensagens == 1){
        printString("Voce ganhou", 5, 5.5, Yellow);
    }else{
        printString(to_string(Carro.combustivel), 5, 5, Yellow);
    }

    // Resataura os par�metro que foram alterados
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, h*AlturaViewportDeMensagens, w, h-h*AlturaViewportDeMensagens);

    if (ativarLuz)
        glEnable(GL_LIGHTING);

}

void VerificaFimDoJogo(){
    bool restamCombustiveis = false;
    for (int i = 0; i < Combustiveis.size(); i++)
    {
        if(!Combustiveis[i].destruido)
            restamCombustiveis = true;
    }
    if(!restamCombustiveis){
        AlturaViewportDeMensagens = 1;
    }
}
// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	DefineLuz();

	PosicUser();
    glLineWidth(2);
	
	glMatrixMode(GL_MODELVIEW);

    
    if (glIsEnabled(GL_TEXTURE_2D))
        cout << "TEXTURA ATIVADA";
    

    glColor3f(1,1,1);
    TracaBezier3Pontos();
    
    DesenhaAvioes();
    DesenhaCidade(QtdX,QtdZ);
    DesenhaPersonagem();

    VerificaColisoesCarro();

    DisparaBombas();

    DesenhaEm2D();

    VerificaFimDoJogo();

	glutSwapBuffers();
}


// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{
	switch ( key ) 
	{
    case 27:        // Termina o programa qdo
      exit ( 0 );   // a tecla ESC for pressionada
      break;        
    case 'p':
        ModoDeProjecao = !ModoDeProjecao;
        glutPostRedisplay();
        break;
    case 'e':
        ModoDeExibicao = !ModoDeExibicao;
        init();
        glutPostRedisplay();
        break;
    case 't':
        comTextura = !comTextura;
        glutPostRedisplay();
        break;
    case 'c':
        TerceiraPessoa = !TerceiraPessoa;
        glutPostRedisplay();
        break;
    case 'z':
        if(Carro.direcaoCamera < 360){
            Carro.direcaoCamera += 15;
        }else{
            Carro.direcaoCamera = 15;
        }
        break;
    case 'x':
        if(Carro.direcaoCamera > 0){
            Carro.direcaoCamera -= 15;
        }else{
            Carro.direcaoCamera = 345;
        }
    case ' ':
        Carro.nitro = !Carro.nitro;
        break;

    default:
        cout << key;
    break;
  }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )  
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )  
{
	switch ( a_keys ) 
	{
		case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
            // Carro.avancaPosicao(Ponto(0, 0, 1));
            // Carro.BoundingBox.calculaAABB(Ponto(1,1,1), Ponto(0,0,0), Carro.Posicao);
            // if(Carro.BoundingBox.calculaColisaoAABB(Carro.BoundingBox, Quadras, 4)){
            //     // cout << "Dentro da caixa de colisão\n";
            //     Carro.avancaPosicao(Ponto(0, 0, -1));
            // }
            Carro.emMovimento = true;
			break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
            // Carro.avancaPosicao(Ponto(0, 0, -1));
            // Carro.BoundingBox.calculaAABB(Ponto(1,1,1), Ponto(0,0,0), Carro.Posicao);
            // if(Carro.BoundingBox.calculaColisaoAABB(Carro.BoundingBox, Quadras, 4)){
            //     // cout << "Dentro da caixa de colisão\n";
            //     Carro.avancaPosicao(Ponto(0, 0, 1));
            // }
            Carro.emMovimento = false;
			break;
        case GLUT_KEY_LEFT:       // When Up Arrow Is Pressed...
            if(Carro.direcao < 360){
                Carro.direcao += 15;
            }else{
                Carro.direcao = 15;
            }
            if(Carro.direcaoCamera < 360){
                Carro.direcaoCamera += 15;
            }else{
                Carro.direcaoCamera = 15;
            }
			break;
	    case GLUT_KEY_RIGHT:     // When Down Arrow Is Pressed...
            if(Carro.direcao > 0){
                Carro.direcao -= 15;
            }else{
                Carro.direcao = 345;
            }
            if(Carro.direcaoCamera > 0){
                Carro.direcaoCamera -= 15;
            }else{
                Carro.direcaoCamera = 345;
            }
            break;
		default:
			break;
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )   
{
	glutInit            ( &argc, argv ); 
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
	glutInitWindowPosition (0,0);
	glutInitWindowSize  ( 1280, 720 );
	glutCreateWindow    ( "Ultimate City Crawler Championship Edition: Gasoline Chase" ); 
		
	init ();
	
	glutDisplayFunc ( display );  
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );

	glutMainLoop ( );     
    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(12, texId);     
	return 0; 
}



