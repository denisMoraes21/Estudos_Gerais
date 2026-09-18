#include "data.h"
#include "mock_platform.h"
#include "network.h"
#include "physical.h"
#include "transport.h"
#include <limits.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
struct netif gnetif;
ETH_HandleTypeDef heth;
lan8742_Object_t LAN8742;
int lock_depth;
static uint32_t tick, ready_at = UINT32_MAX;
static int delay_error, phy_fail, phy_invalid, phy_state, phy_reads, phy_zero;
static int dhcp_calls, dhcp_error, callback_error;
static int mac_update_error;
int ethernetif_set_mac(struct netif *netif, const uint8_t mac[6]) {
    if (mac_update_error) {
        return mac_update_error;
    }
    memcpy(netif->hwaddr, mac, 6);
    return ERR_OK;
}
static ip_addr_t dns;
static int socket_result = 7, connect_result, fcntl_result, close_result,
           close_calls;
static int io_result = 3, io_error, sendto_error, option_error, recv_calls,
           packet_mode;
static size_t io_size;
static unsigned char request[128];
static size_t request_size;
static struct sockaddr_in destination;
static long previous_timeout = LONG_MAX;
static jmp_buf monitor_exit;
static int monitor_iterations;
uint32_t HAL_GetTick(void) { return tick; }
uint32_t HAL_GetUIDw0(void) { return 0x1234abcd; }
int osDelay(uint32_t ms) {
    tick += ms;
    if (ready_at != UINT32_MAX && tick >= ready_at) {
        gnetif.ip.addr = 1;
        gnetif.up = gnetif.link = true;
    }
    if (monitor_iterations && --monitor_iterations == 0)
        longjmp(monitor_exit, 1);
    return delay_error;
}
HAL_StatusTypeDef HAL_ETH_ReadPHYRegister(ETH_HandleTypeDef *h, uint32_t a,
                                          uint32_t r, uint32_t *v) {
    (void)h;
    (void)a;
    (void)r;
    ++phy_reads;
    *v = phy_zero == phy_reads ? 0 :
         (phy_invalid == phy_reads ? 0xffff : 0x1234);
    return phy_fail == phy_reads ? -1 : HAL_OK;
}
int32_t LAN8742_GetLinkState(lan8742_Object_t *p) {
    (void)p;
    return phy_state;
}
void dhcp_stop(struct netif *n) { (void)n; }
int dhcp_start(struct netif *n) {
    (void)n;
    ++dhcp_calls;
    return dhcp_error;
}
void dns_setserver(int i, const ip_addr_t *a) {
    assert(i == 0);
    dns = *a;
}
int tcpip_callback_wait(void (*fn)(void *), void *arg) {
    if (callback_error)
        return -1;
    fn(arg);
    return 0;
}
int lwip_socket(int a, int b, int c) {
    assert(a == AF_INET);
    assert((b == SOCK_STREAM && c == IPPROTO_TCP) ||
           (b == SOCK_RAW && c == IPPROTO_ICMP));
    errno = EMFILE;
    return socket_result;
}
int lwip_connect(int s, const struct sockaddr *a, socklen_t l) {
    assert(s == 7 && l == sizeof(destination));
    destination = *(const struct sockaddr_in *)a;
    errno = ECONNREFUSED;
    return connect_result;
}
int lwip_fcntl(int s, int cmd, int value) {
    assert(s == 7 && cmd == F_SETFL && value == O_NONBLOCK);
    errno = EIO;
    return fcntl_result;
}
int lwip_close(int s) {
    assert(s == 7);
    ++close_calls;
    errno = EBADF;
    return close_result;
}
int lwip_send(int s, const void *p, size_t n, int f) {
    assert(s == 7 && p && f == 0);
    io_size = n;
    errno = io_error;
    return io_result;
}
int lwip_recv(int s, void *p, size_t n, int f) {
    assert(s == 7 && p && f == 0);
    io_size = n;
    errno = io_error;
    return io_result;
}
uint16_t inet_chksum(const void *p, uint16_t n) {
    const unsigned char *b = p;
    uint32_t sum = 0;
    for (unsigned i = 0; i < n; i += 2)
        sum += ((uint16_t)b[i] << 8) | (i + 1 < n ? b[i + 1] : 0);
    while (sum >> 16)
        sum = (sum & 65535) + (sum >> 16);
    return htons((uint16_t)~sum);
}
int lwip_sendto(int s, const void *p, size_t n, int f, const struct sockaddr *a,
                socklen_t l) {
    assert(s == 7 && f == 0 && l == sizeof(destination));
    assert(n <= sizeof(request));
    memcpy(request, p, n);
    request_size = n;
    destination = *(const struct sockaddr_in *)a;
    assert(inet_chksum(p, n) == 0);
    assert(request[0] == ICMP_ECHO && request[1] == 0);
    return sendto_error ? -1 : (int)n;
}
int lwip_setsockopt(int s, int level, int option, const void *p, socklen_t n) {
    assert(s == 7 && level == SOL_SOCKET && option == SO_RCVTIMEO &&
           n == sizeof(struct timeval));
    const struct timeval *t = p;
    long ms = t->tv_sec * 1000 + t->tv_usec / 1000;
    assert(ms > 0 && ms <= previous_timeout);
    previous_timeout = ms;
    return option_error;
}
int lwip_recvfrom(int s, void *p, size_t n, int f, struct sockaddr *a,
                  socklen_t *l) {
    assert(s == 7 && f == 0 && n >= 60);
    assert(*l == sizeof(struct sockaddr_in));
    *l = 1;
    *(struct sockaddr_in *)a = destination;
    ++recv_calls;
    tick += 100;
    unsigned char *b = p;
    memset(b, 0, n);
    b[0] = 0x45;
    memcpy(b + 20, request, request_size);
    b[20] = ICMP_ER;
    if (packet_mode == 1)
        return -1;
    if (packet_mode == 2) {
        b[0] = 0x65;
        return 28;
    }
    if (packet_mode == 3 && recv_calls == 1) {
        b[0] = 0x44;
        return 28;
    }
    if (packet_mode == 4 && recv_calls == 1)
        return 10;
    if (packet_mode == 5 && recv_calls == 1)
        b[24] ^= 1;
    if (packet_mode == 6 && recv_calls == 1)
        ((struct sockaddr_in *)a)->sin_addr.s_addr ^= 1;
    return 28;
}
static network_config_t config = {.ip = "192.168.1.20",
                                  .netmask = "255.255.255.0"};
