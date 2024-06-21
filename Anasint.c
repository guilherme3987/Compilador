#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Analex.h"
#include "Anasint.h"

void consome(int esperado) {
    if (t.codigo == esperado) {
        t = Analex(fd); // Avança para o próximo token
    } else {
        printf("Erro: token inesperado. Esperava-se %d\n", esperado);
        exit(EXIT_FAILURE);
    }
}



/*
decl_list_var =>  id {[ intcon | idconst ]}  //IDCONST 
[ = (intcon | realcon | charcon | stringcon | 
{ (intcon | realcon | charcon | stringcon) {, 
(intcon | realcon | charcon | stringcon) } } ) ] 
*/

//::= {decl_list_var} {decl_block_prot}  block_main {block_def} 
void prog() {
   t = Analex(fd);
    
    while (t.cat == PAL_RESERV && (t.codigo == CONST || t.codigo == INT || t.codigo == REAL || t.codigo == BOOL || t.codigo == CHAR)) {
        decl_list_var();
    }

    while (t.cat == PAL_RESERV && t.codigo == BLOCK) {
        decl_block_prot();
    }

    if (t.cat == PAL_RESERV && t.codigo == MAIN) {
        block_main();
    } 

    while (t.cat == PAL_RESERV && t.codigo == BLOCK) {
        block_def();
    }


}

//[const] tipo decl_var { , decl_var}
void decl_list_var() {
     if (t.codigo == CONST) {
        consome(CONST);
    }
    tipo();
    decl_var();

    while (t.codigo == VIRGULA) {
        consome(VIRGULA);
        decl_var();
    }
}

//char | int | real  | bool
void tipo() {
    if (t.codigo == CHAR || t.codigo == INT || t.codigo == REAL || t.codigo == BOOL) {
        consome(t.codigo);
    } else {
        printf("Erro: tipo inválido.\n");
    }
}

/*
id {[ intcon | idconst ]}  //IDCONST 
[ = (intcon | realcon | charcon | stringcon | 
{ (intcon | realcon | charcon | stringcon) {, 
(intcon | realcon | charcon | stringcon) } } ) ] 

*/
void decl_var() {
    consome(ID);

    if (t.codigo == ABRE_COL) {
        consome(ABRE_COL);
        if (t.cat == INTCON || t.cat == IDCONST) {
            consome(t.cat);
        }
        consome(FECHA_COL);
    }

    if (t.codigo == ATRIBUICAO) {
        consome(ATRIBUICAO);
        if (t.cat == INTCON || t.cat == REALCON || t.cat == CHARCON || t.cat == STRINGCON) {
            consome(t.cat);
        } else if (t.codigo == ABRE_CHAVE) {
            consome(ABRE_CHAVE);
            do {
                if (t.cat == INTCON || t.cat == REALCON || t.cat == CHARCON || t.cat == STRINGCON) {
                    consome(t.cat);
                }
                if (t.codigo == VIRGULA) {
                    consome(VIRGULA);
                } else {
                    break;
                }
            } while (1);
            consome(FECHA_CHAVE);
        } else {
            printf("Erro: valor esperado após '='.\n");
        }
    }
    
}

//block id [with [&] tipo { [ ] } { , [&] tipo { [ ] } }] 
void decl_block_prot() {
    consome(BLOCK);
    consome(ID);

    if (t.codigo == WITH) {
        consome(WITH);

        if (t.codigo == E_COMERCIAL) {
            consome(E_COMERCIAL);
        }
        tipo();
        consome(ID);

        if (t.codigo == ABRE_COL) {
            consome(ABRE_COL);
            consome(FECHA_COL);
        }

        while (t.codigo == VIRGULA) {
            consome(VIRGULA);
            if (t.codigo == E_COMERCIAL) {
                consome(E_COMERCIAL);
            }
            tipo();
            consome(ID);
            if (t.codigo == ABRE_COL) {
                consome(ABRE_COL);
                consome(FECHA_COL);
            }
        }
    }
}


