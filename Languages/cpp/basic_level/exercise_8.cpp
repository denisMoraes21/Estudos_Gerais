//
// Created by denis on 06/04/25.
//

#include <iostream>
#include <vector>

std::vector<std::vector<int>> M = {
   {1, 2, 3},
{4, 5, 6},
    {7, 8, 9}
};


int main() {
    auto N  = M;
    for (int i = 0; i < 3; i++) {
        for (int j = 0;  j < 3; j++) {
            std::cout << M[i][j] << " ";
            N[j][i] = M[i][j];
        }
    }
    std::cout << std::endl;

    for (auto& n : N) {
        for (const auto& m : n) {
            std::cout << m << " ";
        }
    }

    return 0;
}