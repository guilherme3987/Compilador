#ifndef ANALEX
#define ANALEX
#define TAM_MAX_LEXEMA 31

enum TOKEN_CAT {ID = 1, SINAL, INTCON, REALCON, STRINGCON, CHARCON, COMENTARIO, 
FIM_EXPR, FIM_ARQ};

enum SINAIS {ATRIBUICAO = 1, MAIOR_QUE, MENOR_QUE, MAIOR_IGUAL, MENOR_IGUAL, DIFERENTE,
 IGUALDADE, ADICAO, SUBTRACAO, MULTIPLICACAO, DIVISAO, ABRE_PAR, FECHA_PAR, 
 ABRE_COLCHETE, FECHA_COLCHETE, ABRE_CHAVE, FECHA_CHAVE, PONTO, DOIS_PONTOS, 
 PONTO_VIRGULA, VIRGULA, ASPAS, NEGACAO, AND, OR, ENDERECO};


typedef 
    struct {
        enum TOKEN_CAT cat;
        union {
            int codigo;
            char lexema[TAM_MAX_LEXEMA];
            int valor_int;
            double valor_double;
    };
} TOKEN;

#endif

