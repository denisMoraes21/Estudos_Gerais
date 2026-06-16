//
// Created by denis on 06/04/25.
//

#include <iostream>
#include <random>
#include <array>

int main() {

    std::array<int, 10> N{};
    for (int i = 0; i < 10; i++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 100);

        const int random_number = dist(gen);
        N[i] = random_number;
    }

    for (const auto& n : N) {
        std::cout << n << " ";
    }

    return 0;
}