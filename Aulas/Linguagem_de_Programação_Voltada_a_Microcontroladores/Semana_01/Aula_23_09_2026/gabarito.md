# Gabarito — 23/09/2026

## Atividade guiada

Solução completa: `exemplos/03_reservatorio.c`. Os valores finais são nível atual 210 L, nível anterior 180 L, temperatura 24,5 °C, setor B e alarme falso (0). A atribuição para `nivel_anterior_l` copia 180 naquele momento. Atualizar `nivel_l` não altera a cópia.

## Exercício 1 — Escolha de tipos

- Quantidade de peças: `int` atende a contagens pequenas; `unsigned int` também pode ser adequado se os valores e operações forem controlados. A escolha depende da faixa necessária.
- Temperatura de −2,5 °C: `float` ou `double`, porque existe parte fracionária; `double temperatura_c = -2.5;`.
- Setor C: `char setor = 'C';`.
- Presença de peça: `bool peca_presente = true;`, com `<stdbool.h>`.

Não exigir que `int` tenha um tamanho fixo em todas as plataformas.

## Exercício 2 — Rastreamento

| Após a instrução | a | b |
|---|---:|---:|
| `int a = 7;` | 7 | ainda não declarada |
| `int b = a;` | 7 | 7 |
| `a = 9;` | 9 | 7 |
| `b = 4;` | 9 | 4 |

Resultado final: `a = 9` e `b = 4`.

## Exercício 3 — Painel

Solução compilável: `exemplos/04_exercicio_painel.c`. Saída:

```text
Pecas: 120
Tensao: 3.3 V
Setor: C
Pronto: 1
```

O formato `%d` recebe inteiros, `%.1f` exibe uma casa decimal e `%c` exibe o caractere. `bool` é promovido para `int` na chamada de `printf`, permitindo sua exibição como 0 ou 1.

## Exercício 4 — Correções

```c
double t = 21.5;
char s = 'B';
int n = 0;
printf("%d", n);
```

O separador decimal no código é ponto. Aspas simples representam o caractere; aspas duplas produzem uma string. A variável automática local `n` precisa de valor antes da leitura; 0 é uma inicialização possível, a depender da intenção do programa.

## Bilhete de saída

`int nivel = 10;` declara e inicializa uma variável inteira. `nivel = 20;` atribui um novo valor a uma variável já declarada. Um exemplo de `bool` é `bool porta_fechada = true;`.
