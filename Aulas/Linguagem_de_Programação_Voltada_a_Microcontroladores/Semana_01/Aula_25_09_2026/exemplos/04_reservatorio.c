#include <stdio.h>

int main(void) {
    int nivel;

    do {
        printf("Nivel de 0 a 100 (-1 encerra): \n");
        if (scanf("%d", &nivel) != 1) {
            printf("Entrada invalida\n");
            return 1;
        }
        if (nivel == -1) {
            break;
        }
        if (nivel < 0 || nivel > 100) {
            printf("Nivel invalido\n");
        } else if (nivel < 30) {
            printf("Baixo\n");
        } else if (nivel < 80) {
            printf("Adequado\n");
        } else {
            printf("Alto\n");
        }
    } while (nivel != -1);
    printf("Encerrado\n");
    return 0;
}
