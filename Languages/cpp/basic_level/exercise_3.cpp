//
// Created by denis on 06/04/25.
//

#include <iostream>

int main() {

    std::cout << "Show number from 1 to 100" << std::endl;
    std::string s;
    int sum = 0;
    for (int i = 1; i <= 100; i++) {
        s += std::to_string(i) + " - ";

        if (i % 2 == 0) {
            sum += i;
        }
    }
    std::cout << s << std::endl;
    std::cout << "\nSum of even numbers: " << sum << std::endl;
    return 0;
}
