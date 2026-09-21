#include <stdio.h>
#include "regras.h"

int main(void)
{
    const int leituras[] = {28, 30, 32};
    const size_t n = sizeof leituras / sizeof leituras[0];
    for (size_t i = 0; i < n; ++i) {
        if (!temperatura_valida(leituras[i])) {
            fprintf(stderr, "Leitura invalida no indice %zu\n", i);
            return 1;
        }
    }
    printf("Ocorrencias: %zu\n", contar_no_limite(leituras, n, 30));
    return 0;
}
