#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Analex.h"
#include "Anasint.h"
/*
decl_list_var =>  id {[ intcon | idconst ]}  //IDCONST 
[ = (intcon | realcon | charcon | stringcon | 
{ (intcon | realcon | charcon | stringcon) {, 
(intcon | realcon | charcon | stringcon) } } ) ] 
*/

//::= {decl_list_var} {decl_block_prot}  block_main {block_def} 
void prog() {
    t = Analex(fd);

    if((t.cat == PAL_RESERV && t.codigo == MAIN)){
        block_main();
    }

    if ((t.cat == PAL_RESERV) && (t.codigo == CONST) 
    | (t.codigo == INT) | (t.codigo == REAL) | (t.codigo == BOOL)
    | (t.codigo == CHAR))
    {
        decl_list_var();
    }
    
    t = Analex(fd);

    if(t.cat  ==  ID){
        decl_block_prot();

    }

    if(t.cat == PAL_RESERV && t.codigo == BLOCK){
        block_def();
    }

    if(t.cat != FIM_ARQ){
        printf("Fim do programa");
    }

}

//[const] tipo decl_var { , decl_var}
void decl_list_var() {
    t = Analex(fd);
    
    if (!(t.cat == PAL_RESERV && t.codigo == CONST))
    {
        tipo();
        decl_var();
    }


    while (t.cat == SINAIS && t.codigo == VIRGULA)
    {
        decl_var();
    }
    
    

}

//char | int | real  | bool
void tipo() {
    if (!(t.cat == PAL_RESERV && (t.cat == CHAR || 
    t.cat == INT || t.cat == REAL || t.cat == BOOL)))
    {
        printf("Espera-se tipo");
    }
    
}

/*
id {[ intcon | idconst ]}  //IDCONST 
[ = (intcon | realcon | charcon | stringcon | 
{ (intcon | realcon | charcon | stringcon) {, 
(intcon | realcon | charcon | stringcon) } } ) ] 

*/
void decl_var() {

    t = Analex(fd);

    if (t.cat == ID)
    {
        if (t.cat == SINAIS && t.codigo == ABRE_COL)
        {
            t = Analex(fd);

            if (t.cat == PAL_RESERV && 
            (t.codigo == IDCONST || t.codigo == CONST ))
            {
                t = Analex(fd);

                if (t.cat == PAL_RESERV && t.codigo == FECHA_COL)
                {
                    t = Analex(fd);
                }else printf("Espera-se fechar coluna");
                
            }
            
        }else printf("Espera-se uma constante inteira");

        if (t.cat == SINAIS && t.codigo == ATRIBUICAO) {
            t = Analex(fd);

            if (t.cat == INTCON || t.cat == REALCON || t.cat == CHARCON || t.cat == STRINGCON) {
                t = Analex(fd);

                while (t.cat == SINAIS && t.codigo == VIRGULA) {
                    t = Analex(fd);

                    if (t.cat == INTCON || t.cat == REALCON || t.cat == CHARCON || t.cat == STRINGCON) {
                        t = Analex(fd);
                    } else {
                        printf("Esperava-se um valor após a vírgula  ");
                    }
                }
            } else {
                printf("Esperava-se um valor após o sinal de atribuição  ");
            }
        }
    }else printf("Espera-se identificador");
    
    
}

//block id [with [&] tipo { [ ] } { , [&] tipo { [ ] } }] 
void decl_block_prot() {
    t = Analex(fd);

    if(t.cat == ID && (t.cat == PAL_RESERV  && t.codigo == BLOCK)){

        t = Analex(fd);

        if (t.cat == PAL_RESERV && t.codigo == WITH)
        {
            while (true)
            {
                if (t.cat == SINAIS && t.codigo == E_COMERCIAL)
                {
                    t = Analex(fd);
                }
                tipo();

                if (t.cat == SINAIS && t.codigo == ABRE_COL)
                {
                    t = Analex(fd);
                }
                if (t.cat == SINAIS && t.codigo == VIRGULA)
                {
                    t = Analex(fd);
                }   
            }   
        }
    }else printf("Espera-se block e identificadores");
}

