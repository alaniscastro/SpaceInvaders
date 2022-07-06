#include "GL/glew.h"
#include <GL/freeglut.h>
#include <time.h>
#include "SOIL/SOIL.h"
#include "stdlib.h"
#include "stdio.h"
#include "constantes.h"
#include <windows.h> //musica
#include <mmsystem.h> //musica


//Vari�veis que agrupam em um vetor as constantes dos inimigos por fase
int qtddInimX[4] = {QTDD_INIM_X_0, QTDD_INIM_X_1, QTDD_INIM_X_2, QTDD_INIM_X_3};
int qtddInimY[4] = {QTDD_INIM_Y_0, QTDD_INIM_Y_1, QTDD_INIM_Y_2, QTDD_INIM_Y_3};     //Quantidade de inimigos no eixo x
float distYInim[4] = {DIST_Y_INIM_0, DIST_Y_INIM_1, DIST_Y_INIM_2, DIST_Y_INIM_3}; // Dist�ncia no eixo vertical entre os inimigos
float velLatInim[4] = {VEL_LAT_INIM_0, VEL_LAT_INIM_1, VEL_LAT_INIM_2, VEL_LAT_INIM_3};
float velVertInim[4] = {VEL_VERT_INIM_0, VEL_VERT_INIM_1, VEL_VERT_INIM_2, VEL_VERT_INIM_3};
float limitePassos[4] = {LIMITE_PASSOS_0, LIMITE_PASSOS_1, LIMITE_PASSOS_2, LIMITE_PASSOS_3};
int probTirosInimigos[4] = {PROB_TIROS_INIMIGOS_0, PROB_TIROS_INIMIGOS_1, PROB_TIROS_INIMIGOS_2, PROB_TIROS_INIMIGOS_3};

//Vari�veis para salvar a identifica��o das texturas
GLuint idTexturaNaveInimigo;
GLuint idTexturaNave;
GLuint idTexturaBomba;
GLuint idTexturaFundo;
GLuint idFundoPausado;

//#### criei essa variavel para pegar  path base do programa l� no main. argv[0] traz sempre o caminho do exe #3
char basepath[1000];


//Objetos gerais do jogo
struct objeto player;
struct objeto inimigos[20][20];
struct objeto balasPlayer[QTDD_MAX_BALAS];
struct objeto balasInimigos[QTDD_MAX_BALAS];

//Vari�veis relacionadas ao movimento e quantidade dos inimigo na fun��o atualizaInimigos
int contaPassos;        //O contador de passos para o lado que os inimigos d�o, situados entre os limites LIMITE_COUNT e -LIMITE_COUNT
BOOLEAN ida;            //� verdadeira quando os inimigos est�o se movimentando para a direita, caso contr�rio � falsa (volta)
BOOLEAN desce;          //S� � verdadeira quando os inimigos alcan�am os limites laterais e devem descer verticalmente
int qtddInimigos;       //Representa a quantidade de inimigos ainda n�o destru�dos na fase atual. Quando chega em 0, o player passa de fase ou vence o jogo

//Vari�vis relacionadas �s balas
int qtddBalasPlayer;    //Quantidade de balas que j� foram atiradas pelo player, o que ajuda a manter controle de quantidade e posi��o das balas
int qtddBalasInim;      //Quantidade de balas que j� foram atiradas pelos inimigos

//Vari�veis de comandos de teclas
BOOLEAN pausado;                //TRUE: o jogo est� pausado. FALSE: o jogo est� acontecendo
BOOLEAN usandoTeclado;          //TRUE: movimentando o player com o teclado. FALSE: movimentando o player com o mouse
struct objeto pontoDoMouse;     //Ponto onde se encontra o mouse

//Vari�veis relacionadas ao fim de jogo
char* mensagemFinal;            //A mensagem que � exibida no fim de jogo.
BOOLEAN fim;                    //Verdadeira quando o player venceu todas as fases

//Vari�veis relacionadas � troca de fases
int fase;                       //Valor da fase atual

//Fun��o que recebe como par�metro as dimens�es e localiza��o de um objeto que � retornado com as caracter�sticas especificadas
struct objeto criaObjeto (int x, int y, int larg, int alt)
{
    struct objeto novoObjeto;

    //Coordenada x,y: ponto superior esquerdo do objeto
    novoObjeto.x = x;
    novoObjeto.y = y;
    novoObjeto.larg = larg;
    novoObjeto.alt = alt;

    novoObjeto.destruido = FALSE;   //Nenhum inimigo est� destru�do quando o objeto � criado
    return novoObjeto;
}

