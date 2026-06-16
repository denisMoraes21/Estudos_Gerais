//
// Created by denis on 07/04/25.
//

#include <iostream>

int main() {
  int number_1, number_2;
  char symbol;
  std::cin >> number_1 >> number_2 >> symbol;

  switch (symbol) {
    case '+':
      std::cout << "Soma: " << number_1 + number_2 << std::endl;
      break;
    case '-':
      std::cout << "Subtração: " << number_1 - number_2 << std::endl;
      break;
    case '*':
      std::cout << "Multiplicação: " << number_1 * number_2 << std::endl;
      break;
    case '/':
      std::cout << "Divisão: " << number_1 / number_2 << std::endl;
      break;
    default:
      std::cout << "Operation not possible" << std::endl;
      break;
  }
  return 0;
}