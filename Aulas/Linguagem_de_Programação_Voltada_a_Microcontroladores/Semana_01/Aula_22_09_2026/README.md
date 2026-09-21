# Conceitos de lógica de programação — 22/09/2026

Semana 01. Conteúdo: 1.1 algoritmos e pseudocódigos; 1.2 fluxogramas;
1.3 noções de paradigmas de programação. Apresentação em `main.pdf`,
editável em `main.tex` e `page_*.tex` (26 slides incluindo a capa).

## Condução sugerida

Apresentar o problema da estufa, formalizar a regra, executar testes de mesa
e construir o fluxograma com a turma. Comparar paradigmas apenas de forma
conceitual; a sintaxe de C será desenvolvida nos próximos encontros.
Reservar a parte final para a atividade de nível e os exercícios.
Não se pressupõe conhecimento prévio de programação nem placa física.

## Exemplo executável

Na pasta da aula, com GCC instalado:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic exemplos/sequencia.c -o /tmp/sequencia
/tmp/sequencia
```

Saída:

```text
Inicio do ciclo de leitura
Fim do ciclo de leitura
```

Os algoritmos de temperatura e nível são pseudocódigos para teste de mesa,
não controlam equipamentos. O exemplo C introduz apenas a ordem das instruções.

## Material do professor

`gabarito.md` contém as respostas da atividade e dos quatro exercícios.
O fluxograma deve ser avaliado pelo fluxo e pelas condições, não pela posição
dos símbolos no papel. O logo foi reutilizado das aulas existentes da FPFtech.

## Recompilar os slides

```sh
pdflatex -interaction=nonstopmode -halt-on-error main.tex
pdflatex -interaction=nonstopmode -halt-on-error main.tex
```
