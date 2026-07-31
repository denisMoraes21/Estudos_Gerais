#ifndef SOCKET_H
#define SOCKET_H

#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Cria a thread do cliente TCP.
 *
 * Deve ser chamada depois de MX_LWIP_Init() e com o kernel FreeRTOS em
 * execucao.
 *
 * @return osOK em caso de sucesso; osErrorResource se a thread ja existir;
 *         osError caso nao seja possivel cria-la.
 */
osStatus_t socket_init(void);

#ifdef __cplusplus
}
#endif

#endif /* SOCKET_H */
