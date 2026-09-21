#ifndef ALARME_H
#define ALARME_H

#include <stdbool.h>

/* Temperatura e limite em graus Celsius; igualdade ativa alarme. */
bool alarme_ativo(int temperatura_c, int limite_c);

#endif
