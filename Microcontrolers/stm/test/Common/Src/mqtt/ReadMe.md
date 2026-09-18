# Cliente MQTT

O cliente em `mqtt_client.c` implementa conexão MQTT sem TLS pelo lwIP:
endereço IP ou DNS, tentativa assíncrona, callback de resultado, consulta de
estado e desconexão. Os fontes estão no CMake do CM7.

## Uso

Chamar de uma tarefa FreeRTOS, depois de inicializar o lwIP e configurar a rede.
O `main.c` inclui um teste após `f_network_checkout()`: conecta sem TLS e
publica uma vez em `/bms`, com QoS 1 e sem retain. O JSON usa valores
simulados de tensão (V) e corrente (A). Ajuste `mqtt_config` para o broker real.

```c
static void on_mqtt_state(uint8_t state, int result, void *argument)
{
    (void)argument;
    /* Enviar um evento para a tarefa da aplicação, sem bloquear.
     * Não chamar APIs f_mqtt_* daqui, nem mesmo f_mqtt_get_state(). */
    LOG_INFO("MQTT state=%u result=%d", (unsigned int)state, result);
}

static const app_mqtt_config_t config = {
    .broker_hostname = "192.168.1.10", /* Substituir pelo broker real. */
    .broker_port = APP_MQTT_PLAIN_BROKER_PORT,
    .client_id = APP_MQTT_DEVICE_ID,
    .keep_alive_s = APP_MQTT_KEEP_ALIVE_S,
    .on_state = on_mqtt_state,
};

/* Dentro da tarefa, com a rede pronta: */
int result = f_mqtt_init(&config);
if (result == APP_MQTT_OK) {
    result = f_mqtt_connect();
}
/* OK significa tentativa aceita; aguardar on_state(CONNECTED, ...). */
```

Headers do exemplo: `mqtt_client.h`, `mqtt_topics.h` e `logger.h`.
Para hostname, configurar um servidor DNS alcançável; IP literal não usa DNS.
A porta é sempre `config.broker_port`, sem substituição automática.

## Estado e concorrência

- `f_mqtt_init()` valida e copia a configuração, sem abrir conexão.
- `f_mqtt_connect()` passa para CONNECTING e inicia DNS/TCP/MQTT. O retorno
  imediato informa aceitação ou falha inicial. A confirmação do broker chega
  em `on_state`, que também informa recusa, timeout e perda de conexão.
- `f_mqtt_get_state()` consulta DISCONNECTED, CONNECTING ou CONNECTED.
- `f_mqtt_disconnect()` cancela a tentativa ou encerra a conexão.
- Conexão repetida ou reinicialização com conexão ativa retorna BUSY.
- Se houver DNS pendente ao desconectar, seu resultado será descartado.
  Novas conexões/reinicializações retornam BUSY até essa consulta terminar.
- Falhas de inicialização invalidam a configuração anterior, exceto BUSY ou
  falha no despacho ao lwIP, que preservam o cliente.

As operações de conexão/estado/inicialização usam `tcpip_callback_wait()` para
proteger o acesso ao core lwIP. Elas podem esperar pelo core/despacho, mas não
esperam a resposta do broker. Não chamar de interrupções nem de callbacks lwIP.
Os callbacks da aplicação executam com o core protegido e devem retornar rápido.

A estrutura de configuração é copiada. As strings, certificados e o argumento
dos callbacks continuam pertencendo à aplicação: manter esses dados válidos e
sem alterações enquanto o cliente os usa, inclusive durante DNS pendente.
O objeto lwIP é alocado uma vez e reutilizado nas reconexões.

## TLS e partes pendentes

`f_mqtt_init_tls()` tenta preparar TLS e ainda retorna TLS_UNAVAILABLE com uma
configuração válida: Mbed TLS e ALTCP TLS não estão integrados. O caminho de
conexão também rejeita o modo TLS até a integração completa, sem fallback TCP.
A porta usual de MQTTS está em `APP_MQTT_TLS_BROKER_PORT` (8883).

Para MQTTS falta integrar biblioteca/adaptador, entropia do hardware, horário
para validar certificados, memória e validação da cadeia e do hostname/SNI
antes do handshake. A API MQTT local recebe IP; será necessário integrar a
configuração do hostname no transporte TLS antes de iniciar a negociação.

`f_mqtt_publish()` implementa QoS 0/1, copia tópico/payload para o lwIP antes
de retornar e sincroniza com o core. OK indica enfileiramento; no QoS 1,
o callback confirma o PUBACK do broker. Desconexão cancela callbacks pendentes
com NOT_CONNECTED. O argumento do callback deve permanecer válido.
Assinatura, cancelamento de assinatura e recepção continuam pendentes.
`mqtt_service.c` continua sendo a interface provisória do serviço: seleciona
MQTTS, mas ainda não cria tarefa, filas, telemetria ou reconexão automática.

## Testes

`tests/unit/test_mqtt_connection.c` simula o lwIP para verificar IP literal,
DNS imediato/pendente, cancelamento, reconexão, recusa do broker, timeout,
falta de memória, falha de despacho e ausência de fallback TLS. A compilação
ARM verifica compatibilidade com os headers reais. A validação de conexão em
placa com um broker real ainda precisa ser feita.

Referências:
- https://www.nongnu.org/lwip/2_1_x/group__mqtt.html
- https://www.nongnu.org/lwip/2_1_x/group__altcp__tls.html
