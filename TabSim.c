#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TabSimb.h"

void Iniciar_tabela()
{
    tabela_idf.tamTab = 0;
}

int Buscar_tabela(char lexema[])
{
    for(int i = 0; i <tabela_idf.tabela_simb; i++){
        if(!(strcmp(lexema,tabela_idf.tabela_simb[i].lexema))){
            return i;
        }
    }
    //Não encontrado
    return -1;
}

int Buscar_escopo(char lexema[], int escopo)
{
    int i;

    for ( i = 0; i < tabela_idf.tamTab; i++)
    {
        /* code */
        if (!(strcmp(lexema, tabela_idf.tabela_simb[i].lexema)))
        {
            /* code */
            return i;
        }    
    }
    
    //Não encontrado
    return -1;
}

int Insercao_tabela(char lexema[], int escopo, int tipo, char categoria[3], bool zombie) {
    if (tabela_idf.tamTab >= TAM_MAX_TAB) {
        // Tabela cheia, não é possível inserir mais elementos
        return -1;
    }

    // Verifica se o lexema já existe na tabela
    if (Buscar_tabela(lexema) != -1) {
        // Lexema já existe na tabela
        return -2;
    }

    // Inserir novo símbolo na tabela
    strcpy(tabela_idf.tabela_simb[tabela_idf.tamTab].lexema, lexema);
    tabela_idf.tabela_simb[tabela_idf.tamTab].escopo = escopo;
    tabela_idf.tabela_simb[tabela_idf.tamTab].tipo = tipo;
    strcpy(tabela_idf.tabela_simb[tabela_idf.tamTab].categoria, categoria);
    tabela_idf.tabela_simb[tabela_idf.tamTab].zombie = zombie;

    // Incrementa o tamanho da tabela
    tabela_idf.tamTab++;

    return 0; // Inserção bem-sucedida
}

int Remover_ultimo() {
    if (tabela_idf.tamTab <= 0) {
        // Tabela está vazia, não há elementos para remover
        return -1;
    }

    // Decrementa o tamanho da tabela para "remover" o último elemento
    tabela_idf.tamTab--;

    return 0; // Remoção bem-sucedida
}
