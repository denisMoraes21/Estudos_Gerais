//
// Created by denis on 07/04/25.
//

#include <iostream>
#include <array>

std::array<int, 5> M = {1, 7, 9, 2, 0};

int main() {
  bool change;
  int n = M.size();

  do {
    change = false;
    for (int i = 0; i < n - 1; i++) {
      if (M[i] > M[i + 1]) {
        const int temp = M[i];
        M[i] = M[i + 1];
        M[i + 1] = temp;
        change = true;
      }
    }
    n--;
  } while (change);

  for (const auto& j : M) {
    std::cout << j << " ";
  }

  return 0;
}
