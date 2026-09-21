#include <stddef.h>
#include <stdio.h>

/* Os exemplos usam 10; incrementar exige valor menor que INT_MAX. */
static void tentar_incrementar(int contador)
{
    contador++;
    printf("Local: %d\n", contador);
}

static void incrementar_no_local(int *contador)
{
    if (contador != NULL) (*contador)++;
}

int main(void)
{
    int contador = 10;
    tentar_incrementar(contador);
    printf("Original: %d\n", contador);
    incrementar_no_local(&contador);
    printf("Depois do ponteiro: %d\n", contador);
    return 0;
}
