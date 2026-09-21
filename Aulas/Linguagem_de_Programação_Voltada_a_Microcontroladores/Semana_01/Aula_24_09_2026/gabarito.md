# Gabarito — 24/09/2026

## Atividade guiada

Solução completa: `exemplos/03_operacao.c`.

```c
bool nivel_adequado = nivel_l >= 100;
bool na_faixa = (temperatura_c >= 20.0) && (temperatura_c <= 30.0);
bool habilitado = comando && nivel_adequado && na_faixa && !falha;
```

Com nível de 180 L, temperatura de 28,0 °C, comando verdadeiro e falha falsa, os três resultados são 1. Com 99 L, o nível adequado passa a 0, a temperatura continua na faixa (1) e o resultado habilitado passa a 0.

É preciso recalcular as expressões após alterar `nivel_l`: uma atribuição guarda o resultado calculado naquele momento, sem criar um vínculo automático entre as variáveis.

## Exercício 1 — Resultados

| Expressão | Resultado | Motivo |
|---|---:|---|
| `19 / 4` | 4 | Divisão inteira |
| `19 % 4` | 3 | Resto: 19 = 4 × 4 + 3 |
| `19 / 4.0` | 4,75 | Divisão em ponto flutuante |
| `2 + 3 * 4` | 14 | Multiplicação antes da adição |
| `(2 + 3) * 4` | 20 | Parênteses agrupam a adição |

## Exercício 2 — Embalagem

Solução compilável: `exemplos/04_exercicio_embalagem.c`. Use `pecas / capacidade` e `pecas % capacidade` com inteiros. Para 29 peças e capacidade de 6, são 4 caixas completas e 5 peças restantes. Conferência: 4 × 6 + 5 = 29.

## Exercício 3 — Pressão

Solução compilável: `exemplos/05_exercicio_pressao.c`.

```c
bool permitido = (pressao_bar >= 2.0) && (pressao_bar <= 4.0)
              && comando && !falha;
```

Com comando verdadeiro e falha falsa:

| Pressão | permitido |
|---|---:|
| 2,5 bar | 1 |
| 1,9 bar | 0 |
| 4,0 bar | 1 |

O limite superior é inclusivo. Se o comando for falso ou a falha for verdadeira, `permitido` será 0.

## Exercício 4 — Curto-circuito

Com `n = 0`, `(n != 0)` é falso. O operador `&&` determina o resultado falso sem avaliar `(10 / n > 2)`, evitando a divisão por zero. `&` é um operador bit a bit e não oferece curto-circuito: a divisão seria avaliada, causando comportamento indefinido. Não execute a versão inválida como experimento.

## Bilhete de saída

```c
bool aviso = (nivel_l < 50) || (temperatura_c > 40.0);
```

Para nível de exatamente 50 L e temperatura de exatamente 40,0 °C, ambas as comparações são falsas, logo `aviso` é falso. Basta uma das comparações ser verdadeira para o aviso ser verdadeiro.
