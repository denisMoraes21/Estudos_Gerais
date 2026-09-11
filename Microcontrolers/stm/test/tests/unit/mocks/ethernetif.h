#include "mock_platform.h"
typedef int err_t;
#define ERR_IF -12
err_t ethernetif_set_mac(struct netif *netif, const uint8_t mac[6]);
