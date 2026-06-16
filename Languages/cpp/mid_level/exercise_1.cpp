//
// Created by denis on 07/04/25.
//

#include <iostream>
#include <array>

std::array<int, 10> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main() {
  std::cout << "Original vector: ";
  for (const auto& i : arr) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  for (int i = 0; i < static_cast<int>(arr.size() / 2); i++) {
    const int x = arr[i];
    const int y = arr[arr.size() - 1 - i];
    arr[i] = y;
    arr[arr.size() - 1 - i] = x;
  }

  std::cout << "Reverse vector: ";
  for (const auto& i : arr) {
    std::cout << i << " ";
  }

  return 0;
}