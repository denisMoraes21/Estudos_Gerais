- Ponteiros são referências para variáveis.
- int *countPtr; ponteiro para inteiro;
- Ponteiros devem ser inicializados quando forem definidos.
- Um ponteiro pode ser inicializado com NULL, 0 ou algum endereço.
- NULL não aponta pra nada e tem valor 0.
- Inicializar um ponteiro como 0 é equivalente a inicializar com NULL.
- Todo ponteiro deve ser inicializado.

- "&" é o endereço. Operação unária. Retorna o endereço desse operador.
- "*" é a indireção. Operação unária. Operador de dereferência.

int y = 5; # Alocado na memória 600000.
int *yPtr = &y; Tem valor de 600000, mas está alocado na memória 500000.

- Printar y e printar *yPtr é a mesma coisa.
- int *: declara um ponteiro para inteiro. Dessa mesma forma faz cast.

- Fazer referência a um ponteiro que não foi inicializado pode causar erro.
- Alocar um endereço de uma variável a um ponteiro pode causar erro.

- O ponteiro com o operador de dereferência fica com o valor da variável a que faz referência, mas o valor da variável é memória.

- Você pode passar referência como argumento de funções para evitar carregamento de objetos muito grandes.
- Você pode obter multiplos retornos de funções se usar ponteiros como retornos.
- Princípio do menor privilégio: só a função ou só o arquivo deve ter acesso às variàveis que são utilizadas por eles;
- 