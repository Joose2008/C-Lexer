# Scanner de C simplificado — COMPG2 Tarea 1

Analizador léxico manual (sin Lex/Flex) para el subconjunto de C definido en el enunciado. Implementado en **C**, modelando el reconocimiento de tokens como un único **AFD** (autómata finito determinista) con un estado inicial que se ramifica según el primer carácter leído.

## Estructura del proyecto

```
C-Lexer/
├── Makefile             # Reglas de compilación, pruebas y limpieza
├── include/
│   ├── token.h          # Definición de TokenType, struct Token y tabla de keywords
│   └── scanner.h        # Interfaz del escáner (scaner_init, scaner_get_next_token)
├── src/
│   ├── scanner.c        # Implementación del AFD (estados S0–S6)
│   ├── token_name.c     # Conversión de TokenType a texto para visualización
│   └── main.c           # Lectura del archivo de entrada y bucle principal
└── tests/               # Casos de prueba (.c)
    ├── ejemplo1.c                   # Programa básico (int, if, return)
    ├── ejemplo2.c                   # Cobertura amplia: preprocesador, keywords, operadores, literales
    ├── ejemplo3.c                   # Identificadores, números y maximal munch
    ├── bordes.c                     # Casos borde: sin espacios, escapes, directivas
    ├── vacio.c                      # Archivo vacío (solo debe producir EOF)
    ├── error1_caracter_invalido.c   # Carácter inválido `@` (debe dar TOK_ERROR)
    ├── error2_string_sin_cerrar.c   # String sin cerrar (debe dar TOK_ERROR)
    └── error3_char_y_preproc.c      # Char sin cerrar y directiva inválida (debe dar TOK_ERROR)
    └── error4_escape_invalido.c     # Escape inexistente `\q` (debe dar TOK_ERROR)
```

### Separación de responsabilidades
- **`include/token.h`**: Contiene la definición de tipos de token (`TokenType`), la estructura de los tokens y la asociación con palabras reservadas.
- **`include/scanner.h`**: Interfaz del analizador léxico hacia el resto del programa. Expone `scaner_init()` (carga el fuente desde un string en memoria) y `scaner_get_next_token()` (devuelve un token por llamada, `TOK_EOF` al final).
- **`src/scanner.c`**: Implementa el autómata unificado; todo el lenguaje se procesa en este archivo para mantener la cohesión del AFD.
- **`src/token_name.c`**: Traduce constantes del enumerador a cadenas legibles para reportar resultados.
- **`src/main.c`**: Manejo de argumentos por línea de comandos, carga del archivo fuente y consumo del flujo de tokens.

---

## Tipos de Tokens

| Categoría | Ejemplos | Reconocido por |
|---|---|---|
| **Palabras Reservadas** | `int`, `void`, `if`, `else`, `while`, `return`, `for`, `struct`, `scanf`/`printf` (como `READ`/`WRITE`), `main`, etc. | `S1` + búsqueda en tabla de keywords |
| **Directivas de preprocesador** | `#include` → `PREPROC_INCLUDE`, `#define` → `PREPROC_DEFINE`, `#ifdef`, … | `S3` (desde `#`, una por directiva admitida) |
| **Símbolos y operadores** | `{`, `}`, `(`, `)`, `[`, `]`, `;`, `,`, `:`, `.`, `+`, `-`, `*`, `/`, `==`, `!=`, `<=`, `>=`, `&&`, `\|\|`, `<<`, `>>`, `->`, `&`, `\|`, `!`, `^`, `~` | `S6` (con lookahead de 1 carácter) |
| **Literales** | `"texto"`, `'a'`, `'\n'` (un escape inválido como `\q` da `TOK_ERROR`) | `S4` (strings), `S5` (chars), con validación de escapes |
| **INT_NUM** | `0`, `42`, `1024` | `S2` (acumulación de dígitos) |
| **ID** | `contador`, `suma`, `_temp` | `S1` (si no coincide con una palabra clave) |
| **Control / Error** | `TOK_EOF`, `TOK_ERROR` | Fin de archivo o carácter/literal no reconocido |

---

## Diseño del AFD (Resumen)

- **`S0` (Estado Inicial):** Inspecciona el primer carácter (omitiendo espacios en blanco) y deriva hacia:
  - Letra o `_` $\rightarrow$ **`S1`**.
  - Dígito $\rightarrow$ **`S2`**.
  - `#` $\rightarrow$ **`S3`** (directiva de preprocesador).
  - `"` $\rightarrow$ **`S4`** (literal de cadena).
  - `'` $\rightarrow$ **`S5`** (literal de carácter).
  - Cualquier otro símbolo $\rightarrow$ **`S6`**.
  - `EOF` $\rightarrow$ Retorna token de fin de archivo.
  - Carácter no válido $\rightarrow$ Retorna token `TOK_ERROR`.
