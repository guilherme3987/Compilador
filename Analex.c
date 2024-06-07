#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "Analex.h"

#define TAM_LEXEMA 31
#define TAM_NUM 20

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
}// Função Analex
TOKEN Analex(FILE *fd) {
    int estado = 0;
    int tamL = 0;
    int tamD = 0;
    char lexema[TAM_LEXEMA] = "";
    char digitos[TAM_NUM] = "";
    char c;
    TOKEN t;

    // Transições do AFD
    while (true) {
        c = fgetc(fd);
        switch (estado) {
        case 0: // Estado 0
            if (ocorrencia_tab_enter(c)) {
                estado = 0;
            } else if (ocorrencia_letra(c)) { // Estado 0 sai por letra para estado 3
                estado = 3;
                lexema[tamL++] = c;
                lexema[tamL] = '\0'; 
            } else if (ocorrencia_underline(c)) { // Estado 0 sai por _ para estado 1
                estado = 1;
                lexema[tamL++] = c;
                lexema[tamL] = '\0'; 
            } else if (ocorrencia_digito(c)) { // Estado 0 sai por digitos para estado 4
                estado = 4;
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            } else if (c == '+') { // Estado 0 sai por + para estado 22
                t.cat = SINAL;
                t.codigo = ADICAO;
                return t;
            } else if (c == '-') { // Estado 0 sai por - para estado 23
                t.cat = SINAL;
                t.codigo = SUBTRACAO;
                return t;
            } else if (c == '*') { // Estado 0 sai por * para estado 24
                t.cat = SINAL;
                t.codigo = MULTIPLICACAO;
                return t;
            } else if (c == ',') {
                t.cat = SINAL;
                t.codigo = VIRGULA;
                return t;
            } else if (c == '/') { // Comentários ou Divisão (estado 18)
                c = fgetc(fd);
                if (c == '/') {
                    while (c != '\n' && c != EOF) { // Comentários são ignorados
                        c = fgetc(fd);
                    }
                    estado = 0;
                    if (c == '\n') {
                        ungetc(c, fd);
                    }
                    return t;
                } else { // Se não for // é / (divisão)
                    estado = 18;
                    t.cat = SINAL;
                    t.codigo = DIVISAO;
                    return t;
                }
            } else if (c == '=') { // Estado 0 sai por == para estado 27
                c = fgetc(fd);
                if (c == '=') {
                    t.cat = SINAL;
                    t.codigo = IGUALDADE;
                    return t;
                } else { // Estado 0 sai por = para estado 26
                    ungetc(c, fd);
                    t.cat = SINAL;
                    t.codigo = ATRIBUICAO;
                    return t;
                }
            } else if (c == '(') { // Estado 0 sai por ( para estado 35
                t.cat = SINAL;
                t.codigo = ABRE_PAR;
                return t;
            } else if (c == ')') { // Estado 0 sai por ) para estado 36
                t.cat = SINAL;
                t.codigo = FECHA_PAR;
                return t;
            } else if (c == '{') { // Estado 0 sai por { para estado 37
                t.cat = SINAL;
                t.codigo = ABRE_CHAVE;
                return t;
            } else if (c == '}') { // Estado 0 sai por } para estado 38
                t.cat = SINAL;
                t.codigo = FECHA_CHAVE;
                return t;
            } else if (c == '[') {
                t.cat = SINAL;
                t.codigo = ABRE_COLCHETE;
                return t;
            } else if (c == ']') {
                t.cat = SINAL;
                t.codigo = FECHA_COLCHETE;
                return t;
            } else if (c == '>') {
                c = fgetc(fd);
                if (c == '=') {
                    t.cat = SINAL;
                    t.codigo = MAIOR_IGUAL;
                    return t;
                } else {
                    ungetc(c, fd);
                    t.cat = SINAL;
                    t.codigo = MAIOR_QUE;
                    return t;
                }
            } else if (c == '<') {
                c = fgetc(fd);
                if (c == '=') {
                    t.cat = SINAL;
                    t.codigo = MENOR_IGUAL;
                    return t;
                } else {
                    ungetc(c, fd);
                    t.cat = SINAL;
                    t.codigo = MENOR_QUE;
                    return t;
                }
            } else if (c == '!') {
                c = fgetc(fd);
                if (c == '=') {
                    t.cat = SINAL;
                    t.codigo = DIFERENTE;
                    return t;
                } else {
                    t.cat = SINAL;
                    t.codigo = NEGACAO;
                    return t;
                }
            } else if (c == '&') {
                c = fgetc(fd);
                if (c == '&') {
                    t.cat = SINAL;
                    t.codigo = AND;
                    return t;
                } else {
                    ungetc(c, fd);
                    t.cat = SINAL;
                    t.codigo = ENDERECO;
                    return t;
                }
            } else if (c == '|') {
                c = fgetc(fd);
                if (c == '|') {
                    t.cat = SINAL;
                    t.codigo = OR;
                    return t;
                } else {
                    error("ERRO na linha: ", contLinha);
                    exit(1);
                }
            } else if (c == '\'') { // Verifica charcon
                estado = 12;
            } else if (c == '"') { // Verifica stringcon
                estado = 11;
            } else if (c == '\n') {
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
                lexema[tamL++] = c;
                lexema[tamL] = '\0'; 
            } else if (ocorrencia_underline(c)) {
                estado = 1;
                lexema[tamL++] = c;
                lexema[tamL] = '\0'; 
            } else if (ocorrencia_digito(c)) {
                estado = 3;
                lexema[tamL++] = c;
                lexema[tamL] = '\0'; 
            } else {
                error("ERRO na linha: ", contLinha);
                exit(1);
            }
            break;

        case 3: // Verifica palavras reservadas e identificadores
            if (ocorrencia_letra(c) || ocorrencia_underline(c) || ocorrencia_digito(c)) {
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
                estado = 3;
            }else {
                estado = 3;
                ungetc(c, fd);
                if (strcmp(lexema, "const") == 0) {
                    char proxima[5]; // Adiciona espaço para manter "const int"
                    fscanf(fd, "%4s", proxima);
                    if (strcmp(proxima, "int") == 0) {
                        t.cat = IDCONST;
                        strcat(lexema, " "); 
                        strcpy(t.lexema, proxima);
                    }
                }else {
                    ungetc(c, fd);
                    if (strcmp(lexema, "main") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = MAIN;
                    } else if (strcmp(lexema, "downto") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = DOWNTO;
                    } else if (strcmp(lexema, "while") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = WHILE;
                    } else if (strcmp(lexema, "for") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = FOR;
                    } else if (strcmp(lexema, "if") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = IF;
                    } else if (strcmp(lexema, "elseif") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = ELSEIF;
                    } else if (strcmp(lexema, "else") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = ELSE;
                    } else if (strcmp(lexema, "endif") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = ENDIF;
                    } else if (strcmp(lexema, "goback") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = GOBACK;
                    } else if (strcmp(lexema, "getint") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = GETINT;
                    } else if (strcmp(lexema, "getchar") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = GETCHAR;
                    } else if (strcmp(lexema, "putint") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = PUTINT;
                    } else if (strcmp(lexema, "putreal") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = PUTREAL;
                    } else if (strcmp(lexema, "putchar") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = PUTCHAR;
                    } else if (strcmp(lexema, "block") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = BLOCK;
                    } else if (strcmp(lexema, "endblock") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = ENDBLOCK;
                    } else if (strcmp(lexema, "const") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = CONST;
                    } else if (strcmp(lexema, "char") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = CHAR;
                    } else if (strcmp(lexema, "int") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = INT;
                    } else if (strcmp(lexema, "real") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = REAL;
                    } else if (strcmp(lexema, "bool") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = BOOL;
                    } else if (strcmp(lexema, "with") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = WITH;
                    } else if (strcmp(lexema, "do") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = DO;
                    } else if (strcmp(lexema, "varying") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = VARYING;
                    } else if (strcmp(lexema, "from") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = FROM;
                    } else if (strcmp(lexema, "to") == 0) {
                        t.cat = PAL_RESERV;
                        t.codigo = TO;
                    } else {
                        t.cat = ID;
                        strcpy(t.lexema, lexema);
                    }
                }strcpy(t.lexema, lexema);
                return t;
            }
            break;

        case 4: // Verifica constantes inteiras e reais
            if (ocorrencia_digito(c)) {
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
                estado = 4;
            } else if (c == '.') {
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
                estado = 5; // Possível número real
            } else {
                ungetc(c, fd);
                t.cat = INTCON;
                t.valor_int = atoi(digitos);
                return t;
            }
            break;

        case 5: // Verifica a parte fracionária do número real
            if (ocorrencia_digito(c)) {
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
                estado = 6;
            } else {
                error("Número real mal formado na linha: ", contLinha);
            }
            break;

        case 6: // Completa a leitura do número real
            if (ocorrencia_digito(c)) {
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
                estado = 6;
            } else {
                ungetc(c, fd);
                t.cat = REALCON;
                t.valor_double = atof(digitos);
                return t;
            }
            break;

        case 11: // Verifica string constants
            if (c == '"') {
                t.cat = STRINGCON;
                strcpy(t.lexema, lexema);
                return t;
            } else if (c == EOF) {
                error("String não terminada na linha: ", contLinha);
            } else {
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
                estado = 11;
            }
            break;

        case 12: // Verifica char constants
            if (c == '\'') {
                if (tamL == 1) {
                    t.cat = CHARCON;
                    t.lexema[0] = lexema[0];
                    t.lexema[1] = '\0';
                    return t;
                } else {
                    error("Char mal formado na linha: ", contLinha);
                }
            } else if (c == EOF) {
                error("Char não terminado na linha: ", contLinha);
            } else {
                if (tamL == 0) {
                    lexema[tamL++] = c;
                    estado = 12;
                } else {
                    error("Char mal formado na linha: ", contLinha);
                }
            }
            break;

        default:
            error("ERRO na linha: ", contLinha);
        }
    }
}