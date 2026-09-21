#include <stdio.h>

int main(void) {
    int leituras[6] = {18, 22, 22, 27, 31, 24};
    int soma = 0;
    int minimo = leituras[0];
    int maximo = leituras[0];
    int acima_limite = 0;

    for (int i = 0; i < 6; i++) {
        printf("Leitura[%d] = %d\n", i, leituras[i]);
        soma += leituras[i];
        if (leituras[i] < minimo) {
            minimo = leituras[i];
        }
        if (leituras[i] > maximo) {
            maximo = leituras[i];
        }
        if (leituras[i] >= 25) {
            acima_limite++;
        }
    }
    printf("Soma: %d; media: %.1f\n", soma, (double)soma / 6);
    printf("Minimo: %d; maximo: %d\n", minimo, maximo);
    printf("Leituras >= 25: %d\n", acima_limite);

    int alvos[2] = {22, 99};
    for (int busca = 0; busca < 2; busca++) {
        int encontrado = -1;
        for (int i = 0; i < 6; i++) {
            if (leituras[i] == alvos[busca]) {
                encontrado = i;
                break;
            }
        }
        if (encontrado >= 0) {
            printf("Alvo %d: indice %d\n", alvos[busca], encontrado);
        } else {
            printf("Alvo %d: ausente\n", alvos[busca]);
        }
    }
    return 0;
}
