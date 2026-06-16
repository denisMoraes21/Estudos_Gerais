//
// Created by denis on 07/04/25.
//

#include <iostream>

int main() {
  std::string input;
  std::cout << "Word: ";
  std::cin >> input;

  for (size_t i = 0; i < input.length()/2; i++) {
    if (input[i] != input[input.length() - 1 - i]) {
      std::cout << "Não é palíndromo." << std::endl;
      return 0;
    }
  }
  std::cout << "Palíndromo" << std::endl;
  return 0;
}