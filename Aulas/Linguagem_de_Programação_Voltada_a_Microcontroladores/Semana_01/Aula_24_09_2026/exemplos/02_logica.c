#include <stdbool.h>
#include <stdio.h>

int main(void) {
    int quantidade = 0;
    int total = 100;
    bool media_alta = (quantidade != 0) && (total / quantidade > 20);
    double temperatura_c = 25.0;
    bool na_faixa = (temperatura_c >= 20.0) && (temperatura_c <= 30.0);
    int a = 2;
    int b = 1;
    printf("Media alta: %d\n", media_alta);
    printf("Temperatura na faixa: %d\n", na_faixa);
    printf("E logico: %d\n", a && b);
    printf("E bit a bit: %d\n", a & b);
    return 0;
}
