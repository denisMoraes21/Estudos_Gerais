#ifndef REGRAS_H
#define REGRAS_H

#include <stdbool.h>
#include <stddef.h>

/* Quantidade positiva exige esse numero de elementos legiveis.
 * Quantidade zero aceita NULL e retorna zero. */
size_t contar_no_limite(const int valores[], size_t quantidade, int limite);

/* Faixa didatica inclusiva: -40 a 125 graus Celsius. */
bool temperatura_valida(int temperatura_c);

#endif