- **`S1` (Identificadores y Palabras Reservadas):** Acumula caracteres alfanuméricos y guiones bajos. Al encontrar un delimitador, compara el lexema con la tabla de palabras clave; si coincide asigna el token de la palabra reservada, de lo contrario clasifica como `TOK_ID`.
- **`S2` (Constantes Enteras):** Acumula dígitos numéricos decimales hasta encontrar un delimitador y genera `TOK_INT_NUM`.
- **`S3` (Directivas de preprocesador):** Tras `#`, lee el identificador de la directiva y emite su token específico (`PREPROC_INCLUDE`, `PREPROC_DEFINE`, …); si no es una directiva admitida, genera `TOK_ERROR`.
- **`S4`/`S5` (Literales con escapes):** Consumen la secuencia de escape (`\` + carácter) como unidad y la validan contra el conjunto admitido (`n`, `0`, `r`, `t`, `\`, `v`, `f`, `a`, `"`, `'`); un escape inexistente o un literal sin cerrar antes del fin de línea/archivo genera `TOK_ERROR`.
- **`S6` (Operadores y Delimitadores):** Aplica la regla de *maximal munch* con lookahead de 1 carácter para distinguir entre símbolos simples (como `<`, `=`, `&`) y compuestos (como `<=`, `<<`, `==`, `&&`, `->`).

---

## Estado Actual del Proyecto

### Avance realizado
- [x] Creación de la estructura base de directorios (`include/`, `src/`, `tests/`).
- [x] Definición del enum `TokenType` en `include/token.h` con guardia corregida (`TOKEN_H`), `struct Token` (tipo + lexema de hasta 255 caracteres), tabla `KeywordEntry` y declaraciones de `getTokenType()` / `getTokenTypeName()`.
- [x] Interfaz del escáner en `include/scanner.h`: `scaner_init(const char *)` y `scaner_get_next_token()` (operan sobre el fuente cargado en memoria).
- [x] Implementación del AFD en `src/scanner.c`: buffer con `peek()`/`advance()`, estados `S0`–`S6`, tabla de 35 palabras reservadas, directivas de preprocesador con token específico por directiva, literales con validación de escapes y operadores con *maximal munch*.
- [x] Implementación de `getTokenTypeName()` en `src/token_name.c` para todos los tipos de token.
- [x] Implementación de `src/main.c`: apertura del archivo recibido como argumento y ciclo de impresión de tokens hasta `TOK_EOF`.
- [x] `Makefile` con objetivos `all`, `test` y `clean`.
- [x] Carpeta `tests/` con 9 casos de prueba verificados (ver tabla abajo).

### Resultado de los tests (verificado)

| Archivo | Contenido | Resultado esperado |
|---|---|---|
| `ejemplo1.c` | Programa básico | 41 tokens, sin errores |
| `ejemplo2.c` | Cobertura amplia | ~359 tokens, sin errores |
| `ejemplo3.c` | IDs, números, *maximal munch* | ~227 tokens, sin errores |
| `bordes.c` | Casos borde | Sin errores |
| `vacio.c` | Archivo vacío | Solo `TOK_EOF` |
| `error1_caracter_invalido.c` | `@` | `TOK_ERROR`, aborta |
| `error2_string_sin_cerrar.c` | String sin cerrar | `TOK_ERROR`, aborta |
| `error3_char_y_preproc.c` | Char sin cerrar / `#foo` | `TOK_ERROR`, aborta |
| `error4_escape_invalido.c` | Escape inexistente `\q` | `TOK_ERROR`, aborta |

### Tareas pendientes / limitaciones conocidas
- [ ] **Comentarios:** el escáner aún no ignora `//` ni `/* ... */`; los tokeniza como operadores (`/`, `*`) e identificadores.
- [ ] **Operador módulo:** no existe token para `%`; produce `TOK_ERROR`.
- [ ] **Línea/columna:** `struct Token` no lleva posición y el escáner no la rastrea.
- [ ] **Escapes como tokens:** los `TOK_ESC_*` definen los escapes válidos pero no se emiten como tokens independientes; los literales se devuelven completos (`STRING_LIT` / `CHAR_LIT`).
- [ ] **Directivas con espacio:** `# include` (espacio tras `#`) produce `TOK_ERROR`; solo funciona `#include` pegado.
- [ ] **`src/main.c`:** se detiene ante el primer `TOK_ERROR` (no continúa con el resto del archivo) y retorna código `1` incluso en ejecuciones exitosas.
- [ ] **Portabilidad del `Makefile`:** los objetivos usan sintaxis Unix (`./$(OUT)`, `rm -f`); en Windows requieren Git Bash/MSYS. Los objetivos `check`/`check-unit` referencian `tests/run_tests.py` y `tests/test_scanner.c`, que actualmente no existen en el repositorio.
- [ ] **Documentación / Entrega**:
  - [ ] Redactar el informe técnico respondiendo las 3 preguntas teóricas (`informe.pdf` aún no existe).

---

## Compilación y Uso

```bash
# Compilación directa con gcc
gcc -Wall -Wextra -Iinclude -o scanner src/main.c src/scanner.c src/token_name.c

# Con Makefile (Linux / Git Bash en Windows)
make          # compila el ejecutable `scanner`
make test     # ejecuta ejemplo1.c, ejemplo2.c y ejemplo3.c
make clean    # elimina el ejecutable

# Ejecución (en Windows PowerShell el binario es scanner.exe)
./scanner tests/ejemplo1.c        # Linux / Git Bash
.\scanner.exe tests\ejemplo1.c    # PowerShell
```

Salida ante éxito (tokens en `stdout`):
```
Token: INT        Lexema: int
Token: MAIN       Lexema: main
...
Token: EOF        Lexema:
```

Salida ante error léxico (mensaje en `stderr`, el programa se detiene):
```
Error: Token desconocido '@'
```

---

## Requisitos del Informe Técnico

El informe debe responder puntualmente a las siguientes 3 preguntas:
1. **¿Qué tipo de autómata diseñaron, AFD o AFND?**
2. **¿Cómo se diseñó el autómata escogido?**
3. **¿Cómo se diseña la función de escaneo?**