/*
block main 
{decl_list_var} 
{ cmd }  
endblock */
void block_main() {
 consome(BLOCK);
    consome(MAIN);

    while (t.cat == PAL_RESERV && (t.codigo == CONST || t.codigo == INT || t.codigo == CHAR || t.codigo == REAL || t.codigo == BOOL)) {
        decl_list_var();
    }

    while (t.codigo != ENDBLOCK) {
        cmd();
    }
    consome(ENDBLOCK);
        
}

/*
block id [with tipo id1 { [intcon1 | idconst1] } 
{ , tipo  id2 { [intcon2 | idconst2] } }]  
{decl_list_var} { cmd }  endblock 
*/
void block_def() {
  consome(BLOCK);
    consome(ID);

    if (t.codigo == WITH) {
        consome(WITH);
        tipo();
        consome(ID);

        if (t.codigo == ABRE_COL) {
            consome(ABRE_COL);
            if (t.cat == INTCON || t.cat == IDCONST) {
                consome(t.cat);
            }
            consome(FECHA_COL);
        }

        while (t.codigo == VIRGULA) {
            consome(VIRGULA);
            tipo();
            consome(ID);
            if (t.codigo == ABRE_COL) {
                consome(ABRE_COL);
                if (t.cat == INTCON || t.cat == IDCONST) {
                    consome(t.cat);
                }
                consome(FECHA_COL);
            }
        }
    }

    while (t.cat == PAL_RESERV && (t.codigo == CONST || t.codigo == INT || t.codigo == CHAR || t.codigo == REAL || t.codigo == BOOL)) {
        decl_list_var();
    }

    while (t.codigo != ENDBLOCK) {
        cmd();
    }
    consome(ENDBLOCK);
}

//atrib ::= id { [ expr ] } = expr  
void atrib() {
    consome(ID);
    while (t.codigo == ABRE_COL) {
        consome(ABRE_COL);
        expr();
        consome(FECHA_COL);
    }
    consome(ATRIBUICAO);
    expr();
}


/*expr ::= expr_simp [ op_rel  expr_simp ]
op_rel ::= IGUALDADE, DIFERENTE, MAIOR_IGUAL, 
MENOR_IGUAL, MAIOR_QUE, MENOR_QUE
*/
void expr() {
    expr_simp();
    if (t.codigo == IGUALDADE || t.codigo == DIFERENTE || t.codigo == MENOR_IGUAL ||
        t.codigo == MENOR_QUE || t.codigo == MAIOR_IGUAL || t.codigo == MAIOR_QUE) {
        op_rel();
        expr_simp();
    }
}


//[+ | – ] termo {(+ | – | ||) termo} 
void expr_simp() {
    if (t.codigo == ADICAO || t.codigo == SUBTRACAO) {
        consome(t.codigo);
    }
    termo();
    while (t.codigo == ADICAO || t.codigo == SUBTRACAO || t.codigo == OR) {
        consome(t.codigo);
        termo();
    }
}



//fator {(* OU / OU &&)  fator} 
void termo() {
    fator();
    while (t.codigo == MULTIPLICACAO || t.codigo == DIVISAO || t.codigo == AND) {
        consome(t.codigo);
        fator();
    }
}

/*
id { [ expr ] }  | intcon | realcon | charcon |  ( expr )  | ! fator 
*/
void fator() {
    if (t.cat == ID) {
        consome(ID);
        while (t.codigo == ABRE_COL) {
            consome(ABRE_COL);
            expr();
            consome(FECHA_COL);
        }
    } else if (t.cat == INTCON) {
        consome(INTCON);
    } else if (t.cat == REALCON) {
        consome(REALCON);
    } else if (t.cat == CHARCON) {
        consome(CHARCON);
    } else if (t.codigo == ABRE_PAR) {
        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);
    } else if (t.codigo == DIFERENTE) {
        consome(DIFERENTE);
        fator();
    } else {
        printf("Erro: fator inválido.\n");
    }    
}

