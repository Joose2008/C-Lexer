#ifndef TOKEN.H
#define TOKEN.H

typedef enum {
    // Palabras Reservadas
    TOK_INT, TOK_MAIN, TOK_VOID, TOK_BREAK, TOK_DO, TOK_ELSE,
    TOK_IF, TOK_WHILE, TOK_RETURN, TOK_READ, TOK_WRITE,
 
    /* Simbolos especiales */
    TOK_LBRACE, TOK_RBRACE, TOK_LSQUARE, TOK_RSQUARE,
    TOK_LPAR, TOK_RPAR, TOK_SEMI, TOK_PLUS, TOK_MINUS,
    TOK_MUL_OP, TOK_DIV_OP, TOK_AND_OP, TOK_OR_OP, TOK_NOT_OP,
    TOK_ASSIGN, TOK_LT, TOK_GT, TOK_SHL_OP, TOK_SHR_OP,
    TOK_EQ, TOK_NOTEQ, TOK_LTEQ, TOK_GTEQ, TOK_ANDAND, TOK_OROR,
    TOK_COMMA,
 
    /* Identificadores y números */
    TOK_INT_NUM, TOK_ID,
 
    TOK_EOF,   /* fin de archivo */
    TOK_ERROR  /* caracter no reconocido */
} TokenType;