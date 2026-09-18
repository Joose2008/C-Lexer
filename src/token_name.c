#include "token.h"

const char *getTokenTypeName(TokenType type) {
    switch (type) {
        /*Keywords -> Palabras reservadas*/
        case TOK_INT: return "INT";
        case TOK_MAIN: return "MAIN";
        case TOK_VOID: return "VOID";
        case TOK_BREAK: return "BREAK";
        case TOK_DO: return "DO";
        case TOK_ELSE: return "ELSE";
        case TOK_IF: return "IF";
        case TOK_WHILE: return "WHILE";
        case TOK_RETURN: return "RETURN";
        case TOK_READ: return "READ";
        case TOK_WRITE: return "WRITE";
        /* Simbolos especiales */
        case TOK_LBRACE: return "LBRACE";
        case TOK_RBRACE: return "RBRACE";
        case TOK_LSQUARE: return "LSQUARE";
        case TOK_RSQUARE: return "RSQUARE";
        case TOK_LPAR: return "LPAR";
        case TOK_RPAR: return "RPAR";
        case TOK_SEMI: return "SEMI";
        case TOK_PLUS: return "PLUS";
        case TOK_MINUS: return "MINUS";
        case TOK_MUL_OP: return "MUL_OP";
        case TOK_DIV_OP: return "DIV_OP";
        case TOK_AND_OP: return "AND_OP";
        case TOK_OR_OP: return "OR_OP";
        case TOK_NOT_OP: return "NOT_OP";
        case TOK_ASSIGN: return "ASSIGN";
        case TOK_LT: return "LT";
        case TOK_GT: return "GT";
        case TOK_SHL_OP: return "SHL_OP";
        case TOK_SHR_OP: return "SHR_OP";
        case TOK_EQ: return "EQ";
        case TOK_NOTEQ: return "NOTEQ";
        case TOK_LTEQ: return "LTEQ";
        case TOK_GTEQ: return "GTEQ";
        case TOK_ANDAND: return "ANDAND";
        case TOK_OROR: return "OROR";
        case TOK_COMMA: return "COMMA";

        /* Identificadores y números */
        case TOK_INT_NUM: return "INT_NUM";
        case TOK_ID: return "ID";

        /*Control*/
        case TOK_EOF:   return "EOF";
        
        default: return "ERROR";
    }
}
