# Gabarito — 01/10/2026

## Prática

- Pisca: 500 + 500 = 1000 ms por ciclo (~1 Hz).
- Botão: HIGH/solto → false/apagado; LOW/pressionado → true/aceso.
- Dois botões: apenas a combinação pressionado/pressionado acende o LED.
- Soltar qualquer botão apaga a saída na leitura seguinte. Durante `setup`
  a saída é colocada em LOW; a primeira execução de `loop` aplica a regra.
- No reset com os botões pressionados, o LED acende assim que `loop` avalia
  as entradas. Não há retenção de estado anterior nem bloqueio de partida.

## Exercícios

1. Microcontrolador é o circuito integrado com CPU, memória e periféricos.
   A placa agrega esse componente, conectores, alimentação e outros circuitos.
2. Sem caminho que estabeleça um nível definido, uma entrada de alta impedância
   pode variar por interferências. O pull-up mantém o nível alto quando o botão
   está aberto e permite nível baixo quando o botão o conecta ao GND.
3. Trocar `solicitacao && permissao` por `solicitacao || permissao`. Tabela
   lógica para (false,false), (true,false), (false,true), (true,true):
   false, true, true, true. A conversão `digitalRead(...) == LOW` permanece.
4. 250 + 250 = 500 ms por ciclo; frequência aproximada de 2 Hz.

## Desafio: piscar enquanto o botão está pressionado

Conectar um botão entre D2 e GND. O primeiro acendimento deve ocorrer 500 ms
após detectar uma nova pressão; soltar apaga e cancela a contagem anterior.
O exemplo acompanha níveis e não conta pressões; não faz filtragem de rebote.

```cpp
const int BOTAO = 2;
const unsigned long INTERVALO_MS = 500;
unsigned long anterior = 0;
bool estavaPressionado = false;
bool aceso = false;

void setup() {
    pinMode(BOTAO, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    unsigned long agora = millis();
    bool pressionado = (digitalRead(BOTAO) == LOW);
    if (!pressionado) {
        aceso = false;
    } else if (!estavaPressionado) {
        anterior = agora;
        aceso = false;
    } else if (agora - anterior >= INTERVALO_MS) {
        anterior = agora;
        aceso = !aceso;
    }
    if (aceso) digitalWrite(LED_BUILTIN, HIGH);
    else digitalWrite(LED_BUILTIN, LOW);
    estavaPressionado = pressionado;
}
```

Verificar: solto por tempo prolongado → apagado; pressionar → começa apagado;
499 ms → apagado; 500 ms → aceso; 1000 ms → apagado; soltar antes do próximo
intervalo → apagado; pressionar novamente → nova contagem de 500 ms.

Critérios de entrega: esquema coerente com os pinos, interpretação de LOW,
quatro combinações corretas, transições de liberação testadas e justificativa
da lógica. Resultado no computador não comprova a ligação física da placa.
