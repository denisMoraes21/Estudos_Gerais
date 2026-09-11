# Testes unitários das camadas Ethernet

Requisitos: compilador C do computador (GCC ou Clang), CMake e CTest.
Execute na raiz do projeto, sem usar o toolchain ARM:

```sh
cmake -S tests/unit -B tests/unit/build
cmake --build tests/unit/build -j
ctest --test-dir tests/unit/build --output-on-failure
```

Para executar somente transporte: `ctest --test-dir tests/unit/build -R transport --output-on-failure`.

Os testes compilam os arquivos de produção de física, enlace, rede e transporte.
HAL, RTOS e serviços lwIP são substituídos pelos doubles em `mocks/` e
`test_layers.c`. Não precisam de placa, broker, rede ou privilégios de raw socket.
Cada caso roda num processo separado, isolando os estados estáticos de produção.
Assertions permanecem habilitadas mesmo no build Release. Um teste falho retorna
código diferente de zero. O relógio avança virtualmente, sem esperas reais.

## Cenários

- Física: identificação do PHY, falhas nas duas leituras MDIO, valor 0xffff,
  estado de interface/link, velocidades e modos, espera e timeout, monitoramento.
  O loop infinito do monitor é interrompido pelo mock após três atrasos.
- Enlace: disponibilidade de linkoutput, tamanho de MAC, argumentos nulos,
  cópia e leitura do endereço.
- Rede: configuração estática sem iniciar DHCP, DHCP e falha de inicialização,
  endereços inválidos, máscara não contígua, DNS, falha no callback e interface
  não inicializada, modos direto/remoto, gateway, espera por IP, erro do RTOS
  e transbordamento do contador de tempo.
- Ping: requisição/checksum, resposta válida, timeout total sob tráfego inválido,
  versão IPv4 incorreta, IHL inválido, pacote curto, identificador/origem errados,
  reinicialização do comprimento da origem, falhas de socket/envio/timeout e
  espera por conectividade.
- Transporte: validação, IP/porta de destino, configuração não bloqueante,
  limpeza após falha, envio parcial, erros transitórios e permanentes, EOF,
  buffer de tamanho zero, limite INT_MAX e fechamento idempotente.

## Limites

Os testes verificam a lógica própria, não a implementação de HAL/lwIP nem
comunicação real, temporização do RTOS, DMA/cache, TLS ou integração coreMQTT.
Os mocks habilitam DHCP e DNS e representam apenas as APIs usadas; não cobrem
as variantes compiladas com esses recursos desabilitados. O parser de IP usa
inet_pton do host, cujo conjunto de formatos aceitos não é idêntico ao lwIP.
Não se afirma cobertura integral de branches. Física e rede são ligadas juntas,
portanto alguns testes de rede também exercitam a interface física real.

Sessão, apresentação, aplicação e dhcp.c não possuem lógica própria a testar.
`f_phy_link_status()` está declarada em physical.h, mas não implementada: não
há teste de comportamento para ela. O estado privado de duplex não tem getter;
os testes exercitam os estados do PHY e verificam a velocidade pública.

O teste network_static revelou e protege a correção de um defeito: dhcp_start
não deve ser chamado quando use_dhcp é false.

A configuração `.clangd` usa `tests/unit/build/compile_commands.json` para
os testes e mantém os headers ARM restritos ao firmware. Execute a configuração
CMake acima antes de abrir os testes no editor.
