#ifndef ANASINT
#define ANASINT

#include "Analex.h"
#include <stdbool.h>
#include <stdio.h>

//variaveis globais

extern TOKEN t;
extern FILE *fd;
extern int contLinha;

//Declarando procedimentos
void prog();
void decl_list_var();
void decl_block_prot();
void block_main();
void decl_var();
void tipo();
void block_def();
void cmd();
void atrib();
void expr();
void expr_simp();
void termo();
void fator();
void op_rel();



#endif