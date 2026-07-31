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
        LOG_INFO("Aguardando interface Ethernet...");
        osDelay(1000);
    }

    LOG_INFO("Ethernet pronta");

    for (;;)
    {
        socket_fd = lwip_socket(
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
        );

        if (socket_fd < 0)
        {
            LOG_ERROR("Erro ao criar socket");
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
            LOG_ERROR("IP do servidor invalido");
            lwip_close(socket_fd);
            osDelay(2000);
            continue;
        }

        LOG_INFO(
            "Conectando em %s:%d...",
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
                "Falha ao conectar. errno = %d",
                errno
            );

            lwip_close(socket_fd);
            osDelay(2000);
            continue;
        }

        LOG_INFO("Conectado ao servidor TCP");

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
                    "Erro no envio. errno = %d",
                    errno
                );
                break;
            }

            LOG_INFO("Enviados %d bytes", result);

            received = lwip_recv(
                socket_fd,
                rx_buffer,
                sizeof(rx_buffer) - 1,
                0
            );

            if (received == 0)
            {
                LOG_WARN("Servidor encerrou a conexão");
                break;
            }

            if (received < 0)
            {
                LOG_ERROR(
                    "Erro na recepcao. errno = %d",
                    errno
                );
                break;
            }

            rx_buffer[received] = '\0';

            LOG_INFO(
                "Servidor respondeu: %s",
                rx_buffer
            );

            osDelay(1000);
        }

        LOG_INFO("Fechando socket");

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
        LOG_ERROR("Falha ao criar a thread do cliente TCP");
        return osError;
    }

    LOG_INFO("Thread do cliente TCP criada");
    return osOK;
}
