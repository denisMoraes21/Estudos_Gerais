//
// Created by denis on 06/04/25.
//

#include <iostream>


int main() {

    int number;
    std::cout << "Enter a number: ";
    std::cin >> number;

    if (number < 0) {
        std::cout << number << " is a negative number";
    }
    if (number == 0) {
        std::cout << number << " is zero";
    }
    if (number > 0) {
        std::cout << number << " is an odd number";
    }

    return 0;
}