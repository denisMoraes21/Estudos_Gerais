# Aula de 24/09/2026 — Operadores

Semana 01 de Linguagem de Programação Voltada a Microcontroladores.

Conteúdo: 3.1 operadores aritméticos; 3.2 operadores lógicos; 3.3 operadores relacionais. Inclui divisão inteira, resto, conversão antes da divisão, precedência, atribuição versus igualdade, curto-circuito e diferença entre `&&` e `&`.

## Materiais e condução

- `main.pdf`: apresentação pronta, com 25 slides.
- `main.tex`, `page_*.tex`, `ppgee_style.sty` e `images/`: fontes no padrão visual das outras aulas.
- `exemplos/`: cinco programas C11 independentes, com dados simulados.
- `gabarito.md`: soluções comentadas da atividade e dos exercícios.

Comece pela divisão de peças em caixas e peça previsões antes da execução. Faça a tabela-verdade coletivamente. Use a atividade de operação para transformar requisitos escritos em comparações e operadores lógicos. Discuta os limites inclusivos e o recálculo de uma expressão quando um dado muda. Condicionais e laços são conteúdos da próxima aula; os exemplos atuais apenas calculam e exibem resultados.

## Compilar e executar

Requer GCC com suporte a C11. Dentro desta pasta:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/01_aritmetica.c -o /tmp/aritmetica
/tmp/aritmetica
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/02_logica.c -o /tmp/logica
/tmp/logica
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/03_operacao.c -o /tmp/operacao
/tmp/operacao
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/04_exercicio_embalagem.c -o /tmp/exercicio_embalagem
/tmp/exercicio_embalagem
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/05_exercicio_pressao.c -o /tmp/exercicio_pressao
/tmp/exercicio_pressao
```

Saída de `01_aritmetica.c`:

```text
Caixas completas: 3
Pecas restantes: 2
Divisao inteira em double: 3.0
Divisao real: 3.4
Sem parenteses: 14
Com parenteses: 20
```

Saída de `02_logica.c`:

```text
Media alta: 0
Temperatura na faixa: 1
E logico: 1
E bit a bit: 0
```

Saída de `03_operacao.c`:

```text
Nivel adequado: 1
Temperatura na faixa: 1
Habilitado: 1
Com 99 L: nivel=0, faixa=1, habilitado=0
```

Saída de `04_exercicio_embalagem.c`:

```text
Caixas completas: 4
Pecas restantes: 5
```

Saída de `05_exercicio_pressao.c`:

```text
Pressao 2.5 bar: permitido=1
Pressao 1.9 bar: permitido=0
Pressao 4.0 bar: permitido=1
```

## Gerar a apresentação

Requer LaTeX com Beamer, TikZ, listings e babel para português. Execute duas vezes dentro desta pasta:

```sh
pdflatex -interaction=nonstopmode -halt-on-error main.tex
pdflatex -interaction=nonstopmode -halt-on-error main.tex
```

## Critérios de verificação

Os programas devem compilar sem avisos com as opções indicadas. Compare previsões com saídas, confira a avaliação nos limites e explique por que o exemplo com divisor zero não efetua a divisão. O resultado `habilitado` é um modelo didático calculado no computador; não é implementação de proteção ou controle de equipamento real.

Validação realizada: os cinco programas foram compilados com GCC em C11, incluindo `-Wall -Wextra -Wpedantic -Werror`, e executados com sucesso. Todas as saídas coincidem com as apresentadas acima. O PDF foi compilado em duas passagens, tem 25 páginas e não apresenta caixas excedendo a área de composição (`Overfull`).
