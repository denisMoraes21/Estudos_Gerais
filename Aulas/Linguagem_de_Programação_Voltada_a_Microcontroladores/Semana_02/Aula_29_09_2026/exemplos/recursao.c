#include <stdio.h>

/* Contrato das duas funcoes: n <= 20. */
static unsigned soma_recursiva(unsigned n)
{
    if (n == 0U) return 0U;
    return n + soma_recursiva(n - 1U);
}

static unsigned soma_iterativa(unsigned n)
{
    unsigned total = 0U;
    for (unsigned i = 1U; i <= n; ++i) total += i;
    return total;
}

int main(void)
{
    const unsigned entradas[] = {0U, 4U, 5U, 20U};
    const size_t n = sizeof entradas / sizeof entradas[0];
    for (size_t i = 0; i < n; ++i) {
        if (entradas[i] > 20U) return 1;
        printf("n=%u: recursiva=%u, iterativa=%u\n", entradas[i],
               soma_recursiva(entradas[i]), soma_iterativa(entradas[i]));
    }
    return 0;
}
