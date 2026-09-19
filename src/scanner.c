#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "../include/token.h"
#include "../include/scanner.h"

/* scanner.c — Implementación del AFD del analizador léxico.
 * S0 despacha según el primer carácter; S1/S2 acumulan identificadores
 * y números; S3/S4/S5 atienden directivas y literales; S6 resuelve
 * operadores con lookahead de un carácter (maximal munch). */

/* Fuente en memoria y posición de lectura. El buffer pertenece al
 * llamador (ver scaner_init): aquí solo se guarda el puntero. */
static const char *buffer;
static int position = 0;

/* --- Funciones Auxiliares de Lectura --- */

/* Devuelve el caracter actual sin avanzar */
static char peek(void) {
    return buffer[position];
}

/* Devuelve el caracter actual y avanza a la siguiente posición */
static char advance(void) {
    return buffer[position++];
}

/* Inicializa el analizador léxico */
void scaner_init(const char *input) {
    buffer = input;
    position = 0;
}

/* --- Tabla de Palabras Reservadas --- */
/* Incluye las keywords de C más scanf/printf (como READ/WRITE) y main,
 * que el proyecto trata como tokens propios en lugar de identificadores. */
static const KeywordEntry Keywords[] = {
    {"auto", TOK_AUTO}, {"double", TOK_DOUBLE}, {"int", TOK_INT}, {"struct", TOK_STRUCT},
    {"break", TOK_BREAK}, {"else", TOK_ELSE}, {"long", TOK_LONG}, {"switch", TOK_SWITCH},
    {"case", TOK_CASE}, {"enum", TOK_ENUM}, {"register", TOK_REGISTER}, {"typedef", TOK_TYPEDEF},
    {"char", TOK_CHAR}, {"extern", TOK_EXTERN}, {"return", TOK_RETURN}, {"union", TOK_UNION},
    {"const", TOK_CONST}, {"float", TOK_FLOAT}, {"short", TOK_SHORT}, {"unsigned", TOK_UNSIGNED},
    {"continue", TOK_CONTINUE}, {"for", TOK_FOR}, {"signed", TOK_SIGNED}, {"void", TOK_VOID},
    {"default", TOK_DEFAULT}, {"goto", TOK_GOTO}, {"sizeof", TOK_SIZEOF}, {"volatile", TOK_VOLATILE},
    {"do", TOK_DO}, {"if", TOK_IF}, {"static", TOK_STATIC}, {"while", TOK_WHILE},
    /* Funciones estándar originalmente consideradas como keywords en el proyecto */
    {"scanf", TOK_READ}, {"printf", TOK_WRITE}, {"main", TOK_MAIN}
};

#define KEYWORD_TABLE_SIZE (sizeof(Keywords) / sizeof(Keywords[0]))

/* Verifica si un lexema es una palabra reservada o un identificador */
TokenType getTokenType(const char *lexeme) {
    for (size_t i = 0; i < KEYWORD_TABLE_SIZE; i++) {
        if (strcmp(lexeme, Keywords[i].word) == 0) {
            return Keywords[i].type;
        }
    }
    return TOK_ID; // Sin coincidencia: es un identificador
}

/* Ignora los espacios en blanco, saltos de línea y tabulaciones */
static void skipWhitespace(void) {
    while (isspace(peek())) {
        advance();
    }
}

/* --- Transiciones de Estado del AFD --- */

/* Estado S1 -> Identificador o Palabra Reservada */
static Token scanIdentifierOrKeyword(void) {
    Token token;
    token.type = TOK_ID;
    int start = position;

    // Transición: Avanza mientras sean letras, números o '_'
    while (isalnum(peek()) || peek() == '_') {
        advance();
    }

    int length = position - start;
    if (length >= (int)sizeof(token.lexeme)) {
        length = sizeof(token.lexeme) - 1; // Truncamiento para evitar desbordamiento
    }
    strncpy(token.lexeme, &buffer[start], length);
    token.lexeme[length] = '\0';

    // Se verifica si es keyword para asignar el token específico
    token.type = getTokenType(token.lexeme);
    
    return token;
}

