CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/scanner.c src/token_name.c
OUT = scanner

# Compilación del ejecutable del analizador
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

# Muestra los tokens de los tres ejemplos principales
test: $(OUT)
	@echo "=== Test ejemplo1.c ==="
	./$(OUT) tests/ejemplo1.c
	@echo ""
	@echo "=== Test ejemplo2.c ==="
	./$(OUT) tests/ejemplo2.c
	@echo ""
	@echo "=== Test ejemplo3.c ==="
	./$(OUT) tests/ejemplo3.c

# Suite automatizada (requiere tests/run_tests.py)
check:
	python tests/run_tests.py

# Solo la suite unitaria en C (requiere tests/test_scanner.c)
check-unit:
	$(CC) $(CFLAGS) -o tests/test_scanner src/scanner.c src/token_name.c tests/test_scanner.c
	./tests/test_scanner

# Elimina los binarios generados
clean:
	rm -f $(OUT) tests/test_scanner tests/test_scanner.exe

# NOTA Windows: los objetivos usan sintaxis Unix (./, rm); en PowerShell
# compilar con gcc directamente y ejecutar scanner.exe (ver README).
.PHONY: all test check check-unit clean
