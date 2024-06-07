#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Analex.h"
#include "Anasint.h"

TOKEN t;
FILE *fd;
int contLinha;
char TAB[200] = "";


void teste_analex(){

    FILE *fd;

    fd = fopen("C:/Users/guilh/OneDrive/Documentos/Analex_Anasint/teste.dat","r");
    

    while (1){
        t = Analex(fd);
        printf("Linha: %d  ", contLinha);
        switch (t.cat) {
            case ID: 
                printf("<ID><%s>\n", t.lexema);
                break;
/*            case COMENTARIO: printf("COMENTÁRIOS\n\n%s",t.lexema);
                break;*/
            case STRINGCON: printf("<STRINGCON><%s>\n",t.lexema);
                break;
            case CHARCON:printf("<CHARCON><%s>\n ",t.lexema);
                break;
            case PAL_RESERV: printf("<PVR><%s>\n", t.lexema);
                break;
            case IDCONST: printf("<CONST_INT> %s\n", t.lexema);
                break;
            case SINAL:
                switch (t.codigo){
                    case ADICAO: printf("<ADICAO>\n");
                        break;
                    case SUBTRACAO: printf("<SUBTRACAO>\n");
                        break;
                    case MULTIPLICACAO: printf("<MULTIPLICACAO>\n");
                        break;
                    case DIVISAO: printf("<DIVISAO>\n");
                        break;
                    case ATRIBUICAO: printf("<ATRIBUICAO>\n"); 
                        break;
                    case IGUALDADE: printf("<IGUALDADE>\n");
                        break;
                    case ABRE_PAR: printf("<ABRE PARENTESE>\n");
                        break;
                    case FECHA_PAR: printf("<FECHA PARENTESE>\n");
                        break;
                    case PONTO: printf("<PONTO>\n");
                        break;
                    case ABRE_CHAVE: printf("<ABRE CHAVE>\n");
                        break;
                    case FECHA_CHAVE: printf("<FECHA CHAVE>\n");
                        break;
                    case ABRE_COLCHETE: printf("<ABRE COLCHETE>\n");
                        break;
                    case FECHA_COLCHETE: printf("<FECHA COLCHETE>\n");
                        break;
                    case MAIOR_QUE: printf("<MAIOR QUE>\n");
                        break;
                    case MAIOR_IGUAL: printf("<MAIOR IGUAL>\n");
                        break;
                    case MENOR_QUE: printf("<MENOR QUE>\n");
                        break;
                    case MENOR_IGUAL: printf("<MENOR IGUAL>\n");
                        break;
                    case DIFERENTE: printf("<DIFERENTE>\n");
                        break;
                    case NEGACAO: printf("<NEGACAO>\n");
                        break;
                    case AND: printf("<AND>\n");
                        break;
                    case ENDERECO: printf("<ENDERECAMENTO>\n");
                        break;
                    case OR: printf("<OR>\n");
                        break;
                }
                break;
            case INTCON: printf("<INTCON><%d>\n", t.valor_int);
                break;
            case REALCON: printf("<REALCON>%0.2f\n", t.valor_double);
                break;
            case FIM_EXPR: 
                printf("<FINAL DA EXPRESSAO>\n");
                break;
            case FIM_ARQ:
                printf("<FIM DO ARQUIVO>\n");
                break;
            default:
                break;
        }
        if (t.cat == FIM_ARQ)
        {
            break;
        } 
    }

    fclose(fd);

}


void teste_ana_sint(){}

int main(){
    
    printf("[=========== Análise léxica ============]\n");
    teste_analex();

    
    printf("[========== Análise sintática ==========]\n");
    teste_ana_sint();

    return 0;

}