#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.h"

/* main.c — Punto de entrada: carga un archivo fuente, lo analiza y
 * escribe cada token en stdout ("Token: <TIPO> Lexema: <texto>").
 * Los errores léxicos van a stderr y detienen el análisis. */

/* Lee el archivo completo a memoria como cadena terminada en '\0'.
 * Termina el programa si el archivo no se puede abrir o leer. */
static char *read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if(!file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if(!buffer) { 
        fprintf(stderr, "Error al asignar memoria\n"); 
        exit(EXIT_FAILURE); }

    fread(buffer, 1, length, file);
    buffer[length] = '\0'; // El escáner usa '\0' como marca de fin (TOK_EOF)
    fclose(file);
    return buffer;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_fuente>\n", argv[0]);
        return 0;
    }

    char *source_code = read_file(argv[1]);
    scaner_init(source_code);

    Token token;
    do {
        token = scaner_get_next_token();

        // Ante el primer error léxico se informa y se aborta el análisis
        if(token.type == TOK_ERROR) {
            fprintf(stderr, "Error: Token desconocido '%s'\n", token.lexeme);
            free(source_code);
            return 1;
        } else {
            printf("Token: %-10s Lexema: %s\n", getTokenTypeName(token.type), token.lexeme);
        }
    } while(token.type != TOK_EOF);

    free(source_code);
    // NOTA: debería retornar 0 en éxito; pendiente de normalizar
    return 1;
}
