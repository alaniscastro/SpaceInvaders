#define TAMANHO_OBJETOS 4           //Tamanho dos quadrados que definem os objetos em geral
#define INCREMENTO_MOVIMENTACAO 3   //Define o quanto o player vai se movimentar a cada execução da função desenha em que as setas estiverem pressionadas

//Constantes relacionadas às balas
#define QTDD_MAX_BALAS 5000
#define VEL_BALA 0.5
#define LIMITE_BALA_PLAYER 30        //Altura mínima de distância de uma bala para o player poder atirar outra

// Constantes relacionadas aos inimigos na fase 0
#define QTDD_INIM_X_0 8      //Quantidade de inimigos na horizontal
#define QTDD_INIM_Y_0 5       //Quantidade de inimigos na vertical
#define DIST_Y_INIM_0 3       // Distância no eixo vertical entre os inimigos
#define VEL_LAT_INIM_0 0.2      // Velocidade dos inimigos no deslocamento lateral
#define VEL_VERT_INIM_0 1.5      // Velocidade dos inimigos no deslocamento lateral
#define LIMITE_PASSOS_0 50       //Limita quantos "passos" os inimigos vão dar para cada lado
#define PROB_TIROS_INIMIGOS_0 150     //Define qual a probabilidade dos inimigos atirarem (2 ou mais)
//Os vetores abaxio representam a mesma variável com um valor para cada fase. Exemplo: qtddInimX[0] é a quantidade de inimigos na horizontal para a fase 0

// Constantes relacionadas aos inimigos na fase 1
#define QTDD_INIM_X_1 7      //Quantidade de inimigos na horizontal
#define QTDD_INIM_Y_1 3       //Quantidade de inimigos na vertical
#define DIST_Y_INIM_1 3       // Distância no eixo vertical entre os inimigos
#define VEL_LAT_INIM_1 0.25      // Velocidade dos inimigos no deslocamento lateral
#define VEL_VERT_INIM_1 3      // Velocidade dos inimigos no deslocamento lateral
#define LIMITE_PASSOS_1 45       //Limita quantos "passos" os inimigos vão dar para cada lado
#define PROB_TIROS_INIMIGOS_1 100  //Define qual a probabilidade dos inimigos atirarem (2 ou mais)

// Constantes relacionadas aos inimigos na fase 2
#define QTDD_INIM_X_2 6      //Quantidade de inimigos na horizontal
#define QTDD_INIM_Y_2 3       //Quantidade de inimigos na vertical
#define DIST_Y_INIM_2 4       // Distância no eixo vertical entre os inimigos
#define VEL_LAT_INIM_2 0.3      // Velocidade dos inimigos no deslocamento lateral
#define VEL_VERT_INIM_2 3.5     // Velocidade dos inimigos no deslocamento lateral
#define LIMITE_PASSOS_2 45       //Limita quantos "passos" os inimigos vão dar para cada lado
#define PROB_TIROS_INIMIGOS_2 50  //Define qual a probabilidade dos inimigos atirarem (2 ou mais)

// Constantes relacionadas aos inimigos na fase 3
#define QTDD_INIM_X_3 5      //Quantidade de inimigos na horizontal
#define QTDD_INIM_Y_3 4       //Quantidade de inimigos na vertical
#define DIST_Y_INIM_3 5       // Distância no eixo vertical entre os inimigos
#define VEL_LAT_INIM_3 0.4    // Velocidade dos inimigos no deslocamento lateral
#define VEL_VERT_INIM_3 4      // Velocidade dos inimigos no deslocamento lateral
#define LIMITE_PASSOS_3 40       //Limita quantos "passos" os inimigos vão dar para cada lado
#define PROB_TIROS_INIMIGOS_3 20  //Define qual a probabilidade dos inimigos atirarem (2 ou mais)

#define TECLA_ESC 27
#define TECLA_ESPACO 32
#define TECLA_p 'p'
#define TECLA_P 'P'
#define TECLA_r 'r'
#define TECLA_R 'R'
#define TECLA_c 'c'
#define TECLA_C 'C'

//Estrutura que define todos os objetos usados
struct objeto
{
    float x;
    float y;
    int larg;
    int alt;

    int vidas;          //Usado apenas no caso do player
    BOOLEAN destruido;  //Usado apenas no caso dos inimigos
};
