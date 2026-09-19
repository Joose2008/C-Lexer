/* error4: escape invalido dentro de un string -> TOK_ERROR y main aborta */
int main(void) {
    printf("bad\qescape");
    return 0;
}
