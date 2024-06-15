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

    if(!(t.cat == PAL_RESERV && t.codigo == MAIN)){
        printf("Espera-se bloco main");
    }
    block_main();

    if ((t.cat == PAL_RESERV) && (t.codigo == CONST) 
    | (t.codigo == INT) | (t.codigo == REAL) | (t.codigo == BOOL)
    | (t.codigo == CHAR))
    {
        decl_list_var();
    }
    
    t = Analex(fd);

    if(t.cat == PAL_RESERV && t.codigo ==  ID){
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
}

//char | int | real  | bool
void tipo() {
}

/*
::= id {[ intcon | idconst ]}  //IDCONST 
[ = (intcon | realcon | charcon | stringcon | 
{ (intcon | realcon | charcon | stringcon) {, 
(intcon | realcon | charcon | stringcon) } } ) ] 
*/
void decl_var() {
}

//block id [with [&] tipo { [ ] } { , [&] tipo { [ ] } }] 
void decl_block_prot() {

}

/*
block main 
{decl_list_var} 
{ cmd }  
endblock */
void block_main() {

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
        // Lógica para análise de atribuição
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