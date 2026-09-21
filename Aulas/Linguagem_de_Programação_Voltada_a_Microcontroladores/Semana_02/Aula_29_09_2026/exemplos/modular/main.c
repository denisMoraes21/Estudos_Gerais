#include <stdio.h>
#include "alarme.h"

int main(void)
{
    const int leituras[] = {28, 30, 32};
    const size_t n = sizeof leituras / sizeof leituras[0];
    for (size_t i = 0; i < n; ++i) {
        bool ativo = alarme_ativo(leituras[i], 30);
        printf("%d C: %s\n", leituras[i], ativo ? "ALARME" : "normal");
    }
    return 0;
}
