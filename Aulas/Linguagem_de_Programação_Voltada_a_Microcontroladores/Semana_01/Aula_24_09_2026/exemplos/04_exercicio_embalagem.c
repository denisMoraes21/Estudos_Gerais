#include <stdio.h>

int main(void) {
    int pecas = 29;
    int capacidade = 6;
    int caixas_completas = pecas / capacidade;
    int pecas_restantes = pecas % capacidade;
    printf("Caixas completas: %d\n", caixas_completas);
    printf("Pecas restantes: %d\n", pecas_restantes);
    return 0;
}
