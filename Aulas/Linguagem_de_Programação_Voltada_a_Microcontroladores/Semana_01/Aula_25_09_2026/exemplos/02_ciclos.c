#include <stdio.h>

int main(void) {
    int temperatura = 26;
    int acionamentos = 0;

    for (int ciclo = 1; ciclo <= 5; ciclo++) {
        int ventilador = 0;
        if (temperatura >= 30) {
            ventilador = 1;
            acionamentos++;
        }
        printf("%d: %d C, ventilador=%d\n",
               ciclo, temperatura, ventilador);
        temperatura += 2;
    }
    printf("Ciclos ligado: %d\n", acionamentos);
    return 0;
}
