//
// Created by denis on 07/04/25.
//

#include <iostream>

int fibonacci(const int number) {
  if (number == 0) return 0;
  if (number == 1) return 1;
  return fibonacci(number - 1) + fibonacci(number - 2);
}

bool isInFibonacci(const int number, const int i = 0) {
  int fib = fibonacci(i);
  if (fib == number) return true;
  if (fib > number) return false;
  return isInFibonacci(number, i + 1);
}

int main() {

  int number;
  std::cout << "Digite um número: ";
  std::cin >> number;

  std::cout << isInFibonacci(number) << std::endl;



  return 0;
}
