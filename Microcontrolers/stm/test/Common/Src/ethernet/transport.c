#include "transport.h"

#include "lwip/inet.h"
#include "lwip/sockets.h"

#include <errno.h>
#include <limits.h>

bool f_transport_connect(struct NetworkContext *p_context, const char *p_ip,
                         uint16_t v_port) {
    if (p_context == NULL || p_ip == NULL || v_port == 0) {
        errno = EINVAL;
        return false;
    }
    if (p_context->socket != -1) {
        errno = EISCONN;
        return false;
    }

    struct sockaddr_in s_destination = {0};
    s_destination.sin_family = AF_INET;
    s_destination.sin_port = lwip_htons(v_port);
    if (inet_aton(p_ip, &s_destination.sin_addr) == 0) {
        errno = EINVAL;
        return false;
    }

    int v_socket = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (v_socket < 0) {
        return false;
    }
    if (lwip_connect(v_socket, (struct sockaddr *)&s_destination,
                     sizeof(s_destination)) < 0 ||
        lwip_fcntl(v_socket, F_SETFL, O_NONBLOCK) < 0) {
        const int v_error = errno;
        lwip_close(v_socket);
        errno = v_error;
        return false;
    }

    p_context->socket = v_socket;
    return true;
}

static int32_t f_transport_result(int v_result) {
    if (v_result < 0 &&
        (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR)) {
        return 0;
    }
    return v_result;
}

int32_t f_transport_send(struct NetworkContext *p_context, const void *p_data,
                         size_t v_size) {
    if (p_context == NULL || p_context->socket < 0 || p_data == NULL) {
        errno = EINVAL;
        return -1;
    }
    const size_t v_length = v_size > INT_MAX ? INT_MAX : v_size;
    return f_transport_result(
        lwip_send(p_context->socket, p_data, v_length, 0));
}

int32_t f_transport_receive(struct NetworkContext *p_context, void *p_buffer,
                            size_t v_capacity) {
    if (p_context == NULL || p_context->socket < 0 || p_buffer == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (v_capacity == 0) {
        return 0;
    }
    const size_t v_length = v_capacity > INT_MAX ? INT_MAX : v_capacity;
    const int v_received = lwip_recv(p_context->socket, p_buffer, v_length, 0);
    if (v_received == 0) {
        errno = ENOTCONN;
        return -1;
    }
    return f_transport_result(v_received);
}

bool f_transport_close(struct NetworkContext *p_context) {
    if (p_context == NULL) {
        errno = EINVAL;
        return false;
    }
    if (p_context->socket < 0) {
        return true;
    }
    if (lwip_close(p_context->socket) < 0) {
        return false;
    }
    p_context->socket = -1;
    return true;
}
