# Scanner de C simplificado — COMPG2 Tarea 1

Analizador léxico manual (sin Lex/Flex) para el subconjunto de C definido en el enunciado. Implementado en **C**, modelando el reconocimiento de tokens como un único **AFD** (autómata finito determinista) con un estado inicial que se ramifica según el primer carácter leído.

## Estructura del proyecto

```
C-Lexer/
├── Makefile             # (Pendiente) Reglas de compilación y limpieza
├── include/
│   ├── token.h          # Definición de TokenType, struct Token y tabla de keywords
│   └── scanner.h        # Declaración de funciones del escáner (get_next_token, init, etc.)
├── src/
│   ├── scanner.c        # (Pendiente) Implementación del AFD y sub-estados (S0, S1, S2, S3)
│   ├── token_name.c     # Conversión de TokenType a texto para visualización
│   └── main.c           # (Pendiente) Lectura de archivo de entrada y bucle principal
├── tests/               # (Pendiente) Casos de prueba (.c)
└── informe.pdf          # (Pendiente) Informe técnico con las preguntas solicitadas
```

### Separación de responsabilidades
- **`include/token.h`**: Contiene la definición de tipos de token (`TokenType`), la estructura de los tokens y la asociación con palabras reservadas.
- **`include/scanner.h`**: Interfaz del analizador léxico hacia el resto del programa.
- **`src/scanner.c`**: Implementa el autómata unificado; todo el lenguaje se procesa en este archivo para mantener la cohesión del AFD.
- **`src/token_name.c`**: Traduce constantes del enumerador a cadenas legibles para reportar resultados.
- **`src/main.c`**: Manejo de argumentos por línea de comandos, carga del archivo fuente y consumo del flujo de tokens.

---

## Tipos de Tokens

| Categoría | Ejemplos | Reconocido por |
|---|---|---|
| **Palabras Reservadas** | `int`, `void`, `if`, `else`, `while`, `return`, `read`, `write`, etc. | `S1` + búsqueda en tabla de keywords |
| **Símbolos Especiales** | `{`, `}`, `(`, `)`, `;`, `+`, `-`, `==`, `<=`, `&&`, etc. | `S3` (con lookahead de 1 carácter) |
| **INT_NUM** | `0`, `42`, `1024` | `S2` (acumulación de dígitos) |
| **ID** | `contador`, `suma`, `_temp` | `S1` (si no coincide con una palabra clave) |
| **Control / Error** | `TOK_EOF`, `TOK_ERROR` | Fin de archivo o carácter no reconocido |

---

## Diseño del AFD (Resumen)

- **`S0` (Estado Inicial):** Inspecciona el primer carácter (omitiendo espacios en blanco y gestionando saltos de línea) y transiciona hacia:
  - Letra o `_` $\rightarrow$ **`S1`**.
  - Dígito $\rightarrow$ **`S2`**.
  - Símbolos y operadores $\rightarrow$ **`S3`**.
  - `EOF` $\rightarrow$ Retorna token de fin de archivo.
  - Carácter no válido $\rightarrow$ Retorna token `TOK_ERROR`.
- **`S1` (Identificadores y Palabras Reservadas):** Acumula caracteres alfanuméricos y guiones bajos. Al encontrar un delimitador, compara el lexema con la tabla de palabras clave; si coincide asigna el token de la palabra reservada, de lo contrario clasifica como `TOK_ID`.
- **`S2` (Constantes Enteras):** Acumula dígitos numéricos decimales hasta encontrar un delimitador y genera `TOK_INT_NUM`.
- **`S3` (Operadores y Delimitadores):** Aplica la regla de *maximal munch* con lookahead de 1 carácter (`peek_next()`) para distinguir entre símbolos simples (como `<`, `=`, `&`) y compuestos (como `<=`, `<<`, `==`, `&&`).

---

## Estado Actual del Proyecto

### Avance realizado
- [x] Creación de la estructura base de directorios (`include/`, `src/`).
- [x] Definición del enum `TokenType` en `include/token.h` con:
  - Palabras reservadas (`int`, `main`, `void`, `break`, `do`, `else`, `if`, `while`, `return`, `read`, `write`).
  - Símbolos y operadores simples y dobles (`{`, `}`, `(`, `)`, `;`, `+`, `-`, `*`, `/`, `==`, `<=`, `&&`, etc.).
  - Identificadores, números enteros, `TOK_EOF` y `TOK_ERROR`.

### Tareas pendientes (TODOs)
- [ ] **Completar `include/token.h`**:
  - [ ] Corregir directiva guard (`#ifndef TOKEN_H`, `#define TOKEN_H`, `#endif`).
  - [ ] Definir `struct Token` (con campos: tipo, lexema, línea, columna).
  - [ ] Definir tabla y función/búsqueda de palabras reservadas (`keyword_table`).
- [ ] **Definir `include/scanner.h`**:
  - [ ] Declarar `init_scanner(FILE *fp)`.
  - [ ] Declarar `get_next_token(void)`.
- [ ] **Implementar `src/token_name.c`**:
  - [ ] Implementar función `token_type_name(TokenType type)` que retorne el nombre en texto para impresión.
- [ ] **Implementar `src/scanner.c`**:
  - [ ] Buffer de lectura y funciones auxiliares (`advance()`, `peek_next()`).
  - [ ] Salto de espacios en blanco y actualización de números de línea.
  - [ ] Soporte para ignorar comentarios de una línea (`//`) y multilínea (`/* ... */`).
  - [ ] Lógica de los estados `S0`, `S1`, `S2` y `S3`.
  - [ ] Manejo de errores léxicos (`TOK_ERROR`).
- [ ] **Implementar `src/main.c`**:
  - [ ] Apertura del archivo fuente recibido como argumento.
  - [ ] Ciclo de lectura e impresión de tokens hasta llegar a `TOK_EOF`.
- [ ] **Configuración y Pruebas**:
  - [ ] Crear `Makefile` para compilación automatizada (`make`, `make clean`).
  - [ ] Crear carpeta `tests/` con casos de prueba y verificar el funcionamiento.
- [ ] **Documentación / Entrega**:
  - [ ] Redactar el informe técnico respondiendo las 3 preguntas teóricas.
  - [ ] Revisar estilo y comentarios en el código fuente.

---

## Compilación y Uso (Previsto)

Una vez implementados los módulos restantes, el proyecto se podrá compilar con `Makefile` o directamente con `gcc`:

```bash
# Compilación directa con gcc
gcc -Wall -Iinclude -o scanner src/scanner.c src/token_name.c src/main.c

# Ejecución con un archivo de prueba
./scanner tests/ejemplo.c
```

---

## Requisitos del Informe Técnico

El informe debe responder puntualmente a las siguientes 3 preguntas:
1. **¿Qué tipo de autómata diseñaron, AFD o AFND?**
2. **¿Cómo se diseñó el autómata escogido?**
3. **¿Cómo se diseña la función de escaneo?**
