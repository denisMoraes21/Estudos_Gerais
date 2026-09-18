# Regeneração no STM32CubeMX

Abra `test.ioc` no STM32CubeMX 6.17.0, usando STM32Cube FW_H7 V1.13.0,
e execute **Generate Code**. Mantenha **Keep User Code** habilitado e
**Delete previously generated files when not re-generated** desabilitado.
O projeto usa CMake; compile pela raiz:

```sh
cmake --preset Debug
cmake --build --preset Debug -j4
```

As configurações de Ethernet MII, descritores DMA, buffer RX de 1536 bytes,
heap lwIP de 16384 bytes, heap FreeRTOS de 32768 bytes e hooks de falha
estão registradas no `.ioc`. Os caches permanecem desabilitados, como no
firmware validado na placa. As antigas entradas inválidas de MPU região 1
foram removidas; elas não eram aplicadas pelo gerador.

Os arquivos mantidos pelo projeto são:

- `Common/Src/ethernet/ethernetif_port.c`: driver com as correções de DMA,
  buffers e manutenção de cache.
- `Common/Src/ethernet/lwip_port.c`: inicialização e acompanhamento do link.
- `CM7/Linker/ethernet_flash_CM7.ld` e `CM4/Linker/ethernet_flash_CM4.ld`:
  reserva dos primeiros 64 KiB da SRAM D2 para Ethernet, sem sobreposição
  com a RAM do CM4.

Os `CMakeLists.txt` selecionam esses arquivos. As versões de `ethernetif.c`
e `lwip.c` em `CM7/Src` continuam sendo geradas, mas não são compiladas.
Não transfira as correções para esses arquivos nem para `mx-generated.cmake`.

Em `main.c`, os blocos `USER CODE` mantêm o clock da SRAM D2 e a chamada
`MX_LWIP_Init()` antes de liberar o CM4 pelo HSEM. Preserve essa ordem para
evitar a disputa de configuração dos GPIOs. Os hooks do FreeRTOS ficam nos
blocos `USER CODE 4` e `USER CODE 5` de `freertos.c`. O ponteiro do heap lwIP
é vinculado ao símbolo do linker em `USER CODE 1` de `lwipopts.h`.

Após regenerar, execute os testes:

```sh
cmake -S tests/unit -B tests/unit/build
cmake --build tests/unit/build -j4
ctest --test-dir tests/unit/build --output-on-failure
```

Para validar a placa com o firmware recém-compilado (ST-LINK conectado):

```sh
STM32_Programmer_CLI -c port=SWD mode=UR reset=HWrst ap=0 -w CM7/build/test_CM7.elf -v -hardRst
ping -n -q -I enp0s31f6 -c 20 -i 0.2 -W 1 -M do -s 56 192.168.1.21
ping -n -q -I enp0s31f6 -c 20 -i 0.2 -W 1 -M do -s 1472 192.168.1.21
```

Esses comandos de ping pressupõem a interface do computador em
`192.168.1.10/24` e a placa em `192.168.1.21`. O segundo tamanho testa um
pacote IPv4 de 1500 bytes sem fragmentação.
