#include <stdbool.h>
#include <stdio.h>

int main(void) {
    int pecas = 120;
    double tensao_v = 3.3;
    char setor = 'C';
    bool pronto = true;
    printf("Pecas: %d\n", pecas);
    printf("Tensao: %.1f V\n", tensao_v);
    printf("Setor: %c\n", setor);
    printf("Pronto: %d\n", pronto);
    return 0;
}
