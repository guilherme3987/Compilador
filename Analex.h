#ifndef ANALEX
#define ANALEX
#include <stdbool.h>
#include <stdio.h>
#define TAM_MAX_LEXEMA 31

// Definição das enumerações
enum TOKEN_CAT {
    ID = 1, SINAL, INTCON, REALCON, STRINGCON, CHARCON, COMENTARIO, 
    FIM_EXPR, FIM_ARQ, PAL_RESERV, IDCONST,SINAIS 
};

enum SINAIS {
    ATRIBUICAO = 1, MAIOR_QUE, MENOR_QUE, MAIOR_IGUAL, MENOR_IGUAL, DIFERENTE,
    IGUALDADE, ADICAO, SUBTRACAO, MULTIPLICACAO, DIVISAO, ABRE_PAR, FECHA_PAR, 
    ABRE_COLCHETE, FECHA_COLCHETE, ABRE_CHAVE, FECHA_CHAVE, PONTO, DOIS_PONTOS, 
    ABRE_COL,FECHA_COL,PONTO_VIRGULA, VIRGULA, ASPAS, NEGACAO, AND, OR, ENDERECO, POTENCIA, E_COMERCIAL
};

enum PAL_RESERV {
    MAIN = 1,DOWNTO, WHILE,ENDWHILE,THEN, FOR, IF, ELSEIF, ELSE, ENDIF, GOBACK, GETINT, GETCHAR,GETREAL,
    PUTINT, PUTREAL, PUTCHAR, BLOCK, ENDBLOCK, CONST, CHAR, INT, REAL, BOOL, WITH, DO,ENDDO,
    VARYING, FROM, TO
};

// Definição da estrutura TOKEN
typedef struct {
    enum TOKEN_CAT cat;
    //bool token_processado; //Bool para indicar se o token já foi tratado
    union {
        int codigo;
        char lexema[TAM_MAX_LEXEMA];
        int valor_int;
        double valor_double;
    };
} TOKEN;

extern FILE *fd;
extern TOKEN t;
extern int contLinha;
TOKEN Analex(FILE *fd);

#endif