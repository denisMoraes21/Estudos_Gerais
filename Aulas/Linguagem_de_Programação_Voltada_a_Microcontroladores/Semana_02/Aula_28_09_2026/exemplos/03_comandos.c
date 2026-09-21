#include <stdio.h>
#include <string.h>

int main(void) {
    char comando[16];
    int saida = 0;

    printf("Comando (LIGAR ou DESLIGAR): \n");
    if (fgets(comando, sizeof comando, stdin) == NULL) {
        printf("Nenhuma linha recebida\n");
        return 1;
    }

    size_t tamanho = strlen(comando);
    if (tamanho > 0 && comando[tamanho - 1] == '\n') {
        comando[tamanho - 1] = '\0';
    } else {
        /* Aceita 15 caracteres se so faltou a quebra ou chegou EOF. */
        int extra = getchar();
        if (extra != '\n' && extra != EOF) {
            while ((extra = getchar()) != '\n' && extra != EOF) {
                /* Descartar toda a parte que nao coube. */
            }
            printf("Comando muito longo\n");
            return 1;
        }
    }

    if (strcmp(comando, "LIGAR") == 0) {
        saida = 1;
    } else if (strcmp(comando, "DESLIGAR") == 0) {
        saida = 0;
    } else {
        printf("Comando desconhecido\n");
        return 1;
    }
    printf("Saida simulada: %d\n", saida);
    return 0;
}
