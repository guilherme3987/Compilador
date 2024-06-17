#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Analex.h"
#include "Anasint.h"
#include "TabSimb.h"
TabIdf tabela_idf; // Definição da variável global

TOKEN t;
FILE *fd;
int contLinha;

//teste léxico
void teste_analex(){

    FILE *fd;

    //arquivo para teste léxico fd = fopen("C:/Users/guilh/OneDrive/Documentos/Analex_Anasint/teste_analex.dat","r");
    
    //teste sintático
    fd = fopen("C:/Users/guilh/OneDrive/Documentos/Analex_Anasint/teste_anasint.dat","r");

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
                    case E_COMERCIAL: printf("<E_COMERCIAL>\n");
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

//teste sintático
void teste_ana_sint(){
    contLinha = 0;
    
    fd = fopen("C:/Users/guilh/OneDrive/Documentos/Analex_Anasint/teste_anasint.dat","r");

    while (true)
    {
        t = Analex(fd);
        prog();
        if (t.cat == FIM_ARQ)
        {
            printf("\nFim do arquivo de teste\n");
            break;
        }else{
            
            printf("\nExpressão correta na linha: %d", contLinha+1);
        }
        
        
    }
    fclose(fd);
    
}
// Implementação da função para imprimir a tabela de símbolos
void Imprimir_tabela() {
    printf("\n=== Tabela de Símbolos ===\n");

    if (tabela_idf.tamTab == 0) {
        printf("A tabela de símbolos está vazia.\n");
        return;
    }

    printf("Índice | Lexema        | Tipo | Escopo | Categoria | Zombie\n");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < tabela_idf.tamTab; i++) {
        printf("%6d | %13s | %4d | %6d | %9s | %s\n",
               i,
               tabela_idf.tabela_simb[i].lexema,
               tabela_idf.tabela_simb[i].tipo,
               tabela_idf.tabela_simb[i].escopo,
               tabela_idf.tabela_simb[i].categoria,
               tabela_idf.tabela_simb[i].zombie ? "true" : "false");
    }

    printf("\nTotal de entradas na tabela: %d\n", tabela_idf.tamTab);
}

int main() {
    printf("[=========== Análise léxica ============]\n");
    teste_analex();
    
    printf("[========== Análise sintática ==========]\n");
    teste_ana_sint();

    // Após a análise sintática, imprimir a tabela de símbolos
    Imprimir_tabela();

    return 0;
}

