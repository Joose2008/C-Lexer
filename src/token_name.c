#include "token.h"

/* Tabla de nombres para diagnóstico: cada TokenType se traduce a su
 * nombre corto ("INT", "EQ", ...). Los tipos desconocidos devuelven "ERROR". */
const char *getTokenTypeName(TokenType type) {
    switch (type) {
        /* Keywords -> Palabras reservadas (originales) */
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

        /* Keywords -> Palabras reservadas (adicionales) */
        case TOK_AUTO: return "AUTO";
        case TOK_CHAR: return "CHAR";
        case TOK_CONST: return "CONST";
        case TOK_CONTINUE: return "CONTINUE";
        case TOK_DEFAULT: return "DEFAULT";
        case TOK_DOUBLE: return "DOUBLE";
        case TOK_EXTERN: return "EXTERN";
        case TOK_FLOAT: return "FLOAT";
        case TOK_FOR: return "FOR";
        case TOK_GOTO: return "GOTO";
        case TOK_LONG: return "LONG";
        case TOK_REGISTER: return "REGISTER";
        case TOK_SHORT: return "SHORT";
        case TOK_SIGNED: return "SIGNED";
        case TOK_SIZEOF: return "SIZEOF";
        case TOK_STATIC: return "STATIC";
        case TOK_STRUCT: return "STRUCT";
        case TOK_SWITCH: return "SWITCH";
        case TOK_TYPEDEF: return "TYPEDEF";
        case TOK_UNION: return "UNION";
        case TOK_UNSIGNED: return "UNSIGNED";
        case TOK_VOLATILE: return "VOLATILE";
        case TOK_CASE: return "CASE";
        case TOK_ENUM: return "ENUM";

        /* Directivas de preprocesador (Agregadas) */
        case TOK_PREPROC: return "PREPROC";
        case TOK_PREPROC_DEFINE: return "PREPROC_DEFINE";
        case TOK_PREPROC_ELIF: return "PREPROC_ELIF";
        case TOK_PREPROC_ELSE: return "PREPROC_ELSE";
        case TOK_PREPROC_ENDIF: return "PREPROC_ENDIF";
        case TOK_PREPROC_ERROR: return "PREPROC_ERROR";
        case TOK_PREPROC_IF: return "PREPROC_IF";
        case TOK_PREPROC_IFDEF: return "PREPROC_IFDEF";
        case TOK_PREPROC_IFNDEF: return "PREPROC_IFNDEF";
        case TOK_PREPROC_INCLUDE: return "PREPROC_INCLUDE";
        case TOK_PREPROC_MESSAGE: return "PREPROC_MESSAGE";
        case TOK_PREPROC_UNDEF: return "PREPROC_UNDEF";

        /* Caracteres de escape (Agregados si los tokenizas por separado) */
        case TOK_ESC_NEWLINE: return "ESC_NEWLINE";     /* \n */
        case TOK_ESC_NULL: return "ESC_NULL";           /* \0 */
        case TOK_ESC_CR: return "ESC_CR";               /* \r */
        case TOK_ESC_TAB: return "ESC_TAB";             /* \t */
        case TOK_ESC_BACKSLASH: return "ESC_BACKSLASH"; /* \\ */
        case TOK_ESC_VTAB: return "ESC_VTAB";           /* \v */
        case TOK_ESC_FORMFEED: return "ESC_FORMFEED";   /* \f */
        case TOK_ESC_BELL: return "ESC_BELL";           /* \a */
        case TOK_ESC_DQUOTE: return "ESC_DQUOTE";       /* \" */
        case TOK_ESC_SQUOTE: return "ESC_SQUOTE";       /* \' */

        /* Símbolos especiales */
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
        case TOK_COLON: return "COLON";

        /* Operadores de bits adicionales */
        case TOK_XOR_OP: return "XOR_OP";
        case TOK_COMPL_OP: return "COMPL_OP";

        /* Operadores adicionales */
        case TOK_DOT: return "DOT";
        case TOK_ARROW: return "ARROW";

        /* Literales */
        case TOK_STRING_LIT: return "STRING_LIT";
        case TOK_CHAR_LIT: return "CHAR_LIT";

        /* Identificadores y números */
        case TOK_INT_NUM: return "INT_NUM";
        case TOK_ID: return "ID";

        /* Control */
        case TOK_EOF: return "EOF";
        case TOK_ERROR: return "ERROR";

        default: return "ERROR";
    }
}