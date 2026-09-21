#include <assert.h>
#include <stdio.h>
#include "regras.h"

#ifdef NDEBUG
#error "Esta suite requer assert ativo: compile sem -DNDEBUG."
#endif

int main(void)
{
    const int misto[] = {28, 30, 32};
    const int nenhum[] = {25, 28};
    const int todos[] = {30, 31};
    const int igual[] = {30};
    const int negativos[] = {-2, 0, 2};
    assert(contar_no_limite(misto, 3, 30) == 2);
    assert(contar_no_limite(nenhum, 2, 30) == 0);
    assert(contar_no_limite(todos, 2, 30) == 2);
    assert(contar_no_limite(igual, 1, 30) == 1);
    assert(contar_no_limite(NULL, 0, 30) == 0);
    assert(contar_no_limite(negativos, 3, 0) == 2);
    puts("6 testes de contagem passaram");

    assert(!temperatura_valida(-41));
    assert(temperatura_valida(-40));
    assert(temperatura_valida(0));
    assert(temperatura_valida(30));
    assert(temperatura_valida(125));
    assert(!temperatura_valida(126));
    puts("6 testes de validacao passaram");
    return 0;
}
