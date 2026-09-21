#include "regras.h"

size_t contar_no_limite(const int valores[], size_t quantidade, int limite)
{
    size_t total = 0;
    for (size_t i = 0; i < quantidade; ++i) {
        if (valores[i] >= limite) ++total;
    }
    return total;
}

bool temperatura_valida(int temperatura_c)
{
    return temperatura_c >= -40 && temperatura_c <= 125;
}
