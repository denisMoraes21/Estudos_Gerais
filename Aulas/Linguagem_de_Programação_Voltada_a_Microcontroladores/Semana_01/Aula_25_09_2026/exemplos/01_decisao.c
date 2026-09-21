#include <stdio.h>

int main(void) {
    int leitura = 28;
    int ajuste = 2;
    int temperatura = leitura + ajuste;
    int ventilador;

    if (temperatura >= 30) {
        ventilador = 1;
    } else {
        ventilador = 0;
    }
    printf("Temperatura: %d C\n", temperatura);
    printf("Ventilador: %d\n", ventilador);
    return 0;
}
