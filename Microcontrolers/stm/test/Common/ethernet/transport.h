#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct NetworkContext {
    int socket;
};

bool f_transport_connect(struct NetworkContext *p_context, const char *p_ip,
                         uint16_t v_port);

int32_t f_transport_send(struct NetworkContext *p_context, const void *p_data,
                         size_t v_size);
int32_t f_transport_receive(struct NetworkContext *p_context, void *p_buffer,
                            size_t v_capacity);

bool f_transport_close(struct NetworkContext *p_context);

#ifdef __cplusplus
}
#endif

#endif /* TRANSPORT_H */
