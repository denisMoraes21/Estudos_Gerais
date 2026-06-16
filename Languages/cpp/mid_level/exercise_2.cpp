//
// Created by denis on 07/04/25.
//

#include <iostream>

class BankAccount {
    private:
        int balance;
        std::string owner;

    public:
      BankAccount(const int balance, const std::string& owner) {
        this->balance = balance;
        this->owner = owner;
      }

      void setDeposit(const int value) {
        this->balance += value;
      }

      bool setWithdraw(const int value) {
        if (this->balance >= value) {
          this->balance -= value;
          return true;
        }
        return false;
      }

      [[nodiscard]] std::string showBalance() const {
        return this->owner + " " + std::to_string(this->balance);
      }
};


int main() {
  BankAccount bankAccount(100, "Denis");
  bankAccount.setDeposit(200);
  bankAccount.setWithdraw(300);
  std::cout << bankAccount.showBalance() <<std::endl;

  return 0;
}
