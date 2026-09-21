#include <stdio.h>

int main(void) {
    int temperatura[2][3] = {
        {24, 25, 26},
        {28, 29, 30}
    };

    for (int linha = 0; linha < 2; linha++) {
        int soma_linha = 0;
        for (int coluna = 0; coluna < 3; coluna++) {
            printf("%d ", temperatura[linha][coluna]);
            soma_linha += temperatura[linha][coluna];
        }
        printf("\n");
        double media = (double)soma_linha / 3;
        printf("Maquina %d: %.1f C\n", linha, media);
    }
    return 0;
}
