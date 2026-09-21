# Gabarito — 28/09/2026

## Prática guiada: lote de medições

Solução completa: [04_lote.c](exemplos/04_lote.c).

| Índice | Leitura | Soma acumulada | Leitura >= 25? |
| --- | --- | --- | --- |
| 0 | 18 | 18 | Não |
| 1 | 22 | 40 | Não |
| 2 | 22 | 62 | Não |
| 3 | 27 | 89 | Sim |
| 4 | 31 | 120 | Sim |
| 5 | 24 | 144 | Não |

Resultados: soma **144**, média **24.0**, mínimo **18**, máximo **31** e **2** leituras maiores ou iguais a 25. A busca de 22 retorna o índice **1**, porque pede a primeira ocorrência; 99 retorna ausência. O valor -1 é apenas um marcador de ausência, nunca um índice a acessar.

Na extensão com seis valores iguais a 22: soma 132, média 22.0, mínimo e máximo 22, contagem de valores >= 25 igual a zero, primeira ocorrência de 22 no índice 0 e alvo 99 ausente. Se todos forem 30, soma 180, média 30.0 e contagem 6; os alvos 22 e 99 estarão ausentes.

## Exercício 1 — Inicialização parcial e limites

`int v[4] = {3, 6};` produz `{3, 6, 0, 0}`. Índices válidos: 0, 1, 2 e 3. O laço correto é:

```c
for (int i = 0; i < 4; i++) {
    printf("%d\n", v[i]);
}
```

`i <= 4` acessaria `v[4]`, fora do vetor. Esse acesso tem comportamento indefinido, mesmo que pareça funcionar em uma execução.

## Exercício 2 — Soma por coluna

```c
int dados[2][3] = {{10, 20, 30}, {40, 50, 60}};
for (int coluna = 0; coluna < 3; coluna++) {
    int soma = 0;
    for (int linha = 0; linha < 2; linha++) {
        soma += dados[linha][coluna];
    }
    printf("Coluna %d: %d\n", coluna, soma);
}
```

Resultados: coluna 0 = **50**, coluna 1 = **70**, coluna 2 = **90**. O laço externo percorre colunas; o interno percorre as duas linhas da coluna atual. O acumulador precisa reiniciar para cada coluna.

## Exercício 3 — Espaço para o terminador

`PARAR` tem cinco caracteres ASCII. A capacidade mínima é **6** bytes, incluindo `\0`; `strlen("PARAR")` retorna **5** do tipo `size_t`, sem contar o terminador. Exemplo: `char comando[6] = "PARAR";`.

A string vazia também precisa de uma posição para `\0`. Capacidade e comprimento não são sinônimos.

## Exercício 4 — Comparação de conteúdo

O contrato de `strcmp`, declarado em `<string.h>`, retorna zero quando os textos são iguais. Um resultado negativo ou positivo informa a ordem lexicográfica; não há garantia de retorno exatamente -1 ou 1.

Em expressões comuns, vetores se convertem em ponteiros ao primeiro elemento. `a == b` compara esses endereços, não os caracteres guardados. Dois vetores distintos podem conter o mesmo texto sem ter o mesmo endereço. Use `strcmp(a, b) == 0` para igualdade textual. Ambos precisam ser strings válidas e terminadas por `\0`.

## Exercício 5 — Comando STATUS

No programa [03_comandos.c](exemplos/03_comandos.c), preserve toda a leitura e a validação. Acrescente o ramo abaixo antes do `else` de comando desconhecido:

```c
} else if (strcmp(comando, "STATUS") == 0) {
    printf("Saida inicial: %d\n", saida);
    return 0;
```

Como cada execução inicia `saida` em zero e recebe um comando, `STATUS` exibe `Saida inicial: 0`. O `return 0` impede a mensagem de acionamento após uma simples consulta. Para persistir estado entre vários comandos seria necessário repetir a leitura dentro de um laço, uma extensão diferente.

Verifique `STATUS`, `status`, linha vazia, texto de 15 caracteres e texto de 16 caracteres. Somente o primeiro é o novo comando válido; 15 caracteres cabem mas podem formar um comando desconhecido; 16 caracteres exigem rejeição por excesso.

## Raciocínio sobre fgets

Para `char comando[16]`, `fgets` reserva um byte para o terminador e lê no máximo 15 caracteres. `LIGAR\n` inicialmente tem comprimento 6; após substituir a quebra por `\0`, tem comprimento 5. A capacidade continua 16.

Sem quebra no buffer, o próximo caractere separa duas situações: quebra/EOF significa texto completo; outro caractere significa linha longa. O programa descarta o restante e encerra com retorno 1. O retorno de leitura é verificado antes de qualquer chamada a `strlen`.
