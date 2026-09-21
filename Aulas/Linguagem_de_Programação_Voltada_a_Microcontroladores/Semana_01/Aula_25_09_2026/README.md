# Aula 4 — Estruturas de controle de fluxo

**Data:** 25/09/2026 · **Semana:** 01  
**Curso:** Linguagem de Programação Voltada a Microcontroladores

Material no padrão Beamer FPFtech: [slides em PDF](main.pdf), [fonte principal](main.tex) e 27 páginas em `page_*.tex`, incluindo a capa. A aula cobre sequência, `if/else`, `switch`, `for`, `while`, `do/while`, `break`, `continue`, escopo, limites e laços contínuos em sistemas embarcados. Os exemplos simulam automação no terminal.

## Sequência sugerida

1. Recuperar variáveis e operadores; rastrear a sequência (slides 2–5).
2. Escolher caminhos e discutir limites (slides 6–11).
3. Rastrear laços, contadores, acumuladores e interrupções (slides 12–22).
4. Integrar, testar e praticar (slides 23–27).

Pré-requisitos: tipos primitivos, inicialização de variáveis, operadores aritméticos, relacionais e lógicos. Funções próprias serão ensinadas na próxima semana; os programas aqui usam somente `main` e funções da biblioteca padrão.

## Compilar os slides

Na pasta desta aula, com uma distribuição LaTeX que forneça Beamer, TikZ, listings e babel-portuguese:

```sh
mkdir -p /tmp/aula_25_09_2026_pdf
pdflatex -interaction=nonstopmode -halt-on-error -output-directory=/tmp/aula_25_09_2026_pdf main.tex
pdflatex -interaction=nonstopmode -halt-on-error -output-directory=/tmp/aula_25_09_2026_pdf main.tex
```

O PDF produzido ficará em `/tmp/aula_25_09_2026_pdf/main.pdf`.

## Compilar e executar os exemplos

Na pasta desta aula, com GCC e shell POSIX:

```sh
mkdir -p /tmp/aula_25_09_2026_bin
for fonte in exemplos/*.c; do
    nome=$(basename "$fonte" .c)
    gcc -std=c11 -Wall -Wextra -Wpedantic "$fonte" -o "/tmp/aula_25_09_2026_bin/$nome"
done
/tmp/aula_25_09_2026_bin/01_decisao
/tmp/aula_25_09_2026_bin/02_ciclos
printf '1\n2\n0\n' | /tmp/aula_25_09_2026_bin/03_menu
printf '29\n30\n79\n80\n-2\n101\n-1\n' | /tmp/aula_25_09_2026_bin/04_reservatorio
```

Resultados esperados:

- `01_decisao`: `Temperatura: 30 C` e `Ventilador: 1`.
- `02_ciclos`: cinco linhas para 26, 28, 30, 32 e 34 °C; estados 0, 0, 1, 1, 1; mensagem final `Ciclos ligado: 3`.
- `03_menu`: além dos menus, `Sensor: 28 C`, `Ciclo 1`, `Ciclo 2`, `Ciclo 3` e `Encerrando`.
- `04_reservatorio`: além dos pedidos de entrada, `Baixo`, `Adequado`, `Adequado`, `Alto`, duas mensagens `Nivel invalido` e `Encerrado`. O sentinela não recebe classificação.

Casos adicionais: opção 9 seguida de 0 deve produzir `Opcao invalida` e encerrar; entrada `x` ou fim de arquivo antes de um inteiro devem produzir `Entrada invalida` e retorno 1. Os exemplos com `scanf` são introdutórios e pressupõem inteiros pequenos representáveis por `int`; a validação completa de uma linha textual será desenvolvida posteriormente. Os exemplos não implementam temporização física nem acionam hardware.

## Atividades

A prática guiada é o classificador de reservatório; o arquivo `04_reservatorio.c` implementa a extensão com repetição. As respostas e os rastreios dos cinco exercícios estão em [gabarito.md](gabarito.md).