// Fun��o que carrega uma textura e retorna a sua id
GLuint carregaTextura(const char* arquivo) {
    char caminhoArquivo[1000];              //#### essa 3 linhas � para montar o path do arquivo correto, talvez fazer uma funcao #3
    strcpy(caminhoArquivo, basepath);
    strcat(caminhoArquivo, arquivo);

    GLuint idTextura = SOIL_load_OGL_texture(caminhoArquivo, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (idTextura == 0)
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    return idTextura;
}

void escreveNaTela(void * font, char *s, float x, float y) {    //x = 1000 � um valor de referencia com o qual os caracteres grandes s�o "centralizados"

    glColor3f(1, 1, 1);

    if (x == 1000) {
        x = 2  + (120 - strlen(s) * 2) / 2;    //Centraliza o texto
    }

    glRasterPos3f(x, y, 0);

    for (unsigned int i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(font, s[i]);
    }
}

void desenhaInimigos()
{
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaNaveInimigo);

    for(int i = 0; i < qtddInimY[fase]; i++)
    {
        for(int j = 0; j < qtddInimX[fase]; j++)
        {
            struct objeto inimigoDaVez = inimigos[i][j];

            //Verifica se o inimigo atual j� foi destru�do
            if(inimigoDaVez.destruido) {
                continue;
            }

            glBegin(GL_TRIANGLE_STRIP);
                glTexCoord2f(0, 0);
                glVertex2f(inimigoDaVez.x, inimigoDaVez.y);    //V�rtice superior esquerdo

                glTexCoord2f(1, 0);
                glVertex2f(inimigoDaVez.x + inimigoDaVez.larg, inimigoDaVez.y);                              //V�rtice superior direito

                glTexCoord2f(0, 1);
                glVertex2f(inimigoDaVez.x, inimigoDaVez.y - inimigoDaVez.alt); //V�rtice inferior esquerdo

                glTexCoord2f(1, 1);
                glVertex2f(inimigoDaVez.x + inimigoDaVez.larg, inimigoDaVez.y - inimigoDaVez.alt);//V�rtice inferior direito
                glEnd();
            glFlush();
        }
    }
    glDisable(GL_TEXTURE_2D);
}
void desenhaBala(GLfloat x, GLfloat y){
    glBegin(GL_TRIANGLE_STRIP);

    glTexCoord2f(0, 1);
    glVertex2f(x, y);    //V�rtice superior esquerdo

    glTexCoord2f(1, 1);
    glVertex2f(x+1,y );        //V�rtice superior direito

    glTexCoord2f(0, 0);
    glVertex2f(x,y-2 );//V�rtice inferior esquerdo

    glTexCoord2f(1, 0);
    glVertex2f(x+1,y-2 );//V�rtice inferior direito
    glEnd();
    glFlush();
}
void desenhaBalas()
{
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaBomba);

    //Itera pelas balas do player e desenha cada uma na sua posi��o posi��o
    for(int i = 0; i < qtddBalasPlayer; i++)
    {
        desenhaBala(balasPlayer[i].x,balasPlayer[i].y);
    }

    //Itera pelas balas dos inimigos e desenha cada uma na sua posi��o
    for(int i = 0; i < qtddBalasInim; i++) {
        desenhaBala(balasInimigos[i].x,balasInimigos[i].y);
    }
    glDisable(GL_TEXTURE_2D);
}

void desenhaNave()
{
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaNave);

    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 1);
        glVertex2f(player.x, player.y);

        glTexCoord2f(0, 0);
        glVertex2f(player.x, player.y - player.alt);

        glTexCoord2f(1, 0);
        glVertex2f(player.x + player.larg, player.y - player.alt);

        glTexCoord2f(1, 1);
        glVertex2f(player.x + player.larg, player.y);
    glEnd();
    glFlush();
    glDisable(GL_TEXTURE_2D);

    //Escreve a quantidade de vidas na tela
    char buffer[30];
    sprintf(buffer, "Vidas: %d", player.vidas);
    escreveNaTela(GLUT_BITMAP_HELVETICA_12, buffer, 10, 2);
}

