# Gabarito — 25/09/2026

Use após tentar a prática e os exercícios. Os códigos completos dos exemplos estão em `exemplos/`.

## Prática guiada: reservatório

A versão com repetição está em [04_reservatorio.c](exemplos/04_reservatorio.c). A ordem das verificações é importante: leitura válida; sentinela -1; intervalo permitido; faixas.

| Entrada | Resultado |
| --- | --- |
| -2, 101 | Nível inválido |
| 0, 29 | Baixo |
| 30, 79 | Adequado |
| 80, 100 | Alto |
| -1 | Encerrar sem classificar |
| Texto `x` | Entrada inválida e retorno 1 |

Para a versão de uma única leitura, remova o `do/while`, a opção sentinela e o `break`; mantenha a validação e a cadeia de condições. Para a extensão, o código disponibilizado rejeita valores fora da faixa e continua solicitando novos valores. O `break` permite sair antes de qualquer classificação do sentinela.

## Exercício 1 — Contador e acumulador

O corpo executa para `i` igual a 0, 1, 2 e 3. As somas parciais são 0, 1, 3 e 6. Ao testar 4, a condição `i < 4` é falsa. Soma final: **6**. A variável declarada no `for` não fica acessível fora dele.

```c
int soma = 0;
for (int i = 0; i < 4; i++) {
    soma += i;
}
```

## Exercício 2 — Duas permissões

```c
int esteira = 0;
if (peca_detectada && protecao_fechada) {
    esteira = 1;
}
```

| Peça | Proteção fechada | Esteira |
| --- | --- | --- |
| 0 | 0 | 0 |
| 0 | 1 | 0 |
| 1 | 0 | 0 |
| 1 | 1 | 1 |

As variáveis de entrada devem ser declaradas e inicializadas antes do trecho. A saída também pode ser atribuída diretamente pela expressão lógica; a versão acima explicita a decisão ensinada.

## Exercício 3 — Menu e ciclos

Solução completa: [03_menu.c](exemplos/03_menu.c). O `do/while` apresenta o menu ao menos uma vez; o `switch` seleciona a opção; o `for` executa três ciclos no caso 2. O `break` desse caso fica depois do laço e encerra o `switch`. A opção 0 termina o menu quando a condição do `do/while` é reavaliada.

Entrada de teste: `1`, `2`, `9`, `0`. Esperado: consulta, ciclos 1–3, opção inválida e encerramento. Um inteiro diferente de 0 não termina o laço; uma leitura sem inteiro termina com diagnóstico e retorno 1.

## Exercício 4 — Progresso do laço

Sem atualizar `ciclo`, a condição permanece verdadeira para sempre, se o valor inicial for menor que 5.

```c
int ciclo = 0;
while (ciclo < 5) {
    printf("Ciclo %d\n", ciclo);
    ciclo++;
}
```

Executa para 0, 1, 2, 3 e 4. No teste seguinte, `ciclo` vale 5 e o laço encerra. `printf` exige `#include <stdio.h>` em um programa completo.

## Exercício 5 — break e continue

Com `continue` em 2 e `break` em 5, a saída mostra ciclos **1, 3 e 4**. O ciclo 2 pula a impressão; o ciclo 5 termina o laço antes da impressão. A atualização do `for` ainda acontece após `continue`.

## Verificação dos exemplos de aula

- `01_decisao.c`: entrada simulada 28 + ajuste 2 = 30; a igualdade faz o ventilador ligar.
- `02_ciclos.c`: temperaturas 26, 28, 30, 32, 34; estados 0, 0, 1, 1, 1; três ciclos ligado. A variável `temperatura` termina em 36, mas esse valor não foi medido nem exibido no corpo.
- A regra não modela histerese, temporização ou potência elétrica: essas extensões precisam de requisitos próprios.