/*
== ou != ou <= ou < ou >= ou >
*/
void op_rel() {
       if (t.codigo == IGUALDADE || t.codigo == DIFERENTE || t.codigo == MENOR_IGUAL ||
        t.codigo == MENOR_QUE || t.codigo == MAIOR_IGUAL || t.codigo == MAIOR_QUE) {
        consome(t.codigo);
    } else {
        printf("Erro: operador relacional esperado.\n");
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
void cmd() {
    if (t.cat == PAL_RESERV) {
        switch (t.codigo) {
            case DO:
                consome(DO);
                if (t.cat == ID) {
                    consome(ID);
                    if (t.codigo == WITH) {
                        consome(WITH);
                        consome(ID);
                        while (t.codigo == VIRGULA) {
                            consome(VIRGULA);
                            consome(ID);
                        }
                    }
                    if (t.codigo == VARYING) {
                        consome(VARYING);
                        consome(ID);
                        consome(FROM);
                        expr();
                        if (t.codigo == TO) {
                            consome(TO);
                        } else if (t.codigo == DOWNTO) {
                            consome(DOWNTO);
                        } else {
                            printf("Erro: esperado 'to' ou 'downto'.\n");
                        }
                        expr();
                    } else if (t.codigo == WHILE) {
                        consome(WHILE);
                        consome(ABRE_PAR);
                        expr();
                        consome(FECHA_PAR);
                    } else if (t.codigo == FOR) {
                        consome(FOR);
                        expr();
                    }
                } else {
                    cmd();
                    if (t.codigo == VARYING) {
                        consome(VARYING);
                        consome(ID);
                        consome(FROM);
                        expr();
                        if (t.codigo == TO) {
                            consome(TO);
                        } else if (t.codigo == DOWNTO) {
                            consome(DOWNTO);
                        } else {
                            printf("Erro: esperado 'to' ou 'downto'.\n");
                        }
                        expr();
                    } else if (t.codigo == WHILE) {
                        consome(WHILE);
                        consome(ABRE_PAR);
                        expr();
                        consome(FECHA_PAR);
                    } else if (t.codigo == FOR) {
                        consome(FOR);
                        expr();
                    }
                }
                break;

            case IF:
                consome(IF);
                consome(ABRE_PAR);
                expr();
                consome(FECHA_PAR);
                consome(ABRE_CHAVE);
                cmd();
                consome(FECHA_CHAVE);
                while (t.codigo == ELSEIF) {
                    consome(ELSEIF);
                    consome(ABRE_PAR);
                    expr();
                    consome(FECHA_PAR);
                    consome(ABRE_CHAVE);
                    cmd();
                    consome(FECHA_CHAVE);
                }
                if (t.codigo == ELSE) {
                    consome(ELSE);
                    consome(ABRE_CHAVE);
                    cmd();
                    consome(FECHA_CHAVE);
                }
                consome(ENDIF);
                break;

            case WHILE:
                consome(WHILE);
                consome(ABRE_PAR);
                expr();
                consome(FECHA_PAR);
                consome(ABRE_CHAVE);
                cmd();
                consome(FECHA_CHAVE);
                consome(ENDWHILE);
                break;

            case GOBACK:
                consome(GOBACK);
                break;

            case GETINT:
                consome(GETINT);
                consome(ID);
                break;

            case GETREAL:
                consome(GETREAL);
                consome(ID);
                break;

            case GETCHAR:
                consome(GETCHAR);
                consome(ID);
                break;

            case PUTINT:
                consome(PUTINT);
                consome(ID);
                break;

            case PUTREAL:
                consome(PUTREAL);
                consome(ID);
                break;

            case PUTCHAR:
                consome(PUTCHAR);
                consome(ID);
                break;

            default:
                printf("Erro: comando inválido.\n");
        }
    } else if (t.cat == ID) {
        atrib();
    } else {
        printf("Erro: comando inválido.\n");
    }
}
