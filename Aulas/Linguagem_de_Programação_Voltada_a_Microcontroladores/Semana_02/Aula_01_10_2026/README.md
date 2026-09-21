# Noções de programação para microcontroladores — 01/10/2026

Semana 02, encontro 8. Conteúdo: 8.1 microcontroladores e automação;
8.2 GPIO; 8.3 aplicações simples. São 30 slides em `main.pdf`, com fontes
Beamer em `main.tex` e `page_*.tex`. O logo foi reutilizado das aulas FPFtech.

## Ambiente e materiais

Placa de referência: **Arduino Uno R3**, cabo USB de dados, dois botões
momentâneos, protoboard e fios. Usamos o LED integrado. Não é necessário
LED externo nem resistor externo nos botões, pois os exemplos ativam
`INPUT_PULLUP`. Conferir os contatos do botão; em modelos de quatro terminais
existem pares já unidos internamente. Monte as conexões sem alimentação.

O Arduino IDE precisa do suporte Arduino AVR Boards e da placa Arduino Uno
selecionada. Cada sketch fica em uma pasta com o mesmo nome do arquivo `.ino`;
abra **um exemplo por vez**, compile e envie à porta correta.
O código `.ino` usa C++ e a API Arduino, enquanto as aulas anteriores usam C11.

## Sequência de prática

1. `exemplos/pisca/pisca.ino`: 500 ms aceso e 500 ms apagado; período ~1 s.
2. `exemplos/botao_led/botao_led.ino`: botão entre D2 e GND; pressionado acende
   o LED. Monitor Serial a 9600 baud: 1 pressionado, 0 solto. A espera de 100 ms
   reduz as impressões e também limita a frequência de leitura.
3. `exemplos/sinalizador/sinalizador.ino`: os dois botões devem estar pressionados
   para acender. Soltar qualquer um apaga o LED no próximo ciclo.
4. `exemplos/pisca_sem_bloqueio/pisca_sem_bloqueio.ino`: LED alterna a cada 500 ms,
   deixando espaço no ciclo para outras tarefas curtas. Extensão após a prática básica.

| Sinal | Pino | Ligação |
| --- | --- | --- |
| Solicitação | D2 | Botão entre D2 e GND |
| Permissão | D3 | Botão entre D3 e GND |
| Saída | LED_BUILTIN (D13 no Uno R3) | LED integrado |
| Referência | GND | Comum aos botões |

`INPUT_PULLUP`: solto = HIGH; pressionado = LOW. O programa converte o nível
elétrico em `bool` antes de aplicar a regra. A montagem é um modelo didático de
sinalização, sem acionamento de máquinas nem função de segurança industrial.
O Uno R3 usa lógica de 5 V; não transportar essa suposição para outras placas.

## Testes de bancada

| Solicitação | Permissão | LED esperado |
| --- | --- | --- |
| Solto | Solto | Apagado |
| Pressionado | Solto | Apagado |
| Solto | Pressionado | Apagado |
| Pressionado | Pressionado | Aceso |

Além das quatro combinações, solte cada botão a partir da condição de LED
aceso e reinicie a placa com ambos soltos e com ambos pressionados.
O código não memoriza autorização: os níveis são consultados em todo ciclo.
Rebote pode produzir transições breves; contagem de pressões requer tratamento
adicional, conforme discutido nos slides.

## Validação no computador

`validacao/testar.py` compila os quatro sketches com um substituto mínimo da
API Arduino e verifica saídas e transições. Execute na pasta da aula:

```sh
python3 validacao/testar.py
```

Requer Python 3 e g++. É uma simulação de lógica em C++, não uma compilação
para o ATmega328P. Ela não simula eletricidade, rebote, bootloader ou tempos
reais. Para validar o alvo, compile no Arduino IDE e execute os testes de bancada.

## Recompilar slides

```sh
pdflatex -interaction=nonstopmode -halt-on-error main.tex
pdflatex -interaction=nonstopmode -halt-on-error main.tex
```

`gabarito.md` contém respostas e solução comentada do desafio.

## Referências oficiais

- [Arduino Uno R3](https://docs.arduino.cc/hardware/uno-rev3/).
- [Manual do Uno R3](https://docs.arduino.cc/resources/datasheets/A000066-datasheet.pdf).
- [pinMode e INPUT_PULLUP](https://github.com/arduino/reference-en/blob/master/Language/Functions/Digital%20IO/pinMode.adoc).
- [Exemplo oficial de leitura com pull-up](https://github.com/arduino/arduino-examples/blob/main/examples/02.Digital/DigitalInputPullup/DigitalInputPullup.ino).
- [Blink Without Delay](https://github.com/arduino/arduino-examples/blob/main/examples/02.Digital/BlinkWithoutDelay/BlinkWithoutDelay.ino).
- [millis](https://github.com/arduino/reference-en/blob/master/Language/Functions/Time/millis.adoc).
- [Inicialização e ciclo do núcleo AVR](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/main.cpp).

Os diagramas e exemplos desta aula foram preparados para este curso; as
referências sustentam os conceitos de placa, API e temporização.
