#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
/* Inicializa el escaner con el código fuente */
void scaner_init(const char *source_code);
/*Devuelve el siguiente token 
 *Al llegar al final del texto, devuelve TOK_EOF */
Token scaner_get_next_token();


#endif