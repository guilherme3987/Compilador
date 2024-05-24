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
    while (true) {
        c = fgetc(fd);
        switch (estado) {
        case 0:                             //Estado 0
            if (ocorrencia_tab_enter(c)) {
                estado = 0;
            } else if ( ocorrencia_letra(c) ) {       //Estado 0 sai por letra para estado 3
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
                    while (c != '\n' && c != EOF) { //Comentários são ignoradaos        
                        c = fgetc(fd);
                    }
                    estado = 0;
                    if (c == '\n') {
                        ungetc(c, fd);
                    }
                    return t;
                } else {            // Se não for // é / (divisão)           
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
            } else if (c == '|') {
                c = fgetc(fd);
                if (c == '|') {
                    estado = 51;
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
/*
Lógica para const int
if (strcmp(lexema, "const") == 0) {
    // Verificar se o próximo token é "int"
    char proximo[4]; // "int\0" tem 4 caracteres
    fscanf(fd, "%3s", proximo); // Ler o próximo token
    if (strcmp(proximo, "int") == 0) {
        t.cat = IDCONST;
        strcat(lexema, " "); // Adiciona espaço para manter "const int" como uma unidade lexical
        strcat(lexema, proximo);
    }
}
*/
        case 3:
            if (ocorrencia_underline(c) || ocorrencia_letra(c) || ocorrencia_digito(c) ) {
                estado = 3;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            } else {
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
                }else{
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
                        t.codigo  = PAL_RESERV;
                        t.codigo = IF;
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
            }else if( ocorrencia_underline(c)||ocorrencia_letra(c) ){
                error("Erro na linha: %d",contLinha);
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
                error("Erro %d", contLinha);
                exit(1);
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
                if (tamL == 0) { // Se não houver caracteres entre as aspas ""
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
