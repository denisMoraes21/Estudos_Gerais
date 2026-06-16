//
// Created by denis on 06/04/25.
//

#include <iostream>
#include <vector>

int main() {

    struct People {
        std::string name;
        int age{};
        float high{};
    };

    std::vector<People> p(3);

    for (int i = 0; i < 3; i++) {
        People people;
        std::cout << "Dê um nome para pessoa " << i + 1 << " : ";
        std::cin >> people.name;
        std:: cout << "Dê uma idade para pessoa " << i + 1 << " : ";
        std::cin >> people.age;
        std::cout << "Dê uma altura para pessoa " << i + 1 << " : ";
        std::cin >> people.high;

        p.push_back(people);
    }

    const People& oldest = *std::max_element(p.begin(), p.end(), [](const People& a, const People& b){return a.age < b.age;});;

    std::cout << "O mais velho é " << oldest.name << " com " << oldest.age <<  " anos." <<std::endl;

    return 0;
}