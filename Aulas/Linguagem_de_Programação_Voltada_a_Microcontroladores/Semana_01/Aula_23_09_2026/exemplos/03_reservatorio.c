#include <stdbool.h>
#include <stdio.h>

int main(void) {
    int nivel_l = 180;
    double temperatura_c = 24.5;
    char setor = 'B';
    bool alarme = false;
    int nivel_anterior_l = nivel_l;
    nivel_l = 210;
    printf("Nivel atual: %d L\n", nivel_l);
    printf("Nivel anterior: %d L\n", nivel_anterior_l);
    printf("Temperatura: %.1f C\n", temperatura_c);
    printf("Setor: %c\n", setor);
    printf("Alarme: %d\n", alarme);
    return 0;
}