/*
block main 
{decl_list_var} 
{ cmd }  
endblock */
void block_main() {
    t = Analex(fd);
    
    if (t.cat == PAL_RESERV && 
    (t.codigo == BLOCK && t.codigo == MAIN))
    {
        decl_list_var();
        cmd();
    }
    t = Analex(fd);
    if (t.cat == PAL_RESERV && t.codigo == ENDBLOCK)
    {
        printf("Fim do bloco main");
    }
    
        
}

/*
block id [with tipo id1 { [intcon1 | idconst1] } 
{ , tipo  id2 { [intcon2 | idconst2] } }]  
{decl_list_var} { cmd }  endblock 
*/
void block_def() {

}

//atrib ::= id { [ expr ] } = expr  
void atrib() {
    t = Analex(fd);

    if (t.cat == ID) {
        //  análise de atribuição
        if (t.codigo == ATRIBUICAO) {
            expr();  // Analisar expressão à direita da atribuição
        } else {
            printf("Erro: esperado operador de atribuição '='\n");
        }
    } else {
        printf("Erro: esperado identificador para atribuição\n");
    }
}


/*expr ::= expr_simp [ op_rel  expr_simp ]
op_rel ::= IGUALDADE, DIFERENTE, MAIOR_IGUAL, 
MENOR_IGUAL, MAIOR_QUE, MENOR_QUE
*/
void expr() {
    expr_simp();

    t = Analex(fd);
    if (t.cat == SINAIS && (t.codigo == IGUALDADE 
    || t.codigo == DIFERENTE   || t.codigo == MENOR_IGUAL 
    || t.codigo == MAIOR_IGUAL || t.codigo == MENOR_QUE 
    || t.codigo == MAIOR_QUE)) {
        expr_simp();  // Analisar a segunda parte da expressão
    }
}


//[+ | – ] termo {(+ | – | ||) termo} 
void expr_simp() {
    t = Analex(fd);
    while(t.cat == SINAIS &&
    (t.codigo == ADICAO || t.codigo == SUBTRACAO)){
        termo();
    }
    t = Analex(fd);
    while (t.cat == SINAIS &&  
    (t.codigo == AND || t.codigo == OR)) {
        termo();
        t = Analex(fd);
    }
}



//fator {(* OU / OU &&)  fator} 
void termo() {
    fator();

    t = Analex(fd);
    while (t.cat == SINAIS && (t.codigo == MULTIPLICACAO 
    || t.codigo == DIVISAO || t.codigo == AND)) {
        fator();
        t = Analex(fd);
    }
}

/*
id { [ expr ] }  | intcon | realcon | charcon |  ( expr )  | ! fator 
*/
void fator() {

}

/*
== ou != ou <= ou < ou >= ou >
*/
void op_rel() {
    t = Analex(fd);

    if (t.cat == SINAIS && 
    (t.codigo == IGUALDADE  || t.codigo == DIFERENTE || 
    t.codigo == MENOR_IGUAL || t.codigo == MENOR_QUE || 
    t.codigo == MAIOR_IGUAL || t.codigo == MAIOR_QUE)) {
        t = Analex(fd);
    } else {
        printf("Esperava-se um operador relacional ");
    }
}




/*
do (id [with id1 { , id2 }]  | cmd ) varying idx from expr1 
(to | downto) expr2  | do (id [with id1 { , id2 }]  | cmd ) 
while ( expr ) 
| do (id [with id1 { , id2 }]  | cmd ) [for expr times ] 
| if ( expr ) { cmd }{ elseif  ( expr ) { cmd } } [ else { cmd }] endif 
| while ( expr ) { cmd } endwhile 
| atrib   | goback  | getint id | getreal id  | getchar id  | putint id | putreal id  | putchar id       
*/
void cmd(){

}