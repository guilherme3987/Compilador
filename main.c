#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Analex.h"
#include <stdlib.h>

#define TAM_LEXEMA 31
#define TAM_NUM 20
int contLinha = 1;


void error(char msg[], int linha) {
    printf("%s%d\n", msg, linha);
    exit(1);
}

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

bool eh_imprimivel(char c) {
    return c >= 32 && c <= 126; // Caracteres imprimíveis na tabela ASCII vão de 32 a 126
}

TOKEN Analex(FILE *fd) {
    int estado = 0;
    int tamL = 0;
    int tamD = 0;
    char lexema[TAM_LEXEMA] = "";
    char digitos[TAM_NUM] = "";
    char c;
    TOKEN t;

    while (1) {
        c = fgetc(fd);
        switch (estado) {
        case 0:
            if (ocorrencia_tab_enter(c)) {
                estado = 0;
            } else if (ocorrencia_letra(c)) {
                estado = 3;
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
            } else if (ocorrencia_underline(c)) {
                estado = 1;
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
            } else if (ocorrencia_digito(c)) {
                estado = 4;
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            } else if (c == '+') {
                estado = 22;
                t.cat = SINAL;
                t.codigo = ADICAO;
                return t;
            } else if (c == '-') {
                estado = 23;
                t.cat = SINAL;
                t.codigo = SUBTRACAO;
                return t;
            } else if (c == '*') {
                estado = 24;
                t.cat = SINAL;
                t.codigo = MULTIPLICACAO;
                return t;
            } else if (c == '/') { //comentário
                c = fgetc(fd);
                if (c == '/') {
                    while (c != '\n') {
                        c = fgetc(fd);
                    }
                    estado = 0;
                    return t;
                } else {
                    estado = 18;
                    t.cat = SINAL;
                    t.codigo = DIVISAO;
                    return t;
                }
            } else if (c == '=') {
                c = fgetc(fd);
                if (c == '=') {
                    estado = 27;
                    t.cat = SINAL;
                    t.codigo = IGUALDADE;
                    return t;
                } else {
                    estado = 26;
                    ungetc(c, fd);
                    t.cat = SINAL;
                    t.codigo = ATRIBUICAO;
                    return t;
                }
            } else if (c == '(') {
                estado = 35;
                t.cat = SINAL;
                t.codigo = ABRE_PAR;
                return t;
            } else if (c == ')') {
                estado = 36;
                t.cat = SINAL;
                t.codigo = FECHA_PAR;
                return t;
            } else if (c == '{') {
                estado = 37;
                t.cat = SINAL;
                t.codigo = ABRE_CHAVE;
                return t;
            } else if (c == '}') {
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
                t.codigo = PIPE; // Adicione um token para o caractere '|'
                return t;
                }
            } else if (c == '\'') { // Verifica charcon
                estado = 52;
            } else if (c == '"') { // Verifica stringcon
                estado = 53;
            } else if (c == '\n') {
                estado = 0;
                t.cat = FIM_EXPR;
                contLinha++;
                return t;
            } else if (c == EOF) {
                t.cat = FIM_ARQ;
                return t;
            } else {
                printf("ERRO na linha: %d\n", contLinha);
                exit(1);
            }
            break;

        case 1:
            if (ocorrencia_letra(c)) {
                estado = 3;
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
            } else {
                estado = 0;
                ungetc(c, fd);
                if (tamL > 1) { // Verifica se o lexema tem mais de um caractere
                    t.cat = ID;
                    strcpy(t.lexema, lexema);
                    return t;
                } else {
                    printf("ID invalido");

                }
            }
            break;


        case 3:
            if (ocorrencia_letra(c) || ocorrencia_digito(c) || ocorrencia_underline(c)) {
                estado = 3;
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
                
            } else {
                if(!(ocorrencia_letra(c) || ocorrencia_digito(c) || ocorrencia_underline(c))){
                    printf("Id invalido na linha %d\n", contLinha);
                    
                }
                estado = 0;
                ungetc(c, fd);
                t.cat = ID;
                strcpy(t.lexema, lexema);
                return t;
            }
            break;

        case 4:
            if (ocorrencia_digito(c)) {
                estado = 4;
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            } else if (c == '.') {
                estado = 8;
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            } else {
                estado = 6;
                ungetc(c, fd);
                t.cat = INTCON;
                t.valor_int = atoi(digitos);
                return t;
            }
            break;

        case 8:
            if (ocorrencia_digito(c)) {
                estado = 8;
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            } else {
                estado = 8;
                ungetc(c, fd);
                t.cat = REALCON;
                t.valor_double = atof(digitos);
                return t;
            }
            break;
        case 53: // Estado para reconhecer stringcon
            if (c == '"') {
                estado = 0;
                t.cat = STRINGCON;
                strcpy(t.lexema, lexema);
                return t;
            } else if (isprint(c) && c != '\n' && c != '"') {
                if (tamL < TAM_LEXEMA - 1) {
                    lexema[tamL++] = c;
                    lexema[tamL] = '\0';
                } 
            } else {
                printf("ERRO na linha: %d\n", contLinha);
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
                    default:
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
