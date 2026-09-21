#include <stdbool.h>
#include <stdio.h>

int main(void) {
    int nivel_l = 180;
    double temperatura_c = 28.0;
    bool comando = true;
    bool falha = false;
    bool nivel_adequado = nivel_l >= 100;
    bool na_faixa = (temperatura_c >= 20.0) && (temperatura_c <= 30.0);
    bool habilitado = comando && nivel_adequado && na_faixa && !falha;
    printf("Nivel adequado: %d\n", nivel_adequado);
    printf("Temperatura na faixa: %d\n", na_faixa);
    printf("Habilitado: %d\n", habilitado);

    /* Recalcular: atribuicoes nao criam ligacoes automaticas. */
    nivel_l = 99;
    nivel_adequado = nivel_l >= 100;
    habilitado = comando && nivel_adequado && na_faixa && !falha;
    printf("Com 99 L: nivel=%d, faixa=%d, habilitado=%d\n",
           nivel_adequado, na_faixa, habilitado);
    return 0;
}
