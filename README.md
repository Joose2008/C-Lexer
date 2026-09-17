# Scanner de C simplificado — COMPG2 Tarea 1

Analizador léxico manual (sin Lex/Flex) para el subconjunto de C definido en el
enunciado. Implementado en **C**, modelando el reconocimiento de tokens como
un único **AFD** (autómata finito determinista) con un estado inicial que se
ramifica según el primer carácter leído.

## Estructura del proyecto

```
scanner_c/
├── Makefile
├── include/
│   └── token.h          # enum TokenType, struct Token, tabla de keywords
├── src/
│   ├── scanner.c         # el AFD: get_next_token() y los sub-estados S1/S2/S3
│   ├── token_names.c     # token_type_name(): enum -> string para imprimir
│   └── main.c            # lee el archivo de entrada y llama al scanner en loop
├── tests/
│   └── ejemplo1.c ... ejemplo5.c   # casos de prueba proporcionados
└── informe.pdf            # informe técnico (las 3 preguntas pedidas)
```

**Por qué así:** un solo `.c` para el AFD porque, conceptualmente, todo el
lenguaje léxico es un único autómata (no uno por categoría de token). Separar
`token_names.c` y `main.c` es solo separación de responsabilidades (imprimir
vs. reconocer vs. leer archivo), no forma parte del diseño del autómata en sí.

> Nota: mientras se arma la estructura final, el código vive junto en
> `scanner_c/` (sin subcarpetas todavía). Este README ya refleja la
> organización a la que vamos a migrar.

## Los 4 tipos de token

| Categoría | Ejemplos | Reconocido por |
|---|---|---|
| Keywords | `int`, `main`, `return`, `if`... | `S1` + búsqueda en `keyword_table` |
| Símbolos especiales | `{`, `(`, `==`, `<=`, `&&`... | `S3` (con lookahead de 1 carácter) |
| INT_NUM | `0`, `123` | `S2` |
| ID | `a`, `contador`, `_tmp` | `S1` (si no está en `keyword_table`) |

## Diseño del AFD (resumen)

- `S0` (inicial): según el primer carácter, salta a `S1`, `S2` o `S3`.
- `S1`: acumula letras/dígitos/`_`. Al terminar, compara el lexema completo
  contra `keyword_table` (búsqueda exacta) → si coincide, es la keyword
  correspondiente; si no, es `ID`.
- `S2`: acumula dígitos → `INT_NUM`.
- `S3`: usa *maximal munch* con `peek_next()` para decidir si un símbolo es
  simple (`<`) o compuesto (`<=`, `<<`).

## Estado actual / pendientes (TODOs)

- [x] Esqueleto de `token.h` (enum + struct + tabla de keywords)
- [x] Esqueleto de `scanner.c` (S0, S1, S2, S3, `main` provisional)
- [x] `token_names.c` completo
- [x] Compila y corre correctamente con el ejemplo del enunciado
- [ ] **Completar `keyword_table`** en `token.h`: faltan `void`, `break`,
      `do`, `else`, `if`, `while`, `return`, `scanf`, `printf`
- [ ] **Completar `scan_symbol()`** en `scanner.c`:
  - [ ] Símbolos simples que faltan: `[`, `]`, `-`, `*`, `/`, `,`
  - [ ] Lookahead para símbolos compuestos: `=`/`==`, `<`/`<=`/`<<`,
        `>`/`>=`/`>>`, `&`/`&&`, `|`/`||`, `!`/`!=`
- [ ] Decidir si se deben saltar comentarios `//` y `/* */` en
      `skip_whitespace()`
- [ ] Reorganizar en carpetas `include/`, `src/`, `tests/` + `Makefile`
- [ ] Probar contra los 5 casos de prueba proporcionados por el profesor
- [ ] Manejo de errores: carácter no reconocido → token `ERROR` (ya hay un
      esqueleto en `scan_symbol` con `default: tok.type = TOK_ERROR;`)
- [ ] Escribir el informe técnico (3 preguntas: AFD o AFND, cómo se diseñó,
      cómo se diseña la función de escaneo)
- [ ] Revisar/limpiar comentarios del código antes de la entrega (10% de la
      nota es estilo y comentarios)

## Cómo compilar y probar (por ahora, sin Makefile)

```bash
gcc -Wall -o scanner scanner.c token_names.c
./scanner ejemplo.c
```

## Preguntas que debe responder el informe técnico

1. ¿Qué tipo de autómata diseñaron, AFD o AFND?
2. ¿Cómo se diseñó el autómata escogido?
3. ¿Cómo se diseña la función de escaneo?

(Respuestas cortas, claras, formato limpio — no hace falta que sea extenso.)
