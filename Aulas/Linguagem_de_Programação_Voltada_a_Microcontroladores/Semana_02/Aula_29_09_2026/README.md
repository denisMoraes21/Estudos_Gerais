# Aula 29/09/2026 — Funções e modularização

Semana 02 de Linguagem de Programação Voltada a Microcontroladores.

## Conteúdo e uso

28 slides: capa, percurso, parâmetros e retorno, passagem por valor, ponteiros, contratos, funções puras, escopo, vetores com tamanho, módulos `.h`/`.c`, recursividade limitada, prática e exercícios com conferência. Os exemplos rodam no computador e não acessam GPIO. Pré-requisitos: C11, variáveis, decisões, laços e vetores.

Use `main.pdf` em sala; `main.tex`, `page_*.tex`, `ppgee_style.sty` e `images/` permitem editar a apresentação. Para recompilar na pasta da aula:

```sh
pdflatex -interaction=nonstopmode -halt-on-error main.tex
pdflatex -interaction=nonstopmode -halt-on-error main.tex
```

## Exemplos executáveis

Na pasta desta aula, em um terminal com GCC:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/parametros.c -o /tmp/aula29_parametros
/tmp/aula29_parametros
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/vetor.c -o /tmp/aula29_vetor
/tmp/aula29_vetor
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/recursao.c -o /tmp/aula29_recursao
/tmp/aula29_recursao
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/modular/main.c exemplos/modular/alarme.c -o /tmp/aula29_monitor
/tmp/aula29_monitor
```

Saídas esperadas, na mesma ordem:

```text
Local: 11
Original: 10
Depois do ponteiro: 11
```

```text
Maior leitura: 31 C
30 em [28, 30]: sim
```

```text
n=0: recursiva=0, iterativa=0
n=4: recursiva=10, iterativa=10
n=5: recursiva=15, iterativa=15
n=20: recursiva=210, iterativa=210
```

```text
28 C: normal
30 C: ALARME
32 C: ALARME
```

A implementação recursiva é didática e exige `n <= 20`; não use profundidade ilimitada em firmware. `maior_leitura` exige vetor não vazio e tamanho válido. O incremento por ponteiro exige objeto válido, modificável e valor menor que `INT_MAX`; o caso `NULL` não faz alteração.

## Prática e exercícios

Os slides contêm prática guiada, exercícios e conferência. Consulte o [gabarito](gabarito.md) para respostas, resultados esperados e orientações de condução.
