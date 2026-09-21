#include <stdio.h>

int main(void) {
    int leituras[5] = {26, 28, 30, 32, 34};
    int soma = 0;
    int minimo = leituras[0];
    int maximo = leituras[0];
    int alvo = 30;
    int encontrado = -1;

    for (int i = 0; i < 5; i++) {
        printf("Sensor[%d] = %d C\n", i, leituras[i]);
        soma += leituras[i];
        if (leituras[i] < minimo) {
            minimo = leituras[i];
        }
        if (leituras[i] > maximo) {
            maximo = leituras[i];
        }
    }
    double media = (double)soma / 5;
    printf("Soma: %d; media: %.1f C\n", soma, media);
    printf("Minimo: %d; maximo: %d\n", minimo, maximo);

    /* A busca pode parar; a soma anterior precisava visitar tudo. */
    for (int i = 0; i < 5; i++) {
        if (leituras[i] == alvo) {
            encontrado = i;
            break;
        }
    }
    if (encontrado >= 0) {
        printf("Encontrado no indice %d\n", encontrado);
    } else {
        printf("Valor ausente\n");
    }
    return 0;
}
