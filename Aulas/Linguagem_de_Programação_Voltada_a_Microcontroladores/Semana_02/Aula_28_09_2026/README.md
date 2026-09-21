# Aula 5 — Estruturas de dados

**Data:** 28/09/2026 · **Semana:** 02  
**Curso:** Linguagem de Programação Voltada a Microcontroladores

Material no padrão Beamer FPFtech: [slides em PDF](main.pdf), [fonte principal](main.tex) e 28 páginas em `page_*.tex`, incluindo a capa. Conteúdo: vetores, matrizes, acesso e limites, inicialização, atualização, soma, média, mínimo, máximo, busca linear, capacidade versus quantidade utilizada e strings C com `strlen`, `strcmp` e `fgets`.

## Sequência sugerida

1. Representar coleções e percorrer vetores (slides 2–8).
2. Agregar, buscar e administrar a quantidade utilizada (slides 9–13).
3. Organizar matrizes e usar laços aninhados (slides 14–17).
4. Representar e receber texto com limites (slides 18–25).
5. Realizar a prática, discutir exercícios e sintetizar (slides 26–28).

Pré-requisitos: tipos, variáveis, operadores e estruturas de controle. Os exemplos usam biblioteca padrão e somente a função `main`; criação de funções próprias será trabalhada na aula seguinte.

## Compilar os slides

Na pasta desta aula, com Beamer, TikZ, listings e babel-portuguese disponíveis:

```sh
mkdir -p /tmp/aula_28_09_2026_pdf
pdflatex -interaction=nonstopmode -halt-on-error -output-directory=/tmp/aula_28_09_2026_pdf main.tex
pdflatex -interaction=nonstopmode -halt-on-error -output-directory=/tmp/aula_28_09_2026_pdf main.tex
```

O PDF produzido ficará em `/tmp/aula_28_09_2026_pdf/main.pdf`.

## Compilar e executar os exemplos

Na pasta desta aula, com GCC e shell POSIX:

```sh
mkdir -p /tmp/aula_28_09_2026_bin
for fonte in exemplos/*.c; do
    nome=$(basename "$fonte" .c)
    gcc -std=c11 -Wall -Wextra -Wpedantic "$fonte" -o "/tmp/aula_28_09_2026_bin/$nome"
done
/tmp/aula_28_09_2026_bin/01_vetores
/tmp/aula_28_09_2026_bin/02_matrizes
printf 'LIGAR\n' | /tmp/aula_28_09_2026_bin/03_comandos
printf 'DESLIGAR\n' | /tmp/aula_28_09_2026_bin/03_comandos
/tmp/aula_28_09_2026_bin/04_lote
```

Resultados esperados:

- `01_vetores`: leituras 26, 28, 30, 32 e 34; soma 150; média 30.0; mínimo 26; máximo 34; alvo 30 no índice 2.
- `02_matrizes`: linhas `24 25 26` e `28 29 30`, com médias 25.0 e 29.0.
- `03_comandos`: após o pedido, `Saida simulada: 1` para `LIGAR` e `Saida simulada: 0` para `DESLIGAR`.
- `04_lote`: soma 144; média 24.0; mínimo 18; máximo 31; duas leituras >= 25; primeira ocorrência de 22 no índice 1; 99 ausente.

A pontuação decimal do programa é um ponto, conforme a configuração padrão de C.

## Casos de entrada para strings

Cada execução recebe uma única linha textual. O buffer comporta 15 caracteres ASCII e o terminador; a comparação diferencia maiúsculas, minúsculas e espaços.

| Entrada | Saída relevante | Retorno |
| --- | --- | --- |
| `LIGAR` com ou sem quebra final | `Saida simulada: 1` | 0 |
| `DESLIGAR` | `Saida simulada: 0` | 0 |
| `ligar`, `STATUS` ou linha vazia | `Comando desconhecido` | 1 |
| Exatamente 15 caracteres não reconhecidos | `Comando desconhecido` | 1 |
| Mais de 15 caracteres | `Comando muito longo` | 1 |
| Entrada vazia, sem linha (EOF) | `Nenhuma linha recebida` | 1 |

Quando falta a quebra de linha no buffer, o programa consulta o próximo caractere. Uma quebra ou EOF indica que o texto completo coube; qualquer outro caractere indica truncamento, então o restante é descartado e o comando rejeitado. Não há cópia insegura de strings. O exemplo é destinado a linhas de texto do terminal, sem bytes nulos embutidos; não é um parser de dados binários.

A prática está resolvida em `04_lote.c`, e os exercícios adicionais em [gabarito.md](gabarito.md).
