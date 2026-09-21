#include <stdio.h>

int main(void) {
    int pecas = 17;
    int capacidade = 5;
    int caixas_completas = pecas / capacidade;
    int pecas_restantes = pecas % capacidade;
    double divisao_inteira = pecas / capacidade;
    double divisao_real = (double)pecas / capacidade;
    printf("Caixas completas: %d\n", caixas_completas);
    printf("Pecas restantes: %d\n", pecas_restantes);
    printf("Divisao inteira em double: %.1f\n", divisao_inteira);
    printf("Divisao real: %.1f\n", divisao_real);
    printf("Sem parenteses: %d\n", 2 + 3 * 4);
    printf("Com parenteses: %d\n", (2 + 3) * 4);
    return 0;
}
