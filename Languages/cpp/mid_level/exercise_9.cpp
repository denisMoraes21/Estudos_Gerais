//
// Created by denis on 07/04/25.
//

#include <vector>
#include <iostream>

struct Student {
  float grade_1;
  float grade_2;
  float grade_3;
};

std::vector<Student> list_students;

int main() {
  for (int i = 0; i < 3; i++) {
    Student s{};
    std::cout << "Estudante " << i + 1 << std::endl;
    std::cout << "Grade_1: ";
    std::cin >> s.grade_1;
    std::cout << "Grade_2: ";
    std::cin >> s.grade_2;
    std::cout << "Grade_3: ";
    std::cin >> s.grade_3;
    list_students.emplace_back(s);
  }

  std::vector<float> list_avg;
  for (int j = 0; j < list_students.size(); j++) {
    float sum = 0.0;
    sum += list_students[j].grade_1 + list_students[j].grade_2 + list_students[j].grade_3;
    sum /= 3.0;
    list_avg.emplace_back(sum);
    if (sum >= 7.0) {
      std::cout << "Estudante " << j + 1 << " tirou média " << sum << std::endl;
    }
  }

  return 0;
}



