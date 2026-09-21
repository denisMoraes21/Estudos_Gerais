#include <stdio.h>

int main(void) {
    int opcao;

    do {
        printf("0 = sair; 1 = consultar; 2 = executar: \n");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida\n");
            return 1;
        }
        switch (opcao) {
            case 0:
                printf("Encerrando\n");
                break;
            case 1:
                printf("Sensor: 28 C\n");
                break;
            case 2:
                for (int ciclo = 1; ciclo <= 3; ciclo++) {
                    printf("Ciclo %d\n", ciclo);
                }
                break;
            default:
                printf("Opcao invalida\n");
                break;
        }
    } while (opcao != 0);
    return 0;
}
