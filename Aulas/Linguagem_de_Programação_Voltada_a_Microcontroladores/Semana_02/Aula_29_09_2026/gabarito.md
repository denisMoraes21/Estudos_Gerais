# Gabarito — Aula 29/09/2026


1. Com limite 31 e leituras `{28,30,32,31}`, o monitor imprime `28 C: normal`, `30 C: normal`, `32 C: ALARME` e `31 C: ALARME`. A regra fica em `alarme.c`; a declaração fica em `alarme.h`; o cenário e a impressão ficam em `main.c`.
2. Exercício 1: `return v >= min && v <= max;`, com pré-condição `min <= max`. Incluir `<stdbool.h>` para usar `bool`. O desafio modular segue as mesmas guardas e organização do módulo de alarme.
3. Exercício 2: resultado 31. O chamador calcula `sizeof leituras / sizeof leituras[0]` onde o vetor está declarado. Na função, o parâmetro é ponteiro e não informa o tamanho original.
4. Exercício 3: `S(4)=4+3+2+1+0=10`; cinco chamadas, incluindo `S(0)`.
5. Exercício 4: a função recebe uma cópia do inteiro ou uma cópia do ponteiro. O ponteiro copiado permite acessar o objeto original; C continua usando passagem por valor.

Sugestão de condução: pedir previsão da saída antes de rodar; depois solicitar a justificativa da igualdade no limiar e da quantidade de chamadas na recursão. Critério de conclusão: compilar sem avisos, obter saídas previstas e explicar os contratos.
