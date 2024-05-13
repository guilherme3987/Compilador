#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Analex.h"
#include <stdlib.h>

#define TAM_LEXEMA 31
#define TAM_NUM 20

//Var global
int contLinha = 1;

//Rertorno de erro
void error(char msg[], int linha) {
    printf("%s%d\n", msg, linha);
    exit(1);
}

//Função para retornar ocorrências
bool ocorrencia_tab_enter(char c){
    return c == ' ' || c == '\t';
}

bool ocorrencia_letra(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool ocorrencia_digito(char c){
    return c >= '0' && c <= '9';
}
bool ocorrencia_underline(char c){
    return c == '_';
}

// Caracteres imprimíveis na tabela ASCII 
bool eh_imprimivel(char c) {
    return c >= 32 && c <= 126; 
}
//Função Analex
TOKEN Analex(FILE *fd) {
    int estado = 0;
    int tamL = 0;
    int tamD = 0;
    char lexema[TAM_LEXEMA] = "";
    char digitos[TAM_NUM] = "";
    char c;
    TOKEN t;

    ///Transições do AFD
    while (1) {
        c = fgetc(fd);
        switch (estado) {
        case 0:                             //Estado 0
            if (ocorrencia_tab_enter(c)) {
                estado = 0;
            } else if (ocorrencia_letra(c)) {       //Estado 0 sai por letra para estado 3
                estado = 3;
                lexema[tamL] = c;  
                lexema[++tamL] = '\0'; 
            } else if (ocorrencia_underline(c)) {   //Estado 0 sai por _ para estado 1
                estado = 1;
                lexema[tamL] = c;  
                lexema[++tamL] = '\0'; 
            } else if (ocorrencia_digito(c)) {      //Estado 0 sai por digitos para estado 4
                estado = 4;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            } else if (c == '+') {              //Estado 0 sai por + para estado 22
                estado = 22;
                t.cat = SINAL;
                t.codigo = ADICAO;
                return t;
            } else if (c == '-') {              //Estado 0 sai por - para estado 23
                estado = 23;
                t.cat = SINAL;
                t.codigo = SUBTRACAO;
                return t;
            } else if (c == '*') {              //Estado 0 sai por * para estado 24
                estado = 24;
                t.cat = SINAL;
                t.codigo = MULTIPLICACAO;
                return t;
            } else if (c == '/') {          //Comentários ou Divisão (estado 18) 
                c = fgetc(fd);
                if (c == '/') {             
                    while (c != '\n' && c != EOF) {         
                        c = fgetc(fd);
                    }
                    estado = 0;
                    if (c == '\n') {
                        ungetc(c, fd);
                    }
                    return t;
                } else {                
                    estado = 18;
                    t.cat = SINAL;
                    t.codigo = DIVISAO;
                    return t;
                }
            } else if (c == '=') {          //Estado 0 sai por == para estado 27 
                c = fgetc(fd);
                if (c == '=') {
                    estado = 27;
                    t.cat = SINAL;
                    t.codigo = IGUALDADE;       
                    return t;
                } else {                    
                    estado = 26;        //Estado 0 sai por = para estado 26
                    ungetc(c, fd);
                    t.cat = SINAL;
                    t.codigo = ATRIBUICAO;
                    return t;
                }
            } else if (c == '(') {      //Estado 0 sai por ( para esatdo 35
                estado = 35;
                t.cat = SINAL;
                t.codigo = ABRE_PAR;
                return t;
            } else if (c == ')') {      //Estado 0 sai por ) para estado 36
                estado = 36;
                t.cat = SINAL;
                t.codigo = FECHA_PAR;
                return t;
            } else if (c == '{'){       //Estado 0 sai por { para estado 37
                estado = 37;
                t.cat = SINAL;
                t.codigo = ABRE_CHAVE;
                return t;
            } else if (c == '}') {      //Estado 0 sai por  } para estado 38 
                estado = 38;
                t.cat = SINAL;
                t.codigo = FECHA_CHAVE;
                return t;
            } else if (c == '[') {
                estado = 39;
                t.cat = SINAL;
                t.codigo = ABRE_COLCHETE;
                return t;
            } else if (c == ']') {
                estado = 40;
                t.cat = SINAL;
                t.codigo = FECHA_COLCHETE;
                return t;
            } else if (c == '>') {
                c = fgetc(fd);
                if (c == '=') {
                    estado = 43;
                    t.cat = SINAL;
                    t.codigo = MAIOR_IGUAL;
                    return t;
                } else {
                    estado = 41;
                    ungetc(c, fd);
                    t.cat = SINAL;
                    t.codigo = MAIOR_QUE;
                    return t;
                }
            } else if (c == '<') {
                c = fgetc(fd);
                if (c == '=') {
                    estado = 46;
                    t.cat = SINAL;
                    t.codigo = MENOR_IGUAL;
                    return t;
                } else {
                    estado = 45;
                    t.cat = SINAL;
                    t.codigo = MENOR_QUE;
                    return t;
                }
            } else if (c == '!') {
                c = fgetc(fd);
                if (c == '=') {
                    estado = 29;
                    t.cat = SINAL;
                    t.codigo = DIFERENTE;
                    return t;
                } else {
                    estado = 30;
                    t.cat = SINAL;
                    t.codigo = NEGACAO;
                    return t;
                }
            } else if (c == '&') {
                c = fgetc(fd);
                if (c == '&') {
                    estado = 48;
                    t.cat = SINAL;
                    t.codigo = AND;
                    return t;
                } else {
                    estado = 49;
                    ungetc(c, fd);
                    t.cat = SINAL;
                    t.codigo = ENDERECO;
                    return t;
                }
            } else if(c == '|'){
                estado = 50;
                c = fgetc(fd);
                if( c == '|'){
                    estado = 51;
                    ungetc(c, fd);
                    t.cat = SINAL;
                    t.codigo = OR;
                    return t;
                } else {
                    estado = 0;
                    ungetc(c, fd);
                    t.cat = SINAL;
                    t.codigo = PIPE; //  um token para o caractere '|'
                    return t;
                }
            } else if (c == '\'') { // Verifica charcon
                estado = 12;
            } else if (c == '"') { // Verifica stringcon
                estado = 11;
            } else if (c == '\n') {
                estado = 0;
                t.cat = FIM_EXPR;
                contLinha++;
                return t;
            } else if (c == EOF) {
                t.cat = FIM_ARQ;
                return t;
            } else {
                error("ERRO na linha: ", contLinha);
                exit(1);
            }
            break;

        case 1:
            if (ocorrencia_letra(c)) {
                estado = 3;
                lexema[tamL] = c;  
                lexema[++tamL] = '\0'; 
            } else {
                estado = 0;
                ungetc(c, fd);
                if (tamL > 1) { // Verifica se o lexema tem mais de um caractere
                    t.cat = ID;
                    strcpy(t.lexema, lexema);
                    return t;
                } else {
                    error("ERRO na linha: ", contLinha);
                    exit(1);
                }
            }
            break;

        case 3:
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
                estado = 3;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            } else {
                estado = 3;
                ungetc(c, fd);

                // Verifica se o lexema é uma palavra reservada
               if (strcmp("MAIN", lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo = MAIN;
                } else if (strcmp( "DOWNTO", lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = DOWNTO;
                } else if (strcmp("WHILE",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = WHILE;
                } else if (strcmp( "FOR",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = FOR;
                } else if (strcmp("IF",lexema) == 0) {
                    t.codigo  = IF;
                } else if (strcmp("ELSEIF",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = ELSEIF;
                } else if (strcmp("ELSE",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = ELSE;
                } else if (strcmp("ENDIF",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = ENDIF;
                } else if (strcmp("GOBACK",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = GOBACK;
                } else if (strcmp("GETINT",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = GETINT;
                } else if (strcmp("GETCHAR",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = GETCHAR;
                } else if (strcmp("PUTINT",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = PUTINT;
                } else if (strcmp("PUTREAL",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = PUTREAL;
                } else if (strcmp("PUTCHAR",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = PUTCHAR;
                } else if (strcmp("BLOCK",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = BLOCK;
                } else if (strcmp("ENDBLOCK",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = ENDBLOCK;
                } else if (strcmp("CONST",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = CONST;
                } else if (strcmp("CHAR",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = CHAR;
                } else if (strcmp("INT",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = INT;
                } else if (strcmp("REAL",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = REAL;
                } else if (strcmp("BOOL",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = BOOL;
                } else if (strcmp("WITH",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = WITH;
                } else if (strcmp("DO",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = DO;
                } else if (strcmp("VARYING",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = VARYING;
                } else if (strcmp("FROM",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = FROM;
                } else if (strcmp("TO",lexema) == 0) {
                    t.cat = PAL_RESERV;
                    t.codigo  = TO;
                }else{
                    t.cat = ID;
                    
                }
                strcpy(t.lexema,lexema);
                return t;
            }
            break;

        case 4:
            if (ocorrencia_digito(c)) {
                estado = 4;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            } else if (c == '.') {
                estado = 8;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            } else {
                estado = 6;
                ungetc(c, fd);
                // Verifica se o número inteiro tem formato correto
                if (digitos[0] == '0' && digitos[1] != '.') {
                    error("ERRO na linha:  ", contLinha);
                    exit(1);
                }
                t.cat = INTCON;
                t.valor_int = atoi(digitos);
                return t;
            }
            break;

        case 8:
            if (ocorrencia_digito(c)) {
                estado = 8;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            } else if (c == '.') {
                // Verifica se já houve um ponto decimal
                if (strchr(digitos, '.') != NULL) {
                    error("ERRO na linha", contLinha);
                    exit(1);
                }
                estado = 8;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            } else {
                estado = 8;
                ungetc(c, fd);
                // Verifica se o número real tem formato correto
                if (digitos[tamD - 1] == '.' || digitos[0] == '.') {
                    error("ERRO na linha: ", contLinha);
                    exit(1);
                }
                t.cat = REALCON;
                t.valor_double = atof(digitos);
                return t;
            }
            break;
        case 12: // Estado para reconhecer charcon
            if (c == '\'') { // Verifica charcon
                estado = 12;
            } else if (c == '\\') { // Verifica se é uma barra invertida
                estado = 15;
            } else if (isprint(c)) { // Verifica se é um caractere imprimível
                estado = 14;
                lexema[tamL] = c;  
                lexema[++tamL] = '\0'; 
            } else {
                error("ERRO na linha: ", contLinha);
                exit(1);
            }
            break;

        case 15: // Estado para lidar com barra invertida
            if (c == 'n') { // Verifica se é '\n'
                strcpy(lexema, "pular linha");
                estado = 14;
            } else if (c == '0') { // Verifica se é '\0'
                strcpy(lexema, "caractere nulo");
                estado = 14;
            } else {
                error("ERRO na linha: ", contLinha);
                exit(1);
            }
            break;

        case 14:
            if (c == '\'') {
                estado = 0;
                t.cat = CHARCON;
                strcpy(t.lexema, lexema);
                return t;
            } else {
                error("ERRO na linha: ", contLinha);
                exit(1); 
            }
            break;

        case 11: // Estado para reconhecer stringcon
            if (c == '"') {
                if (tamL == 0) { // Se não houver caracteres entre as aspas
                    error("ERRO na linha: ", contLinha);
                    exit(1);
                }
                estado = 0;
                t.cat = STRINGCON;
                strcpy(t.lexema, lexema);
                return t;
            } else if (c == '\n') { // Se encontrar uma quebra de linha antes do fechamento das aspas
                error("ERRO na linha: ", contLinha);
                exit(1);
            } else if (c == '\\') { // Se encontrar uma barra invertida
                estado = 16;
            } else if (isprint(c)) { // Verifica se é um caractere imprimível
                if (tamL < TAM_LEXEMA - 1) {
                    lexema[tamL] = c;  
                    lexema[++tamL] = '\0'; 
                } 
            } else {
                error("ERRO na linha: ", contLinha);
                exit(1);
            }
            break;

        case 16: // Estado para lidar com barra invertida dentro de uma string
            if (c == 'n') { // Verifica se é '\n'
                strcpy(lexema, "pular linha");
                estado = 11; // Volta para o estado de leitura da string
            } else if (c == '0') { // Verifica se é '\0'
                strcpy(lexema, "caractere nulo");
                estado = 11; // Volta para o estado de leitura da string
            } else {
                error("ERRO na linha: ", contLinha);
                exit(1);
            }
            break;

        }
    }
}

int main(){

    FILE *fd;
    TOKEN tk;

    fd = fopen("C:/Users/guilh/OneDrive/Documentos/ANALEX/teste.txt","r");
    

    while (1){
        tk = Analex(fd);
        printf("Linha: %d  ", contLinha);
        switch (tk.cat) {
            case ID: 
                printf("ID: %s\n\n ", tk.lexema);
                break;
/*            case COMENTARIO: printf("COMENTÁRIOS\n\n%s",tk.lexema);
                break;*/
            case STRINGCON: printf("STRINGCON:%s\n\n",tk.lexema);
                break;
            case CHARCON:printf("CHARCON: %s\n\n ",tk.lexema);
                break;
            case PAL_RESERV: printf("PVR: %s\n\n", tk.lexema);
                break;
            case SINAL:
                switch (tk.codigo){
                    case ADICAO: printf("ADICAO\n\n");
                        break;
                    case SUBTRACAO: printf("SUBTRACAO\n\n");
                        break;
                    case MULTIPLICACAO: printf("MULTIPLICACAO\n\n");
                        break;
                    case DIVISAO: printf("DIVISAO\n\n");
                        break;
                    case ATRIBUICAO: printf("ATRIBUICAO\n\n"); 
                        break;
                    case IGUALDADE: printf("IGUALDADE\n\n");
                        break;
                    case ABRE_PAR: printf("ABRE PARENTESE\n\n");
                        break;
                    case FECHA_PAR: printf("FECHA PARENTESE\n\n");
                        break;
                    case PONTO: printf("PONTO\n\n");
                        break;
                    case ABRE_CHAVE: printf("ABRE CHAVE\n\n");
                        break;
                    case FECHA_CHAVE: printf("FECHA CHAVE\n\n");
                        break;
                    case ABRE_COLCHETE: printf("ABRE COLCHETE\n\n");
                        break;
                    case FECHA_COLCHETE: printf("FECHA COLCHETE\n\n");
                        break;
                    case MAIOR_QUE: printf("MAIOR QUE\n\n");
                        break;
                    case MAIOR_IGUAL: printf("MAIOR IGUAL\n\n");
                        break;
                    case MENOR_QUE: printf("MENOR QUE\n\n");
                        break;
                    case MENOR_IGUAL: printf("MENOR IGUAL\n\n");
                        break;
                    case DIFERENTE: printf("DIFERENTE\n\n");
                        break;
                    case NEGACAO: printf("NEGACAO\n\n");
                        break;
                    case AND: printf("AND\n\n");
                        break;
                    case ENDERECO: printf("ENDERECAMENTO\n\n");
                        break;
                    case OR: printf("OR\n\n");
                        break;
                }
                break;
            case INTCON: printf("INTCON: %d\n\n", tk.valor_int);
                break;
            case REALCON: printf("REALCON: %0.2f\n\n", tk.valor_double);
                break;
            case FIM_EXPR: 
                printf("FINAL DA EXPRESSAO\n");
                break;
            case FIM_ARQ:
                printf("FIM DO ARQUIVO\n");
                break;
            default:
                break;
        }
        if (tk.cat == FIM_ARQ)
        {
            break;
        } 
    }

    fclose(fd);

    return 0;
}
