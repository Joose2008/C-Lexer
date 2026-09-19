#ifndef SCANNER_H
#define SCANNER_H

/* scanner.h — Interfaz pública del analizador léxico.
 * El fuente se carga una vez como cadena en memoria; cada llamada
 * a scaner_get_next_token() devuelve el siguiente token por valor. */

#include "token.h"

/* Carga el código fuente a analizar. El puntero debe seguir válido
 * mientras se extraigan tokens (no se hace copia interna). */
void scaner_init(const char *source_code);

/* Devuelve el siguiente token del flujo.
 * Al llegar al final del texto, devuelve TOK_EOF. */
Token scaner_get_next_token();


#endif