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

int Insercao_tabela(char lexema[], int escopo, int tipo, char categoria[3], bool zombie)
{
    return 0;
}

int Remover_ultimo()
{
    return 0;
}
