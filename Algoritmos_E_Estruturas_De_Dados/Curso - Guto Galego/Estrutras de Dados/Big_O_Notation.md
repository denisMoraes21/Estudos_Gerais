# Estruturas de dados

Obs: Big Threes não serão abordadas.

## Big O Notation

- Algoritmos mais eficientes.
- Desempenho de algoritmo mas não performance. Como o algoritmo escala conforme o tamanho do input.
  Ex: Analise assintótica. Não dá pra dizer que O(n) é mais rápido que O(n²).
- Casos pequenos quase não faz diferença Big O.
- Complexidade temporal (runtime) e complexidade espacial (memória alocada).
- Ex: O(n) percorreu cada elemento uma vez em um array. Alocar uma só memória para atividade O(1).
- Considerar pior caso.

### O(1)

* Tempo constante: olhar para o primeiro elemento de um vetor, independente do taamanho da busca.
* Memória constante: encontrar o maior elemento de um array, indepedente do tamanho da busca.

### O(Log N)

* Conforme o imput aumenta muito rápido (exponencialmente), o tempo não cresce tão rápido (linearmente)
* Binary search: divide a partir do meio do array para busca; do meio para trás; dobrou o tamanho do input, mas a quantidade de buscas ficou em uma unidade a mais apenas.

### O(N)

* Buscar em sequência em um array, analisar um de cada vez;
* Se escalar o tamanho, input aumenta também;

### O(N Log N)

* A maioria Sort, a não ser o Buble Sort (O (N²));
* Difícil de aparecer, algoritmo mal feito, dividir e conquistar. Ordenações;

### O(N²)

* Loop dentro de loop.
* for i in arr:

  * for j in arr:
* casda

### O(2^N)

### O(sqrt(N))

### O(N!)
