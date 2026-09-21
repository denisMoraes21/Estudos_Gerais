#include "alarme.h"

bool alarme_ativo(int temperatura_c, int limite_c)
{
    return temperatura_c >= limite_c;
}
