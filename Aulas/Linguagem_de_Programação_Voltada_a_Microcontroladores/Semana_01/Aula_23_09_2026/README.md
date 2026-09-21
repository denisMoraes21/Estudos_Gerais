# Aula de 23/09/2026 — Tipos de dados e variáveis

Semana 01 de Linguagem de Programação Voltada a Microcontroladores.

Conteúdo: 2.1 tipos primitivos de dados; 2.2 declaração e uso de variáveis. A aula retoma algoritmos e prepara o uso de operadores. Os exemplos usam C11 no computador, com dados simulados de automação e sem acesso a hardware.

## Materiais e condução

- `main.pdf`: apresentação pronta, com 25 slides.
- `main.tex`, `page_*.tex`, `ppgee_style.sty` e `images/`: fontes da apresentação no padrão visual das outras aulas.
- `exemplos/`: quatro programas completos e independentes.
- `gabarito.md`: respostas comentadas da atividade e dos exercícios.

Comece pedindo aos estudantes que diferenciem contagem, medição, caractere e estado. Faça o rastreamento das atribuições antes de executar o painel. Compare os limites informados pelo compilador com a ideia de que diferentes plataformas podem usar representações diferentes. Conclua com a atividade do reservatório e a entrega individual. O gabarito deve ser consultado após a tentativa.

## Compilar e executar os exemplos

Requer GCC com suporte a C11. Dentro desta pasta:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/01_painel.c -o /tmp/painel
/tmp/painel
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/02_ambiente.c -o /tmp/ambiente
/tmp/ambiente
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/03_reservatorio.c -o /tmp/reservatorio
/tmp/reservatorio
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/04_exercicio_painel.c -o /tmp/exercicio_painel
/tmp/exercicio_painel
```

Saída de `01_painel.c`:

```text
Ciclos: 42
Temperatura: 27.5 C
Setor: A
Disponivel: 1
```

`02_ambiente.c` informa `CHAR_BIT`, `sizeof(int)`, `INT_MIN`, `INT_MAX` e `sizeof(double)`. Os números dependem da implementação; não há uma saída numérica universal exigida. `sizeof` mede bytes de C e `CHAR_BIT` informa quantos bits há em cada byte.

Saída de `03_reservatorio.c`:

```text
Nivel atual: 210 L
Nivel anterior: 180 L
Temperatura: 24.5 C
Setor: B
Alarme: 0
```

Saída de `04_exercicio_painel.c` (solução do exercício 3):

```text
Pecas: 120
Tensao: 3.3 V
Setor: C
Pronto: 1
```

## Gerar a apresentação

Requer LaTeX com Beamer, TikZ, listings e babel para português. Execute duas vezes, nesta pasta:

```sh
pdflatex -interaction=nonstopmode -halt-on-error main.tex
pdflatex -interaction=nonstopmode -halt-on-error main.tex
```

## Critérios de verificação

Os exemplos devem compilar sem avisos com as opções indicadas. Confira os valores previstos, a coerência dos formatos de `printf`, o uso de aspas simples para caracteres e a inicialização antes de qualquer leitura. Na avaliação, aceite outros nomes e tipos que representem corretamente a informação e cuja escolha seja justificada.

Validação realizada: os quatro programas foram compilados com GCC em C11, incluindo `-Wall -Wextra -Wpedantic -Werror`, e executados com sucesso. As saídas fixas coincidem com as apresentadas acima; o exemplo de ambiente foi conferido considerando a dependência da implementação. O PDF foi compilado em duas passagens, tem 25 páginas e não apresenta caixas excedendo a área de composição (`Overfull`).
