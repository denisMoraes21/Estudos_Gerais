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

- Você pode passar referência como argumento de funções para evitar carregamento e cópia de objetos muito grandes, economiza espaço.
- Você pode obter multiplos retornos de funções se usar ponteiros como retornos.
- Princípio do menor privilégio: só a função ou só o arquivo deve ter acesso às variàveis que são utilizadas por eles;


Existem 4 formas de passr um ponteiro de dados para uma função:

- Um ponteiro não constante para um dado não constante: o dado pode ser 
modificado através de ponteiro de dereferência e o ponteiro pode ser modificado
para apontar para outros dados.

Exemplo: 
// fig07_06.c
// Converting a string to uppercase using a
// non-constant pointer to non-constant data.
#include <ctype.h>
#include <stdio.h>

void convertToUppercase(char *sPtr);                    // prototype
int main(void) {
    char string[] = "cHaRaCters and $32.98";            // initialize char array
    printf("The string before conversion is: %s\n", string);
    convertToUppercase(string);
    printf("The string after conversion is: %s\n", string); 
} 

// convert string to uppercase letters
void convertToUppercase(char *sPtr) {
    while (*sPtr != ) {                                 // current character is not
        *sPtr = toupper(*sPtr);                         // convert to uppercase
        ++sPtr;                                         // make sPtr point to the next character
   }
}

- Um ponteiro constante para um dado não constante: sempre aponta para uma mesma
locação de memória, mas o dado para essa locação pode ser modificado através 
do ponteiro.

Exemplo:
// fig07_09.c
// Attempting to modify a constant pointer to non-constant data.

#include <stdio.h>
int   main(void) {
    int x = 0; // define x
    int y = 0; // define y
    // ptr is a constant pointer to an integer that can be modified
    // through ptr, but ptr always points to the same memory location
    int * const ptr = &x;
    *ptr = 7; // allowed: *ptr is not const14   
    ptr = &y; // error: ptr is const; cannot assign new address
}

O valor do ponteiro pode ser modificado mas o ponteiro não pode receber um
novo ponteiro.

- Um ponteiro não constante para um dado constante: o dado pode ser modificado
para apontar para qualquer dado de um tipo apropriado, mas o dado para o qual
aponta não pode ser modificado (só leitura);

Exemplo:
// fig07_07.c
// Printing a string one character at a time using
// a non-constant pointer to constant data.

#include <stdio.h>

void printCharacters(const char *sPtr);
int   main(void) {
    // initialize char array
    char string[] = "print characters of a string";
    puts("The string is:");
    printCharacters(string);
    puts("");
}

// sPtr cannot be used to modify the character to which it points,
// i.e., sPtr is a "read-only" pointer
void printCharacters(const char *sPtr) {
    // loop through entire string
    for (; *sPtr != ; ++sPtr) { 
        // no initialization
        printf("%c", *sPtr);     
    }
}

O custo para fazer uma refência a uma estrutura grande é execution-time 
overhead. Aí passa pra função como const podendo somente leitura.

- Um ponteiro constante para um dado constante: forma correta de passar array 
para funções. Leva erro tentando modificar qualquer coisa;

Exemplo:
// fig07_10.c
// Attempting to modify a constant pointer to constant data.

#include <stdio.h>

int main(void) {
    int x = 5;
    int y = 0;
    // ptr is a constant pointer to a constant integer. ptr always
    // points to the same location; the integer at that location
    // cannot be modified
    const int *const ptr = &x; // initialization is OK
    printf("%d\n", *ptr);
    *ptr = 7; // error: *ptr is const; cannot assign new value
    ptr = &y; // error: ptr is const; cannot assign new address
}