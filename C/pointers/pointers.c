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


int cubeIntByValue(int value);
int cubeIntByPointer(int *pointer);

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

    return 0;
}

int cubeIntByValue(int value) {
    return value * value * value;
}

int cubeIntByPointer(int *pointer) {
    return *pointer * *pointer * *pointer;
}
