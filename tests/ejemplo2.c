/* ejemplo2.c - Cobertura total: preprocesador, keywords, operadores, literales */
#include <stdio.h>
#define MAX 100

int main(void) {
    int x = 10;
    float f = 3;
    char c = 'a';
    char nl = '\n';
    const char *msg = "hola \"mundo\"\n";
    int arr[10];
    int i;

    for (i = 0; i < MAX; i = i + 1) {
        if ((x == 10 && f != 0) || (x <= 5 && x >= 2)) {
            x = x << 1;
            x = x >> 1;
        } else {
            x = x + 1 - 2 * 3 / 4;
        }
    }

    struct Punto { int x; int y; };
    struct Punto p;
    p.x = 1;
    struct Punto *ptr = &p;
    ptr->x = 2;

    int a = x & 3 | 4 ^ 5;
    int b = ~x;
    int not = !x;
    switch (x) {
        case 1: x = 2; break;
        default: x = 0; break;
    }
    while (x < 100) { x = x + 1; continue; }
    do { x = x - 1; } while (x > 0);
    enum Color { ROJO, VERDE };
    unsigned long n = 42;
    short s = 1;
    signed d = -1;
    double dd = 2;
    long lg = 5;
    static int st = 0;
    extern int ext;
    register int rg = 0;
    auto int au = 0;
    volatile int vl = 0;
    sizeof(int);
    typedef int entero;
    union U { int a; float b; };
    goto fin;
fin:
    printf("suma=%d\n", x);
    scanf("%d", &x);
    return 0;
}
