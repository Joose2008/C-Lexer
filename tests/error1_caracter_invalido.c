/* error1: '@' no pertenece al lenguaje -> el escaner emite TOK_ERROR y main aborta */
int main(void) {
    int a = 5 @ 3;
    return 0;
}
