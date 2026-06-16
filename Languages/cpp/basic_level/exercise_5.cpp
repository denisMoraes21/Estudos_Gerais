//
// Created by denis on 06/04/25.
//

#include <iostream>

int larger_one(const int number_1, const int number_2) {
    return number_1 > number_2 ? number_1 : number_2;
}

int factorial(const int number) {
    int fact = 1;
    for (int i  = 1; i <= number; i++) {
        fact *= i;
    }
    return fact;
}

int main() {

    // Larger One
    int number_1, number_2;
    std::cout << "Digite dois números: " << std::endl;
    std::cin >> number_1 >> number_2;
    std::cout << "O maior número dos dois é: " << larger_one(number_1, number_2) << std::endl;

    // Factorial
    std::cout << "O fatorial do primeiro número é: " << factorial(number_1) << std::endl;

    return 0;
}