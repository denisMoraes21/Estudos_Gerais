# Gabarito — Aula 30/09/2026


- Prática: passos `(i, leitura, total após)` são `(0,28,0)`, `(1,30,1)` e `(2,32,2)`. O teste com vetor `{30}` cobre a igualdade.
- Exercício 1: falta de `;` → sintaxe; implementação ausente na ligação → ligação; `vetor[n]` em vetor de `n` elementos → acesso inválido com comportamento indefinido; `>` em regra inclusiva → lógica.
- Exercício 2: vetor `{29,30,31,30}` resulta em 3; `assert(contar_no_limite(fronteiras, 4, 30) == 3);`.
- Exercício 3: 32 satisfaz tanto `>` quanto `>=`; leitura 30 e limite 30 distinguem as versões, com resultado correto 1.
- Exercício 4: -41 rejeitado, -40 aceito, 125 aceito, 126 rejeitado.
- Desafio: acrescente o vetor e a asserção do exercício 2; a mensagem passa a dizer `7 testes de contagem passaram`.

Peça que o estudante registre esperado e hipótese antes de executar. Conclusão da prática: compilar sem avisos, observar o estado no laço e apresentar um caso que detecte o erro lógico. Passar na suíte não comprova a ausência de todos os defeitos; a validade dos acessos continua sendo parte do contrato e da revisão.
