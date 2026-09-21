#include "socket.h"

#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip.h"

#include "logger.h"

#include <stdio.h>
#include <string.h>

#define SERVER_IP       "192.168.1.10"
#define SERVER_PORT     5000
#define RX_BUFFER_SIZE  128

extern struct netif gnetif;

static osThreadId_t tcp_client_task_handle;

static const osThreadAttr_t tcp_client_task_attributes = {
    .name = "tcpClient",
    .stack_size = 4096,
    .priority = (osPriority_t)osPriorityNormal,
};

static void tcp_client_task(void *argument)
{
    int socket_fd;
    int result;
    int received;

    struct sockaddr_in server_address;

    char tx_buffer[128];
    char rx_buffer[RX_BUFFER_SIZE];

    (void)argument;

    /*
     * Aguarda a interface Ethernet e o link físico ficarem ativos.
     */
    while (!netif_is_up(&gnetif) ||
           !netif_is_link_up(&gnetif))
    {
        LOG_INFO("Waiting for Ethernet interface...");
        osDelay(1000);
    }

    LOG_INFO("Ethernet ready");

    for (;;)
    {
        socket_fd = lwip_socket(
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
        );

        if (socket_fd < 0)
        {
            LOG_ERROR("Failed to create socket");
            osDelay(2000);
            continue;
        }

        memset(&server_address, 0, sizeof(server_address));

        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(SERVER_PORT);

        result = inet_aton(
            SERVER_IP,
            &server_address.sin_addr
        );

        if (result == 0)
        {
            LOG_ERROR("Invalid server IP");
            lwip_close(socket_fd);
            osDelay(2000);
            continue;
        }

        LOG_INFO(
            "Connecting to %s:%d...",
            SERVER_IP,
            SERVER_PORT
        );

        result = lwip_connect(
            socket_fd,
            (struct sockaddr *)&server_address,
            sizeof(server_address)
        );

        if (result < 0)
        {
            LOG_ERROR(
                "Connection failed. errno = %d",
                errno
            );

            lwip_close(socket_fd);
            osDelay(2000);
            continue;
        }

        LOG_INFO("Connected to TCP server");

        uint32_t counter = 0;

        while (1)
        {
            int message_length = snprintf(
                tx_buffer,
                sizeof(tx_buffer),
                "Mensagem STM32: %lu\n",
                (unsigned long)counter++
            );

            result = lwip_send(
                socket_fd,
                tx_buffer,
                message_length,
                0
            );

            if (result < 0)
            {
                LOG_ERROR(
                    "Send failed. errno = %d",
                    errno
                );
                break;
            }

            LOG_INFO("Sent %d bytes", result);

            received = lwip_recv(
                socket_fd,
                rx_buffer,
                sizeof(rx_buffer) - 1,
                0
            );

            if (received == 0)
            {
                LOG_WARN("Server closed the connection");
                break;
            }

            if (received < 0)
            {
                LOG_ERROR(
                    "Receive failed. errno = %d",
                    errno
                );
                break;
            }

            rx_buffer[received] = '\0';

            LOG_INFO(
                "Server response: %s",
                rx_buffer
            );

            osDelay(1000);
        }

        LOG_INFO("Closing socket");

        lwip_shutdown(socket_fd, SHUT_RDWR);
        lwip_close(socket_fd);

        osDelay(2000);
    }
}

osStatus_t socket_init(void)
{
    if (tcp_client_task_handle != NULL)
    {
        return osErrorResource;
    }

    tcp_client_task_handle = osThreadNew(
        tcp_client_task,
        NULL,
        &tcp_client_task_attributes
    );

    if (tcp_client_task_handle == NULL)
    {
        LOG_ERROR("Failed to create TCP client thread");
        return osError;
    }

    LOG_INFO("TCP client thread created");
    return osOK;
}
