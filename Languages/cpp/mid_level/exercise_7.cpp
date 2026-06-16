//
// Created by denis on 07/04/25.
//

#include <iostream>

int main() {
  constexpr int a = 10;
  constexpr int b = 20;
  const int* ptr1 = &a;
  const int* ptr2 = &b;
  const int soma = *ptr1 + *ptr2;
  std::cout << soma << std::endl;

  int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int* inicio = v;
  int tamanho = sizeof(v)/sizeof(int);
  std::cout << *inicio << std::endl;
  std::cout << tamanho << std::endl;
  std::cout << v << std::endl;
  std::cout << v + tamanho << std::endl;

  int soma2 = 0;
  while (inicio < v + tamanho) {
    soma2 += *inicio;
    inicio++;
  }
  std::cout << soma2 << std::endl;
  return 0;
}