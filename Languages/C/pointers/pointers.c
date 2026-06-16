/**
 * @file    pointers.c
 * @brief   Exemplos de uso de ponteiros e passagem de parâmetros em C.
 *
 * Este arquivo demonstra:
 *  - Passagem de parâmetros por valor
 *  - Passagem de parâmetros por ponteiro
 *  - Boas práticas no uso de ponteiros
 *  - Exercícios e exemplos sobre ponteiros
 *
 * @author  Denis Moraes Guimarães
 * @date    2026-01-22
 * @version 1.0
 *
 * @note    Compilar com -Wall -Wextra para melhor detecção de erros.
 */

 
#include <stdio.h>
#include <ctype.h>


int cubeIntByValue(int value);
int cubeIntByPointer(int *pointer);
void convertToUppercase(char *sPtr);
void printCharacters(const char *sPtr);
void f(void);

int main(void) {
    int y = 5;
    int *yPtr = &y;
    int **yPtrPtr = &yPtr;
    printf("O valor de y é: %d\n", y);

    printf("O valor de *yPtr é: %d\n", *yPtr);
    printf("O valor de yPtr é: %p\n", yPtr);
    
    printf("O valor de **yPtr é: %d\n", **yPtrPtr);
    printf("O valor de yPtrPtr é: %p\n", yPtrPtr);

    printf("O valor de &y é: %p\n", (void *)&y);
    printf("O valor de &yPtr é : %p\n", (void *)&yPtr);
    printf("O valor de &yPtrPtr é: %p\n", (void *)&yPtrPtr);

    int value;
    // int *valuePtr; --> Nunca declare um ponteiro sem inicializá-lo
    printf("Digite um número: ");
    scanf("%d", &value);
    int *valuePtr = &value;

    printf("O cubo passado por valor é: %d\n", cubeIntByValue(value));
    printf("O cubo passado por ponteiro é: %d\n", cubeIntByPointer(&value));
    printf("O cubo passado por ponteiro é: %d\n", cubeIntByPointer(valuePtr));


    char word[] = "Hello World!";

    printf("Ponteiro não constante para um dado não constante\n");
    printf("%s\n", word);
    convertToUppercase(word);
    printf("%s\n", word);

    printf("Ponteiro não constante para um dado constante\n");
    printf("%s\n", word);
    printCharacters(word);
    printf("%s\n", word);

    f();

    return 0;
}

int cubeIntByValue(int value) {
    return value * value * value;
}

int cubeIntByPointer(int *pointer) {
    return *pointer * *pointer * *pointer;
}

// Liberdade total
// Ponteiro não constante para um dado não constante
void convertToUppercase(char *sPtr) {
    while (*sPtr != '\0') {
        *sPtr = toupper(*sPtr);
        ++sPtr;
    }
}

// Liberdade restrita
// Ponteiro não constante para um dado constante
void printCharacters(const char *sPtr) {
    for (; *sPtr != '\0'; ++sPtr) {
        printf("%c", *sPtr);
    }
    printf("\n");

    // *sPtr = "teste"; // --> Não consegue modificar um dado const
    // Sempre tem o mesmo valor
}

// Liberdade restrita
// Ponteiro constante para um dado não constante
void f(void) {
    int x = 0;
    int y = 0;

    int * const ptr = &x;

    *ptr = 7;
    // ptr = &y; // --> Não consegue modificar um ponteiro const
    // Sempre aponta para o mesmo local
}

// Ponteiro constante para um dado constante
void g(void) {
    int x = 0;
    int y = 0;

    const int * const ptr = &x;

    printf("%d\n", *ptr);
    // *ptr = 7; // --> Valor constante
    // ptr = &y; // --> Ponteiro constante
}