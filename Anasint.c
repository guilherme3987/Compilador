#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Analex.h"
#include "Anasint.h"

void prog() {
    //Consumir token
    Analex(fd);

    if ((t.cat = PAL_RESERV) && 
    (t.codigo == CONST)||(t.codigo == INT)||
    (t.codigo == CHAR) ||(t.codigo == REAL)||(t.codigo == BOOL)) 
    {
        decl_list_var();
    }

    if ((!(t.cat == PAL_RESERV) && (t.codigo == BLOCK)))
    {
        printf("Declaração de bloco esperada.");
        
    }
    //Consumir token
    Analex(fd);

    if ((t.cat == PAL_RESERV) &&(t.codigo = ID))
    {
        decl_block_prot();
    }

    if (!(t.cat == PAL_RESERV) && (t.codigo = MAIN))
    {
        printf("Declaração de bloco main esperada. ");
    }
    
    block_main();

    //Consumir token
    Analex(fd);

    
}

void decl_list_var() {

}

void decl_block_prot() {

}
void block_main() {

}
void decl_var() {
}

void tipo() {

}

void block_def() {

}


void cmd() {

}

void atrib() {

}
void expr() {
 
}

void expr_simp() {
 
}


void termo() {

}

void fator() {

}

void op_rel() {

}
