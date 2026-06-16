//
// Created by denis on 06/04/25.
//

#include <iostream>
#include <fstream>

int main() {
    if (std::ofstream file("file.txt", std::ios::app); file.is_open()) {
        std::string name;
        std::cout << "Digite um nome: ";
        std::cin >> name;
        file << name + "\n";
        file.close();
    }
    else {
        std::cerr << "Erro ao abrir arquivo." << std::endl;
    }

    if (std::ifstream file_("file.txt"); file_) {
        std::string line;
        while (std::getline(file_, line)) {
            std::cout << line << std::endl;
        }
        file_.close();
    }
    else {
        std::cerr << "Erro ao abrir arquivo." << std::endl;
    }

    return 0;
}