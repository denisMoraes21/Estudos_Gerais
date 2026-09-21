#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/* Contrato: quantidade > 0; valores aponta para quantidade elementos. */
static int maior_leitura(const int valores[], size_t quantidade)
{
    int maior = valores[0];
    for (size_t i = 1; i < quantidade; ++i) {
        if (valores[i] > maior) maior = valores[i];
    }
    return maior;
}

/* Contrato: minimo <= maximo. */
static bool dentro_faixa(int valor, int minimo, int maximo)
{
    return valor >= minimo && valor <= maximo;
}

int main(void)
{
    const int leituras[] = {28, 31, 29, 30};
    const size_t n = sizeof leituras / sizeof leituras[0];
    printf("Maior leitura: %d C\n", maior_leitura(leituras, n));
    printf("30 em [28, 30]: %s\n", dentro_faixa(30, 28, 30) ? "sim" : "nao");
    return 0;
}
