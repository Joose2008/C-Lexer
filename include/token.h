#ifndef TOKEN_H
#define TOKEN_H

/* token.h — Vocabulario del analizador léxico.
 * Define los tipos de token, el contenedor Token y la tabla
 * que distingue palabras reservadas de identificadores. */

typedef enum {
    // Palabras reservadas (originales)
    TOK_INT, TOK_MAIN, TOK_VOID, TOK_BREAK, TOK_DO, TOK_ELSE,
    TOK_IF, TOK_WHILE, TOK_RETURN, TOK_READ, TOK_WRITE,

    // Palabras reservadas (adicionales)
    TOK_AUTO, TOK_CHAR, TOK_CONST, TOK_CONTINUE, TOK_DEFAULT,
    TOK_DOUBLE, TOK_EXTERN, TOK_FLOAT, TOK_FOR, TOK_GOTO,
    TOK_LONG, TOK_REGISTER, TOK_SHORT, TOK_SIGNED, TOK_SIZEOF,
    TOK_STATIC, TOK_STRUCT, TOK_SWITCH, TOK_TYPEDEF, TOK_UNION,
    TOK_UNSIGNED, TOK_VOLATILE, TOK_CASE, TOK_ENUM,

    // Directivas de preprocesador.
    // El escáner emite la variante específica (TOK_PREPROC_INCLUDE, ...);
    // TOK_PREPROC queda como genérico de reserva.
    TOK_PREPROC,
    TOK_PREPROC_DEFINE,
    TOK_PREPROC_ELIF,
    TOK_PREPROC_ELSE,
    TOK_PREPROC_ENDIF,
    TOK_PREPROC_ERROR,
    TOK_PREPROC_IF,
    TOK_PREPROC_IFDEF,
    TOK_PREPROC_IFNDEF,
    TOK_PREPROC_INCLUDE,
    TOK_PREPROC_MESSAGE,
    TOK_PREPROC_UNDEF,

    // Caracteres de escape.
    // NOTA: se consumen dentro de los literales (S4/S5) y nunca se emiten
    // como tokens independientes; definen el conjunto de escapes válidos
    // que el escáner acepta dentro de strings y chars.
    TOK_ESC_NEWLINE,     // \n
    TOK_ESC_NULL,        // \0
    TOK_ESC_CR,          // \r
    TOK_ESC_TAB,         // \t
    TOK_ESC_BACKSLASH,   /* backslash */
    TOK_ESC_VTAB,        // \v
    TOK_ESC_FORMFEED,    // \f
    TOK_ESC_BELL,        // \a
    TOK_ESC_DQUOTE,      // \"
    TOK_ESC_SQUOTE,      // \'

    // Simbolos especiales
    TOK_LBRACE, TOK_RBRACE, TOK_LSQUARE, TOK_RSQUARE,
    TOK_LPAR, TOK_RPAR, TOK_SEMI, TOK_PLUS, TOK_MINUS,
    TOK_MUL_OP, TOK_DIV_OP, TOK_AND_OP, TOK_OR_OP, TOK_NOT_OP,
    TOK_ASSIGN, TOK_LT, TOK_GT, TOK_SHL_OP, TOK_SHR_OP,
    TOK_EQ, TOK_NOTEQ, TOK_LTEQ, TOK_GTEQ, TOK_ANDAND, TOK_OROR,
    TOK_COMMA, TOK_COLON,

    // Operadores de bits adicionales
    TOK_XOR_OP, TOK_COMPL_OP,

    // Operadores adicionales
    TOK_DOT, TOK_ARROW,

    // Literales
    TOK_STRING_LIT, TOK_CHAR_LIT,

    // Identificadores y números
    TOK_INT_NUM, TOK_ID,

    // Control
    TOK_EOF,
    TOK_ERROR
} TokenType;

/* Unidad mínima de salida del escáner: tipo + texto original.
 * El lexema se trunca a 255 caracteres si excede el buffer. */
typedef struct {
    TokenType type;
    char lexeme[256]; // Tamaño máximo del lexema (ampliado para strings)
} Token;

/* Entrada de la tabla de palabras reservadas. */
typedef struct {
    const char *word;
    TokenType type;
} KeywordEntry;

/* Clasifica un lexema: devuelve el token de la palabra reservada
 * correspondente, o TOK_ID si no figura en la tabla. */
TokenType getTokenType(const char *lexeme);

/* Nombre legible del tipo de token (TOK_INT -> "INT"), para diagnóstico. */
const char *getTokenTypeName(TokenType type);

#endif