/* Estado S2 -> Número Entero */
static Token scanInteger(void) {
    Token token;
    token.type = TOK_INT_NUM;
    int start = position;

    // Transición: Avanza mientras sean dígitos numéricos
    while (isdigit(peek())) {
        advance();
    }

    int length = position - start;
    if (length >= (int)sizeof(token.lexeme)) {
        length = sizeof(token.lexeme) - 1;
    }
    strncpy(token.lexeme, &buffer[start], length);
    token.lexeme[length] = '\0';

    return token;
}

/* (2) Directivas de Preprocesamiento */
/* Estado S3 -> Detecta '#' y la directiva correspondiente.
 * Limitación: exige '#' pegado al nombre ("#include"); con un espacio
 * intermedio ("# include") devuelve TOK_ERROR. */
static Token scan_preproc(void) {
    Token tok;
    tok.type = TOK_ERROR; // Por defecto error, a menos que sea una directiva válida
    int start = position;

    advance(); // Consumir '#'

    // Leer el identificador de la directiva
    while (isalpha(peek())) {
        advance();
    }

    int len = position - start;
    if (len >= (int)sizeof(tok.lexeme)) {
        len = sizeof(tok.lexeme) - 1;
    }
    strncpy(tok.lexeme, &buffer[start], len);
    tok.lexeme[len] = '\0';

    // Verificar explícitamente si es una de las directivas admitidas.
    // Cada una produce su token específico; lo no listado queda TOK_ERROR.
    const char *directive = tok.lexeme + 1; // Avanzar el puntero después de '#'
    if (strcmp(directive, "define") == 0) {
        tok.type = TOK_PREPROC_DEFINE;
    } else if (strcmp(directive, "elif") == 0) {
        tok.type = TOK_PREPROC_ELIF;
    } else if (strcmp(directive, "else") == 0) {
        tok.type = TOK_PREPROC_ELSE;
    } else if (strcmp(directive, "endif") == 0) {
        tok.type = TOK_PREPROC_ENDIF;
    } else if (strcmp(directive, "error") == 0) {
        tok.type = TOK_PREPROC_ERROR;
    } else if (strcmp(directive, "if") == 0) {
        tok.type = TOK_PREPROC_IF;
    } else if (strcmp(directive, "ifdef") == 0) {
        tok.type = TOK_PREPROC_IFDEF;
    } else if (strcmp(directive, "ifndef") == 0) {
        tok.type = TOK_PREPROC_IFNDEF;
    } else if (strcmp(directive, "include") == 0) {
        tok.type = TOK_PREPROC_INCLUDE;
    } else if (strcmp(directive, "message") == 0) {
        tok.type = TOK_PREPROC_MESSAGE;
    } else if (strcmp(directive, "undef") == 0) {
        tok.type = TOK_PREPROC_UNDEF;
    }

    return tok;
}

/* Estado S4 -> Cadenas de texto "..." (con secuencias de escape) */

/* Conjunto de escapes válidos (ver TOK_ESC_* en token.h).
 * El literal conserva el texto crudo; aquí solo se valida. */
static int is_valid_escape(char c) {
    return c == 'n' || c == '0' || c == 'r' || c == 't' ||
           c == '\\' || c == 'v' || c == 'f' || c == 'a' ||
           c == '"' || c == '\'';
}

