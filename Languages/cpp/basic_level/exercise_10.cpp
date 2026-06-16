//
// Created by denis on 06/04/25.
//

#include <iostream>

int main() {
    std::string text;
    std::cout << "Digite uma frase: ";

    // Esse comando lê uma frase com espaços
    std::getline(std::cin, text);

    // Esse comando lê apenas uma palavra;
    //std::cin >> text;

    // Solução força bruta
    // int count = 0;

    // for (const auto c : text) {
    //     if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
    //         c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
    //         ++count;
    //     }
    // }

    const int count = std::count_if(text.begin(), text.end(), [](char c) {
        c = std::tolower(static_cast<unsigned char>(c));
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    });

    std::cout << "A frase tem " << count << " vogais." << std::endl;
    return 0;
}