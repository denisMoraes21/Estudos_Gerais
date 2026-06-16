//
// Created by denis on 07/04/25.
//
#include <iostream>
#include <string>

std::string invert(const std::string& str) {
  if (str.empty()) {
    return "";
  }
  return str.back() + invert(str.substr(0, str.size() - 1));
}

int main() {
  std::string texto;
  std::cout << "Digite uma string: ";
  std::cin >> texto;

  std::string invertida = invert(texto);
  std::cout << "String invertida: " << invertida << std::endl;

  return 0;
}