static void ready(void) {
    gnetif.input = &gnetif;
    gnetif.up = gnetif.link = true;
    gnetif.ip.addr = 1;
}
#define CASE(name) if (strcmp(argv[1], #name) == 0)
int main(int argc, char **argv) {
    assert(argc == 2);
    unsigned char buffer[8] = {0};
    struct NetworkContext ctx = {.socket = -1};
    CASE(data_init) {
        assert(!f_data_init());
        gnetif.linkoutput = &gnetif;
        assert(f_data_init());
    }
    else CASE(data_mac) {
        assert(!f_data_set_mac(buffer));
        assert(f_data_get_mac() == NULL);
        gnetif.hwaddr_len = 6;
        assert(!f_data_set_mac(NULL));
        buffer[0] = 2;
        buffer[5] = 42;
        assert(f_data_set_mac(buffer));
        assert(memcmp(f_data_get_mac(), buffer, 6) == 0);
        buffer[5] = 0;
        assert(gnetif.hwaddr[5] == 42);
        callback_error = -1;
        assert(!f_data_set_mac(buffer));
        assert(gnetif.hwaddr[5] == 42);
        callback_error = 0;
        mac_update_error = -1;
        assert(!f_data_set_mac(buffer));
        assert(gnetif.hwaddr[5] == 42);
        mac_update_error = 0;
        buffer[0] = 3;
        assert(!f_data_set_mac(buffer));
        memset(buffer, 0, sizeof(buffer));
        assert(!f_data_set_mac(buffer));
    }
    else CASE(phy_ids) {
        assert(f_phy_init());
        for (int i = 1; i <= 2; ++i) {
            phy_reads = 0;
            phy_fail = i;
            assert(!f_phy_init());
            assert(phy_reads == i);
        }
        phy_fail = 0;
        for (int i = 1; i <= 2; ++i) {
            phy_reads = 0;
            phy_invalid = i;
            assert(!f_phy_init());
        }
    }
    else CASE(phy_zero_ids) {
        for (int i = 1; i <= 2; ++i) {
            phy_reads = 0;
            phy_zero = i;
            assert(!f_phy_init());
            assert(phy_reads == i);
        }
    }
    else CASE(phy_states) {
        assert(!f_phy_is_up() && !f_phy_is_link_up());
        ready();
        assert(f_phy_is_up() && f_phy_is_link_up());
        int speeds[] = {0, 100, 100, 10, 10};
        for (int i = 0; i < 5; ++i) {
            phy_state = i;
            assert(f_link_state() == i);
            assert(f_phy_get_speed_and_mode() == speeds[i]);
        }
        phy_state = -1;
        assert(f_phy_get_speed_and_mode() == 0);
    }
    else CASE(phy_wait) {
        assert(!f_phy_wait_for_link());
        assert(tick == 5000);
        tick = 0;
        ready_at = 2000;
        assert(f_phy_wait_for_link());
        assert(tick == 2000);
    }
    else CASE(phy_monitor) {
        monitor_iterations = 3;
        phy_state = 1;
        if (setjmp(monitor_exit) == 0)
            f_phy_monitor();
        assert(tick == 750);
    }
    else CASE(network_static) {
        ready();
        assert(f_network_init(NETWORK_MODE_DIRECT, &config));
        assert(dhcp_calls == 0);
        assert(gnetif.ip.addr == inet_addr(config.ip));
        assert(f_network_get_mode() == NETWORK_MODE_DIRECT);
    }
    else CASE(network_validation) {
        assert(!f_network_init(NETWORK_MODE_DIRECT, NULL));
        assert(!f_network_init(99, &config));
        const char *bad[] = {NULL, "invalid", "0.0.0.0"};
        for (int i = 0; i < 3; ++i) {
            network_config_t c = config;
            c.ip = bad[i];
            assert(!f_network_init(0, &c));
        }
        config.netmask = "255.0.255.0";
        assert(!f_network_init(0, &config));
        config.netmask = "255.255.255.0";
        config.gateway = "bad";
        assert(!f_network_init(0, &config));
        config.gateway = NULL;
        config.dns = "bad";
        assert(!f_network_init(0, &config));
        assert(dhcp_calls == 0);
    }
    else CASE(network_dhcp) {
        ready();
        network_config_t c = {.use_dhcp = true};
        assert(f_network_init(NETWORK_MODE_LAN, &c));
        assert(dhcp_calls == 1);
        dhcp_error = -1;
        assert(!f_network_init(NETWORK_MODE_REMOTE, &c));
        assert(f_network_get_mode() == NETWORK_MODE_LAN);
        c.ip = config.ip;
        assert(!f_network_init(0, &c));
    }
    else CASE(network_callback) {
        assert(!f_network_init(0, &config));
        ready();
        callback_error = 1;
        assert(!f_network_init(0, &config));
        callback_error = 0;
        config.dns = "1.1.1.1";
        assert(f_network_init(0, &config));
        assert(dns.addr == inet_addr(config.dns));
    }
    else CASE(network_ready) {
        assert(!f_network_is_ready());
        ready();
        assert(f_network_has_ip());
        assert(!f_network_has_gateway());
        assert(f_network_is_ready());
        assert(f_network_init(NETWORK_MODE_REMOTE, &config));
        assert(!f_network_is_ready());
        gnetif.gw.addr = 1;
        assert(f_network_is_ready());
        gnetif.link = false;
        assert(!f_network_is_ready());
        assert(lock_depth == 0);
    }
    else CASE(network_wait) {
        assert(!f_network_wait_for_ip(0));
        assert(!f_network_wait_for_ip(5));
        assert(tick == 5);
        ready_at = 8;
        assert(f_network_wait_for_ip(10));
        gnetif.ip.addr = 0;
        ready_at = UINT32_MAX;
        delay_error = -1;
        assert(!f_network_wait_for_ip(10));
        delay_error = 0;
        tick = UINT32_MAX - 2;
        assert(!f_network_wait_for_ip(5));
        assert(tick == 2);
    }
    else CASE(ping_success) {
        ready();
        assert(f_ping("192.168.1.42"));
        assert(destination.sin_addr.s_addr == inet_addr("192.168.1.42"));
        assert(request_size == 40);
        assert(recv_calls == 1);
        assert(request[4] == 0xab && request[5] == 0xcd);
    }
    else CASE(ping_timeout) {
        ready();
        packet_mode = 2;
        assert(!f_ping(PING_TARGET_IP));
        assert(tick == 1000 && recv_calls == 10);
    }
    else CASE(ping_filter) {
        ready();
        packet_mode = atoi(getenv("PACKET_MODE") ? getenv("PACKET_MODE") : "3");
        assert(f_ping(PING_TARGET_IP));
        assert(recv_calls == 2);
    }
    else CASE(ping_errors) {
        assert(!f_ping(PING_TARGET_IP));
        ready();
        assert(!f_ping(NULL));
        assert(!f_ping(""));
        assert(!f_ping("invalid-ip"));
        assert(request_size == 0);
        socket_result = -1;
        assert(!f_ping(PING_TARGET_IP));
        socket_result = 7;
        sendto_error = 1;
        assert(!f_ping(PING_TARGET_IP));
        assert(close_calls == 1);
        sendto_error = 0;
        option_error = -1;
        assert(!f_ping(PING_TARGET_IP));
        assert(close_calls == 2);
    }
    else CASE(ping_wait) {
        assert(!f_wait_ping(PING_TARGET_IP));
        assert(tick == 20000);
        ready();
        assert(f_wait_ping("192.168.1.42"));
        assert(destination.sin_addr.s_addr == inet_addr("192.168.1.42"));
    }
    else CASE(transport_validation) {
        assert(!f_transport_connect(NULL, "1.2.3.4", 1883));
        assert(!f_transport_connect(&ctx, NULL, 1883));
        assert(!f_transport_connect(&ctx, "bad", 1883));
        assert(!f_transport_connect(&ctx, "1.2.3.4", 0));
        assert(f_transport_send(&ctx, buffer, 8) == -1);
        assert(f_transport_receive(&ctx, buffer, 8) == -1);
        assert(!f_transport_close(NULL));
    }
    else CASE(transport_connect) {
        assert(f_transport_connect(&ctx, "192.168.1.10", 1883));
        assert(ctx.socket == 7);
        assert(destination.sin_port == htons(1883));
        assert(destination.sin_addr.s_addr == inet_addr("192.168.1.10"));
        assert(!f_transport_connect(&ctx, "1.2.3.4", 1883));
        assert(errno == EISCONN);
        assert(f_transport_close(&ctx));
        assert(ctx.socket == -1);
        assert(f_transport_close(&ctx) && close_calls == 1);
    }
    else CASE(transport_failures) {
        socket_result = -1;
        assert(!f_transport_connect(&ctx, "1.2.3.4", 1883));
        assert(close_calls == 0);
        socket_result = 7;
        connect_result = -1;
        assert(!f_transport_connect(&ctx, "1.2.3.4", 1883));
        assert(errno == ECONNREFUSED && ctx.socket == -1 && close_calls == 1);
        connect_result = 0;
        fcntl_result = -1;
        assert(!f_transport_connect(&ctx, "1.2.3.4", 1883));
        assert(errno == EIO && close_calls == 2);
        ctx.socket = 7;
        close_result = -1;
        assert(!f_transport_close(&ctx));
        assert(ctx.socket == 7);
    }
    else CASE(transport_io) {
        ctx.socket = 7;
        assert(f_transport_send(&ctx, buffer, 8) == 3 && io_size == 8);
        assert(f_transport_receive(&ctx, buffer, 8) == 3);
        io_result = -1;
        int transient[] = {EAGAIN, EWOULDBLOCK, EINTR};
        for (int i = 0; i < 3; ++i) {
            io_error = transient[i];
            assert(f_transport_send(&ctx, buffer, 8) == 0);
            assert(f_transport_receive(&ctx, buffer, 8) == 0);
        }
        io_error = ECONNRESET;
        assert(f_transport_send(&ctx, buffer, 8) == -1);
        assert(f_transport_receive(&ctx, buffer, 8) == -1);
        io_result = 0;
        assert(f_transport_receive(&ctx, buffer, 8) == -1 && errno == ENOTCONN);
        assert(f_transport_receive(&ctx, buffer, 0) == 0);
        assert(f_transport_send(&ctx, NULL, 8) == -1);
        assert(f_transport_receive(NULL, buffer, 8) == -1);
        io_result = 1;
        assert(f_transport_send(&ctx, buffer, SIZE_MAX) == 1 &&
               io_size == INT_MAX);
    }
    else {
        fprintf(stderr, "Unknown test %s\n", argv[1]);
        return 1;
    }
    printf("PASS %s\n", argv[1]);
    return 0;
}
