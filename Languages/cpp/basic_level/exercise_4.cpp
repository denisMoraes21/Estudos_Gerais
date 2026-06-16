//
// Created by denis on 06/04/25.
//

#include <iostream>

int main() {
    int numbers[10];
    for (auto& number : numbers) {
        std::cin >> number;
    }

    int total = 0;
    for (const int n : numbers) {
        total += n;
    }

    constexpr int size = std::size(numbers);
    const double avg = static_cast<double>(total) / size;
    std::cout << avg << std::endl;

    return 0;
}

// Static Cast: Conversão segura e explícita entre tipos em tempos de compiladção.
// Dynamic Cast: Conversão segura em tempo de execuação entre classes polimórficas (com herança e pelo menos um método virtual.
// Const Cast: Remove ou adiciona const ou volatile a um tipo.
// Reinterpret Cast: Conversão bruta e perigosa.