static Token scan_string(void) {
    Token tok;
    tok.type = TOK_STRING_LIT;
    int start = position;

    advance(); // Consumir '"' de apertura

    // Transición: Leer hasta encontrar la comilla de cierre o fin de línea/archivo
    while (peek() != '\0' && peek() != '\n' && peek() != '\r') {
        if (peek() == '\\') {
            advance(); // Consumir barra invertida de escape
            if (peek() == '\0' || peek() == '\n' || peek() == '\r') {
                break; // Escape cortado por fin de línea/archivo: error abajo
            }
            advance(); // Consumir el caracter escapado
            if (!is_valid_escape(buffer[position - 1])) {
                // Escape inexistente (ej. \q): el literal es inválido
                tok.type = TOK_ERROR;
                int len = position - start;
                if (len >= (int)sizeof(tok.lexeme)) {
                    len = sizeof(tok.lexeme) - 1;
                }
                strncpy(tok.lexeme, &buffer[start], len);
                tok.lexeme[len] = '\0';
                return tok;
            }
        } else if (peek() == '"') {
            advance(); // Consumir '"' de cierre
            int len = position - start;
            if (len >= (int)sizeof(tok.lexeme)) {
                len = sizeof(tok.lexeme) - 1;
            }
            strncpy(tok.lexeme, &buffer[start], len);
            tok.lexeme[len] = '\0';
            return tok; // Cadena válida
        } else {
            advance();
        }
    }

    // Transición de Error: No se cerró el literal
    tok.type = TOK_ERROR;
    int len = position - start;
    if (len >= (int)sizeof(tok.lexeme)) {
        len = sizeof(tok.lexeme) - 1;
    }
    strncpy(tok.lexeme, &buffer[start], len);
    tok.lexeme[len] = '\0';
    return tok;
}

/* Estado S5 -> Literales de carácter '...' (con secuencias de escape) */
static Token scan_char_lit(void) {
    Token tok;
    tok.type = TOK_CHAR_LIT;
    int start = position;

    advance(); // Consumir '\'' de apertura

    while (peek() != '\0' && peek() != '\n' && peek() != '\r') {
        if (peek() == '\\') {
            advance(); // Consumir '\'
            if (peek() == '\0' || peek() == '\n' || peek() == '\r') {
                break; // Escape cortado por fin de línea/archivo: error abajo
            }
            advance(); // Consumir caracter escapado
            if (!is_valid_escape(buffer[position - 1])) {
                // Escape inexistente (ej. \q): el literal es inválido
                tok.type = TOK_ERROR;
                int len = position - start;
                if (len >= (int)sizeof(tok.lexeme)) {
                    len = sizeof(tok.lexeme) - 1;
                }
                strncpy(tok.lexeme, &buffer[start], len);
                tok.lexeme[len] = '\0';
                return tok;
            }
        } else if (peek() == '\'') {
            advance(); // Consumir '\'' de cierre
            int len = position - start;
            if (len >= (int)sizeof(tok.lexeme)) {
                len = sizeof(tok.lexeme) - 1;
            }
            strncpy(tok.lexeme, &buffer[start], len);
            tok.lexeme[len] = '\0';
            return tok;
        } else {
            advance();
        }
    }

    // Transición de Error: No se cerró el literal
    tok.type = TOK_ERROR;
    int len = position - start;
    if (len >= (int)sizeof(tok.lexeme)) {
        len = sizeof(tok.lexeme) - 1;
    }
    strncpy(tok.lexeme, &buffer[start], len);
    tok.lexeme[len] = '\0';
    return tok;
}

/* Estado S6 -> Símbolos y operadores con lookahead (Maximal Munch):
 * ante un prefijo ambiguo ("<", "=", "&", ...) se mira el siguiente
 * carácter para preferir el token compuesto ("<=", "==", "&&", ...). */