void desenhaFundo()
{
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);

    //Verifica se o jogo est� pausado e escolhe a textura adequada
    if(pausado) {
        glBindTexture(GL_TEXTURE_2D, idFundoPausado);
    } else {
        glBindTexture(GL_TEXTURE_2D, idTexturaFundo);
    }

    glBegin(GL_TRIANGLE_FAN);

        glTexCoord2f(0, 1);
        glVertex2f(0, 80);

        glTexCoord2f(0, 0);
        glVertex2f(0, 0);

        glTexCoord2f(1, 0);
        glVertex2f(120, 0);

        glTexCoord2f(1, 1);
        glVertex2f(120, 80);

    glEnd();
    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void desenhaTelaDePausa() {
    if(pausado) {

        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.5, 0.5, 0.5);
        desenhaFundo();

        if(fim) {
            escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_24, mensagemFinal, 1000, 60);
        }

        if(!fim) {
            escreveNaTela(GLUT_BITMAP_TIMES_ROMAN_24, (char*)"Jogo pausado", 1000, 60);
        }
        escreveNaTela(GLUT_BITMAP_HELVETICA_12, (char*)"Pressione 'P' para continuar", 10, 20);
        escreveNaTela(GLUT_BITMAP_HELVETICA_12, (char*)"Pressione 'R' para reiniciar", 10, 15);
        escreveNaTela(GLUT_BITMAP_HELVETICA_12, (char*)"Pressione 'Esc' para sair", 10, 10);

        escreveNaTela(GLUT_BITMAP_HELVETICA_12, (char*)"Comandos no jogo:", 60, 25);
        escreveNaTela(GLUT_BITMAP_HELVETICA_12, (char*)"Movimente o player com as setas", 65, 20);
        escreveNaTela(GLUT_BITMAP_HELVETICA_12, (char*)"Pressione 'C' para controlar com o mouse", 65, 15);
        escreveNaTela(GLUT_BITMAP_HELVETICA_12, (char*)"Pressione espaco para atirar", 65, 10);
    }
}

// Rotina de desenho
void desenha()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.5, 0.5, 0.5);
    desenhaFundo();

    glColor3f(1, 1, 1);
    desenhaNave();

    glColor3f(1, 1, 1);
    desenhaInimigos();

    glColor3f(1, 1, 1);
    desenhaBalas();

    //Desenha a fase atual na tela
    char buffer[30];
    sprintf(buffer, "Fase: %d/4", fase+1);
    escreveNaTela(GLUT_BITMAP_HELVETICA_12, buffer, 10, 5);

    desenhaTelaDePausa();

    glutSwapBuffers();
}

GLuint sorteiaPlanoDeFundo() {
    int sorteio = rand()%4; //Sorteia de 1 a 4, cada n�mero carrega uma imagem

    const char * arrayCaminhoImagens [] = {"imagens\\espaco1.jpg", "imagens\\espaco2.png", "imagens\\espaco3.jpg", "imagens\\espaco4.jpg"};
    return carregaTextura(arrayCaminhoImagens[sorteio]);
}

void atiraBala(BOOLEAN ehBalaDoPlayer)
{
    //Cria uma bala do player
    if(ehBalaDoPlayer) {
        if(qtddBalasPlayer > 0 && balasPlayer[qtddBalasPlayer-1].y < LIMITE_BALA_PLAYER) {
            return;
        }

        balasPlayer[qtddBalasPlayer].y = player.y + TAMANHO_OBJETOS/2;
        balasPlayer[qtddBalasPlayer].x = player.x + (player.larg/2)-1;
        qtddBalasPlayer++;

    }
    else {
        //Cria uma bala dos inimigos e sorteia o inimigo para atirar
        int i = rand() % qtddInimY[fase];
        int j = rand() % qtddInimX[fase];
        balasInimigos[qtddBalasInim].y = inimigos[i][j].y + TAMANHO_OBJETOS/2;
        balasInimigos[qtddBalasInim].x = inimigos[i][j].x + (player.larg/2)-1;
        qtddBalasInim++;
    }

    glutPostRedisplay();
}

//Colisao
BOOLEAN colisao(struct objeto objeto1, struct objeto objeto2){
    if(objeto1.y < objeto2.y-objeto2.alt)return FALSE;
    else if(objeto1.y-objeto1.alt > objeto2.y)return FALSE;
    else if(objeto1.x+objeto1.larg < objeto2.x)return FALSE;
    else if(objeto1.x > objeto2.x+objeto2.larg)return FALSE;

    return TRUE;
}

void playBackgroundSound(char * arquivoSom){
    //path relativo
    char caminhoArquivo[1000];
    strcpy(caminhoArquivo, basepath);
    strcat(caminhoArquivo, arquivoSom);
    PlaySound(TEXT(caminhoArquivo),NULL,SND_LOOP|SND_ASYNC);
}

