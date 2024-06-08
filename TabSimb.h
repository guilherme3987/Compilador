#ifndef TABSIMB
#define TABSIM

#include <stdio.h>
#include <stdlib.h>
#include "Analex.h"

#define TAM_MAX_TAB 100
#define TAM_MAX_DIM 10

//Definindo estrutura de entrada da  tabela
typedef
    struct 
    {
        /* data */
        char lexema[TAM_MAX_LEXEMA];
        int tipo;
        int escopo;
        char categoria[3]; //VG,FUNC,PAR
        bool zombie;

        int dimensoes[2][TAM_MAX_DIM];

    }TabSimb;

//Definindo estrututra da tabela de 
typedef
    struct
    {
        /* data */
        int tamTab;
        TabSimb tabela_simb[TAM_MAX_TAB];
    }TabIdf;
    
extern TabIdf tabela_idf;
extern TOKEN t;

//Declarando procedimentos
void Iniciar_tabela();
int Buscar_tabela(char lexema[]);
int Buscar_escopo(char lexema[], int escopo);
int Insercao_tabela(char lexema[], int escopo, int tipo, char categoria[3], bool zombie);
int Remover_ultimo();

#endif