static Token scan_symbol(void){
    Token tok; 
    char current = peek();
    int start = position;
    advance(); // Consumir el primer caracter siempre

    switch(current){
        /* Delimitadores de agrupación */
        case '{': tok.type = TOK_LBRACE; break;
        case '}': tok.type = TOK_RBRACE; break;
        case '[': tok.type = TOK_LSQUARE; break;
        case ']': tok.type = TOK_RSQUARE; break;
        case '(': tok.type = TOK_LPAR; break;
        case ')': tok.type = TOK_RPAR; break;
        
        /* Separadores y operadores simples de un carácter */
        case ';': tok.type = TOK_SEMI; break;
        case ',': tok.type = TOK_COMMA; break;
        case '.': tok.type = TOK_DOT; break;
        case ':': tok.type = TOK_COLON; break;
        
        /* Aritméticos simples */
        case '+': tok.type = TOK_PLUS; break;
        case '*': tok.type = TOK_MUL_OP; break;
        case '/': tok.type = TOK_DIV_OP; break;
        
        /* Operadores de bits de un carácter */
        case '^': tok.type = TOK_XOR_OP; break;
        case '~': tok.type = TOK_COMPL_OP; break;

        /* --- Símbolos compuestos (evalúan el siguiente caracter con peek) --- */
        
        /* Flecha (->) o menos (-) */
        case '-':
            if (peek() == '>') {
                advance();
                tok.type = TOK_ARROW;
            } else {
                tok.type = TOK_MINUS;
            }
            break;

        /* Igualdad (==) o asignación (=) */
        case '=':
            if (peek() == '=') {
                advance();
                tok.type = TOK_EQ;
            } else {
                tok.type = TOK_ASSIGN;
            }
            break;
        
        /* Desplazamiento (<<), menor-igual (<=) o menor (<) */
        case '<':
            if (peek() == '<') {
                advance();
                tok.type = TOK_SHL_OP;
            } else if (peek() == '=') {
                advance();
                tok.type = TOK_LTEQ;
            } else {
                tok.type = TOK_LT;
            }
            break;

        /* Desplazamiento (>>), mayor-igual (>=) o mayor (>) */
        case '>':
            if (peek() == '>') {
                advance();
                tok.type = TOK_SHR_OP;
            } else if (peek() == '=') {
                advance();
                tok.type = TOK_GTEQ;
            } else {
                tok.type = TOK_GT;
            }
            break;

        /* Distinto (!=) o negación lógica (!) */
        case '!':
            if (peek() == '=') {
                advance();
                tok.type = TOK_NOTEQ;
            } else {
                tok.type = TOK_NOT_OP;
            }
            break;
        
        /* AND lógico (&&) o AND de bits (&) */
        case '&':
            if (peek() == '&') {
                advance();
                tok.type = TOK_ANDAND;
            } else {
                tok.type = TOK_AND_OP;
            }
            break; 

        /* OR lógico (||) u OR de bits (|) */
        case '|':
            if (peek() == '|') {
                advance();
                tok.type = TOK_OROR;
            } else {
                tok.type = TOK_OR_OP;
            }
            break;

        /* Token Desconocido */
        default:
            tok.type = TOK_ERROR;
            break;
    }

    // Copiar el lexema completo identificado
    int len = position - start;
    if (len >= (int)sizeof(tok.lexeme)) {
        len = sizeof(tok.lexeme) - 1; 
    }
    strncpy(tok.lexeme, &buffer[start], len);
    tok.lexeme[len] = '\0';

    return tok;
}

/* --- Función Principal del Analizador Léxico --- */
/* Estado S0 -> Estado Inicial del AFD */
Token scaner_get_next_token(void) {
    skipWhitespace();

    char current = peek();
    
    // Fin del fuente: no hay más tokens
    if (current == '\0') {
        Token token;
        token.type = TOK_EOF;
        token.lexeme[0] = '\0';
        return token;
    }

    // Letra o '_' deriva hacia identificadores y palabras reservadas
    if (isalpha(current) || current == '_') {
        return scanIdentifierOrKeyword();
    }

    // Dígito deriva a números enteros
    if (isdigit(current)) {
        return scanInteger();
    }

    // '#' deriva a directiva de preprocesamiento
    if (current == '#') {
        return scan_preproc();
    }

    // '"' deriva a literal de cadena
    if (current == '"') {
        return scan_string();
    }

    // '\'' deriva a literal de carácter
    if (current == '\'') {
        return scan_char_lit();
    }

    // (7) Transición por defecto: Operadores, agrupación y otros símbolos
    // Si no coincide con ninguno, scan_symbol devuelve TOK_ERROR
    return scan_symbol();
}