void carregaNovaFase()
{
    //carrega uma nova fase resetando as vari�veis necess�rias

    //cria os inimigos. Nesse caso, y do inimigo � a dist�ncia do inicio da janela at� a sua altura
    float distX = (120 - qtddInimX[fase] * TAMANHO_OBJETOS)/(qtddInimX[fase]+1); // Dist�ncia no eixo horizontal entre os inimigos
    for(int i = 0; i < qtddInimY[fase]; i++)
    {
        //  Come�a do ponto (0,120) em diante
        for(int j = 0; j < qtddInimX[fase]; j++)
        {
            float yObj = 80 - distYInim[fase]*(i+1) - TAMANHO_OBJETOS*i; // Altura da linha atual
            float xObj =  distX + j * (distX+TAMANHO_OBJETOS); //Dist�ncia entre a coluna e x=0

            inimigos[i][j] = criaObjeto(xObj, yObj, TAMANHO_OBJETOS, TAMANHO_OBJETOS);

            glFlush();
        }
    }

    //Define os valores iniciais de algumas vari�veis que devem ser redefinidas a cada fase
    //Define os valores iniciais de algumas vari�veis que devem ser redefinidas a cada fase
    contaPassos = 0;
    ida = TRUE;
    desce = FALSE;

    qtddBalasPlayer = 0;
    qtddBalasInim = 0;

    qtddInimigos = qtddInimX[fase] * qtddInimY[fase];

    idTexturaFundo = sorteiaPlanoDeFundo(); //Sempre que uma nova fase for carregada, um dos planos de fundo ser� sorteado.

    playBackgroundSound((char*)"musica\\starwars2.wav");
}

//Fun��o que inicializa o valor de algumas vari�veis
void inicializa()
{
    // define qual � a cor do fundo
    glClearColor(1.0, 1.0, 1.0, 0.0);

    //cria o jogador
    player  = criaObjeto(59, TAMANHO_OBJETOS+5, TAMANHO_OBJETOS, TAMANHO_OBJETOS);
    player.vidas = 5;

    fase = 0;

    carregaNovaFase();

    //Carrega as texturas
    idTexturaNave = carregaTextura("imagens\\nave.png");
    idTexturaBomba = carregaTextura("imagens\\bomba.png");
    idTexturaNaveInimigo = carregaTextura("imagens\\nave_inimiga.png");

    idFundoPausado = carregaTextura("imagens\\fundoPausa.jpg");

    //Inicia os valores de algumas vari�veis
    pausado = FALSE;
    usandoTeclado = TRUE;
    pontoDoMouse = criaObjeto(60, 15, 0, 0);

    fim = FALSE;
}

void fimDeJogo (BOOLEAN perdeu,  char* mensagem) {
    mensagemFinal = mensagem;   //#### precisa de mensagem final ser uma variavel global ??? #8
    inicializa();
    pausado = true;
    fim = TRUE;
}

//Atualiza as vari�veis de acordo com a l�gica implementada, para depois redesenhar a tela com as mudan�as feitas
void atualiza(int periodo)
{
    if (!pausado) {
        fim = FALSE;

        //Verifica se os inimigos acabaram
        if(qtddInimigos == 0) {
            fase++;
            if(fase >= 4) {
                fimDeJogo(FALSE, (char*)"Voce venceu.");
            }
            else {
                carregaNovaFase();
            }
        }

        //Verifica se as naves inimmigas chegaram nos limites laterais
        if (contaPassos >= limitePassos[fase])
        {
            ida = FALSE;
            desce = TRUE;
        }
        else{
            if (contaPassos <= -limitePassos[fase])
            {
                ida = TRUE;
                desce = TRUE;
            }
        }

        //Movimentos e tiros de inimigos
        for(int i = 0; i < qtddInimY[fase] ; i++)
        {
            for(int j = 0; j < qtddInimX[fase]; j++)
            {
                //Verifica se o atual inimigo foi destru�do
                if(inimigos[i][j].destruido) {
                    continue;
                }

                //Movimenta o inimigo lateralmente
                if(contaPassos < limitePassos[fase] && ida)
                {
                    inimigos[i][j].x += velLatInim[fase];
                }
                if (contaPassos > -limitePassos[fase] && !ida)
                {
                    inimigos[i][j].x -= velLatInim[fase];
                }

                //Movimenta o inimigo verticalmente
                if (desce)
                {
                    inimigos[i][j].y -= velVertInim[fase];
                }

                //Verifica��o de colis�es dos aliens com as balas do player
                for(int k = 0; k < qtddBalasPlayer; k++) {
                    if(colisao(balasPlayer[k], inimigos[i][j])) {
                        inimigos[i][j].x = -100;    //#### #11
                        inimigos[i][j].y = 1000;
                        inimigos[i][j].destruido = TRUE;

                        balasPlayer[k].x = 1000;
                        balasPlayer[k].y = 1000;

                        qtddInimigos--;
                    }
                }

                //Verifica��o de colis�o dos aliens com o player
                if(colisao(player, inimigos[i][j])) {
                    player.x = 1000;        //#### #11
                    player.y = 1000;

                    fimDeJogo(TRUE, (char*)"Voce perdeu.");
                }

                //Verifica se os inimigos chegaram ao solo
                if(inimigos[i][j].y <= 0 ) {
                    fimDeJogo(TRUE, (char*)"Voce perdeu. Os inimigos chegaram ao solo");
                }
            }
        }

        //Verifica se os inimigos est�o se movimentando para a esquerda ou para a direita
        contaPassos += (ida ? 1 : -1);
        desce = FALSE;

        //Movimenta as balas do player
        for(int i = 0; i < qtddBalasPlayer; i++)
        {
            balasPlayer[i].y += VEL_BALA;
        }

        //Tiros dos inimigos
        if(rand()% probTirosInimigos[fase] == 1) {      // De tempos em tempos, os inimigos lan�am tiros
            atiraBala(FALSE);
        }

        //Movimento dos tiros dos inimigos
        for(int i = 0; i < qtddBalasInim; i++) {
            balasInimigos[i].y -= VEL_BALA;

            //Verifica colis�o das balas dos inimigos com o player
            if(colisao(balasInimigos[i], player)) {
                balasInimigos[i].x = -1000;
                balasInimigos[i].y = -1000;

                player.vidas--;
                if(player.vidas <= 0) {
                    player.x = 1000;
                    player.y = 1000;
                    fimDeJogo(TRUE,(char*) "Voce perdeu. Sua nave foi atingida muitas vezes!");
                }
            }
        }

        //Movimenta��o do player quando est� sendo feita pela posi��o do mouse
        if(!usandoTeclado) {
            if(player.x < pontoDoMouse.x*1/5) {
                player.x += 0.2;
            }
            else {
                player.x -= 0.2;
            }
        }
    }
    glutPostRedisplay();                    //Redesenha a tela com as atualiza��es feitas
    glutTimerFunc(periodo, atualiza, periodo);
}


