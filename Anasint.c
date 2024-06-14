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
    if(t.cat == PAL_RESERV && (t.codigo == CONST 
    ||t.codigo == IDCONST|| t.codigo == INT || t.codigo == CHAR ||t.codigo == REAL)){
        printf("Foram declaradas variáveis");
    }else{
        printf("..");
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
 
}

//expr ::= expr_simp [ op_rel  expr_simp ] 
void expr() {
 
}

//[+ | – ] termo {(+ | – | ||) termo} 
void expr_simp() {
 
}

//fator {(* OU / OU &&)  fator} 
void termo() {
 
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