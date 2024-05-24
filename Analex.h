#ifndef ANALEX
#define ANALEX

#define TAM_MAX_LEXEMA 31

// Definição das enumerações
enum TOKEN_CAT {
    ID = 1, SINAL, INTCON, REALCON, STRINGCON, CHARCON, COMENTARIO, 
    FIM_EXPR, FIM_ARQ,PAL_RESERV,IDCONST
};

enum SINAIS {
    ATRIBUICAO = 1, MAIOR_QUE, MENOR_QUE, MAIOR_IGUAL, MENOR_IGUAL, DIFERENTE,
    IGUALDADE, ADICAO, SUBTRACAO, MULTIPLICACAO, DIVISAO, ABRE_PAR, FECHA_PAR, 
    ABRE_COLCHETE, FECHA_COLCHETE, ABRE_CHAVE, FECHA_CHAVE, PONTO, DOIS_PONTOS, 
    PONTO_VIRGULA, VIRGULA, ASPAS, NEGACAO, AND, OR, ENDERECO, POTENCIA
};

enum PAL_RESERV {
    MAIN = 1,DOWNTO, WHILE, FOR, IF, ELSEIF, ELSE, ENDIF, GOBACK, GETINT, GETCHAR,
    PUTINT, PUTREAL, PUTCHAR, BLOCK, ENDBLOCK, CONST, CHAR, INT, REAL, BOOL, WITH, DO,
    VARYING, FROM, TO
};

// Definição da estrutura TOKEN
typedef struct {
    enum TOKEN_CAT cat;
    union {
        int codigo;
        char lexema[TAM_MAX_LEXEMA];
        int valor_int;
        double valor_double;

    };
} TOKEN;

//variaveis globais

extern FILE *fd;
extern TOKEN t;
extern int contLinha;
TOKEN Analex(FILE *);

#endif
