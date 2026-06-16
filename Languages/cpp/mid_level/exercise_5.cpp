//
// Created by denis on 07/04/25.
//

#include <iostream>

std::string sentence = "ordem e progresso";

int main() {
  int words = 1;
  for (auto& j : sentence) {
    if (j == ' ') {
      words += 1;
    }
  }

  std::cout << words << std::endl;
  return 0;
}
