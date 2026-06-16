# Estrutura de dados

- Alocação estática: já se sabe o tamanho antes da compilação.
- Alocação dinâmica: alocação durante execução.
- Ponteiro aponta para endereço
- "*" : operador de dereferenciação, guarda endereço quando algo é atribuído, mas quando é usando mostra valor. Usar & para guardar no ponteiro.
- Usar malloc para alocar uma certa quantidade de memória para um ponteiro. O endereço ainda é só um, mas a memória toda já está reservada.
- Alocação contígua, um ao lado do outro. Não precisa usar array explicito para usar estrutura igual. Pode usar malloc. Alocar e desalocar. Vantagem, o malloc consegue declarar um array dinâmico. Um array já se sabe o tamanho antes da compilação.
- Indireção multipla (pointer to pointer): "**prt". Descobrir quando isso é útil. O endereço é o valor?
- Ponteiros e matrizes multidimenssionais: Ponteiros de ponteiros podem ser acessados como array. Abstração, matrizes que mudam de tamanho em tempo de execução; realoc é perigoso, dobra a memória; verificar melhor realoc
- Recursividade. Existe um algotimo que consegue converter qualquer algortimo recursivo em algoritmo interativo. Back tracking;
- Listas encadeadas: não usa memória contígua. Último ponteiro, null, fim da lista. Também tem tamanho dinâmico. Desvantagem: não tem como acessar qualquer elemento de forma direta. Utilização de chamada recursiva para autoreferência.
- Listas encadeadas empurram pra frente os valores. Parece pilha, mas você pode apagar qualquer elemento.
- Árvores binárias: cada nó aponta para dois filhos. Cada filho pode ser visto como uma raiz de uma outra árvore. Natureza recursiva. Impressão pré fixada, pós fixada. Enxerto e poda. Usando quando se quer fazer busca mais rápida. Busca de arquivo, organização de cronogramas.
- Busca binária: busca sequencial. Divide-se ao meio e se faz a análise. Muito rápida.
- Árvode binária de busca: valor da raíza é sempre maior que qualquer nó do lado esquerdo e sempre menor que qualquer nó do lado direito.
- Árvore genérica: vários filhos. Filhos pré fixados com com array. Mas pode ser dinâmica onde cada filho é uma lista encadeada.
