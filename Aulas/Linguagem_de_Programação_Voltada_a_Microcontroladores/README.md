# Linguagem de Programação Voltada a Microcontroladores

Material dos oito encontros de **22/09 a 01/10/2026**, organizado nas duas
semanas solicitadas. As apresentações seguem o padrão das aulas existentes:
Beamer, tema Montpellier em verde, identificação da FPFtech, professor
Denis Moraes Guimarães, capa datada e arquivos separados por slide.

## Cronograma e apresentações

| Semana | Data | Aula e conteúdo | Slides |
| --- | --- | --- | ---: |
| 01 | 22/09/2026 | [1. Conceitos de lógica de programação](Semana_01/Aula_22_09_2026/main.pdf): 1.1 algoritmos e pseudocódigos; 1.2 fluxogramas; 1.3 paradigmas. | 26 |
| 01 | 23/09/2026 | [2. Tipos de dados e variáveis](Semana_01/Aula_23_09_2026/main.pdf): 2.1 tipos primitivos; 2.2 declaração e uso de variáveis. | 25 |
| 01 | 24/09/2026 | [3. Operadores](Semana_01/Aula_24_09_2026/main.pdf): 3.1 aritméticos; 3.2 lógicos; 3.3 relacionais. | 25 |
| 01 | 25/09/2026 | [4. Estruturas de controle de fluxo](Semana_01/Aula_25_09_2026/main.pdf): 4.1 sequência; 4.2 condicionais; 4.3 repetição. | 27 |
| 02 | 28/09/2026 | [5. Estruturas de dados](Semana_02/Aula_28_09_2026/main.pdf): 5.1 vetores e matrizes; 5.2 operações básicas; 5.3 strings. | 28 |
| 02 | 29/09/2026 | [6. Funções e modularização](Semana_02/Aula_29_09_2026/main.pdf): 6.1 parâmetros e retorno; 6.2 organização modular; 6.3 recursividade. | 28 |
| 02 | 30/09/2026 | [7. Depuração e testes](Semana_02/Aula_30_09_2026/main.pdf): 7.1 técnicas de depuração; 7.2 identificação e correção de erros; 7.3 testes básicos. | 28 |
| 02 | 01/10/2026 | [8. Noções de programação para microcontroladores](Semana_02/Aula_01_10_2026/main.pdf): 8.1 microcontroladores e automação; 8.2 GPIO; 8.3 aplicações simples. | 30 |

São **217 slides**, incluindo as oito capas. Cada encontro inclui objetivos,
explicações, exemplos, atividades e síntese. Os gabaritos acompanham o material
em arquivos separados para consulta do professor.

## Organização de cada aula

```text
Semana_XX/Aula_DD_MM_AAAA/
├── main.pdf          apresentação para uso em sala
├── main.tex          fonte principal em LaTeX
├── page_01.tex       identificação da capa
├── page_02.tex ...   conteúdo, um slide por arquivo
├── ppgee_style.sty   tema e configuração dos códigos
├── images/          logo reutilizado das aulas existentes
├── exemplos/        programas completos ou sketches Arduino
├── README.md        condução, comandos e resultados esperados
└── gabarito.md       respostas das atividades e exercícios
```

Os diagramas e fluxogramas são construídos em TikZ e ficam incorporados nos
PDFs. Não é preciso acessar a internet para apresentar os slides.

## Progressão e ambiente de prática

Os encontros 1 a 7 usam **C11 no computador**, com situações simuladas de
automação: temperatura, reservatórios, contagem de peças, classificação de
leituras e comandos de texto. É possível trabalhar esses conteúdos sem placa.
GCC é usado para compilar; a aula de depuração inclui também GDB.

O encontro 8 usa **Arduino Uno R3**, dois botões e o LED integrado. Os arquivos
`.ino` são compilados como C++ no ambiente Arduino, diferença explicada nos
slides. A prática começa com piscar o LED, passa pela leitura de um botão e
termina em uma sinalização condicionada a duas entradas. A temporização com
`millis` aparece como extensão. Montagem, tabela de testes e referências
oficiais estão no [guia de 01/10](Semana_02/Aula_01_10_2026/README.md).

Os READMEs das aulas trazem comandos de compilação e saídas esperadas;
projetos com vários arquivos devem usar o comando de ligação indicado.
As soluções dos exercícios devem ser disponibilizadas após a tentativa dos
estudantes. As sugestões de condução não pressupõem uma duração fixa de aula.

## Recompilar os oito PDFs

Requer Python 3 e uma distribuição LaTeX com `pdflatex`, Beamer, TikZ,
listings, fontes T1 e babel para português. Na pasta deste curso:

```sh
python3 compilar.py
```

O script compila cada apresentação duas vezes, atualiza `main.pdf` e guarda
os auxiliares apenas em uma pasta temporária. Interrompe se houver erro ou
conteúdo excedendo a página. Para editar e compilar uma aula individualmente,
use os comandos do README correspondente.

## Verificações do material

- PDFs compilados em duas passagens e verificados quanto a conteúdo excedendo
  a área dos slides; revisão visual de capas, diagramas, tabelas e códigos.
- Exemplos C11 compilados com `-Wall -Wextra -Wpedantic -Werror`, com conferência
  de saídas e casos de limite nas atividades que recebem entradas.
- Aula de testes: suíte com 12 verificações de contagem e validação, além de
  roteiro GDB executável.
- Quatro sketches Arduino exercitados em uma simulação da API no computador:
  `python3 Semana_02/Aula_01_10_2026/validacao/testar.py`.

A validação dos sketches no computador verifica a lógica e as transições.
A compilação para AVR, o envio e a execução na placa devem ser feitos no
Arduino IDE e na bancada; não foram realizados neste ambiente.