void redimensionada(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 120.0, 0.0, 80.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//Troca o controle de teclado para o mouse e vice versa
void trocaControle() {
    usandoTeclado = !usandoTeclado;     //Se o jogador est� usando o teclado, passa a uar o mouse, e vice-versa
}

//Callback do movimento do mouse
void movimentoDoMouse (int x, int y) {
    pontoDoMouse.x = x;
}

//Callback de teclado usada para caracteres ASCII
void teclaPressionada(unsigned char key, int x, int y)
{
    // v� qual tecla foi pressionada
    if (key == TECLA_ESC)
        exit(0);  // Sai da aplica��o
    if (key == TECLA_ESPACO){
        //atira uma bala do player
        if(!pausado) {
            atiraBala(TRUE);
        }
    }
    if ((key == TECLA_P) || (key == TECLA_p)){
        pausado = !pausado;
        playBackgroundSound((pausado ? (char*)"musica\\elevatorsong.wav" : (char*)"musica\\starwars2.wav"));
    }
    if ((key == TECLA_R) || (key == TECLA_r))
        inicializa();

    if ((key == TECLA_C) || (key == TECLA_c))
        trocaControle();

    glutPostRedisplay();
}

//Callback de teclado para teclas especiais
void tecladoEspecial (int key, int x, int y)
{
    //Movimenta��o do player quando � feita pelo teclado
    if(!pausado && usandoTeclado) {
        if(key == GLUT_KEY_LEFT && player.x > 0)
        {
            player.x -= INCREMENTO_MOVIMENTACAO;
        }
        if(key == GLUT_KEY_RIGHT && player.x < 116)
        {
            player.x += INCREMENTO_MOVIMENTACAO;
        }
    }
    glutPostRedisplay();
}

// --  Fun��o principal
int main(int argc, char *argv[])
{
    //faz uma c�pia do path para ser usado como referencia para carregar as imagens
    strcpy(basepath, argv[0]);
    for(int i=strlen(basepath);((basepath[i-1]!='\\') && (i>0));i--){ basepath[i-1]='\x0'; }

    glutInit(&argc, argv);
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600,400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Galaxian");

    // Registra callbacks para eventos
    glutDisplayFunc(desenha);
    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);
    glutSpecialFunc(tecladoEspecial);
    //glutIdleFunc(atualiza);
    glutTimerFunc(0, atualiza, 3);
    glutPassiveMotionFunc(movimentoDoMouse);


    // Configura valor inicial de algumas
    // vari�veis de estado do OpenGL
    inicializa();

    glutMainLoop();
    return 0;
}
