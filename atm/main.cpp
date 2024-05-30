
#include <bits/stdc++.h>
using namespace std;

class Account {
private:
    long accountNo;
    double availableAmount;

public:
    Account(long accountNo, double availableAmount) : accountNo(accountNo), availableAmount(availableAmount) {}

    long getAccountNo() { return accountNo; };
    double getAcccountBalance() { return availableAmount; };
    bool withdraw(int amount) {
        if (availableAmount >= amount) {
            availableAmount -= amount;
            return true;
        } else {
            std::cout << "Available amount with bank is not sufficient.";
            return false;
        }
    }
};

class Card {
private:
    std::string cardNo;
    Account* account;
    int pinNo;

public:
    Card(std::string cardNo, int pin, Account* account) : cardNo(cardNo), pinNo(pin), account(account) {}

    Account* getLinkedAccount() { return account; };
    bool validatePin(int pinNo) { return this->pinNo == pinNo; };
};

class ATMState {
protected:
    class ATM* atm;

public:
    ATMState(class ATM* atm) : atm(atm) {}
    virtual ~ATMState() = default;
    virtual void insertCard(Card* card) = 0;
    virtual void validatePin(int pin) = 0;
    virtual void withdraw(int amount) = 0;
    virtual void ejectCard() = 0;
};

class IdleState : public ATMState {
public:
    IdleState(class ATM* atm) : ATMState(atm) {}

    void insertCard(Card* card) override {
        atm->setInsertedCard(card);
        atm->setState(new CardInsertedState(atm));
        std::cout << "Card inserted successfully" << std::endl;
    }

    void validatePin(int pin) override {
        std::cout << "Insert Card First, and then validate pin" << std::endl;
    }

    void withdraw(int amount) override {
        std::cout << "Insert Card First, to withdraw amount" << std::endl;
    }

    void ejectCard() override {
        std::cout << "Insert Card First" << std::endl;
    }
};

class CardInsertedState : public ATMState {
public:
    CardInsertedState(class ATM* atm) : ATMState(atm) {}

    void insertCard(Card* card) override {
        std::cout << "Card already inserted" << std::endl;
    }

    void validatePin(int pin) override {
        bool result = atm->getInsertedCard()->validatePin(pin);
        if (!result) {
            std::cout << "Pin Validation Failed, please reenter" << std::endl;
        } else {
            std::cout << "Pin Validation succeeded." << std::endl;
            atm->setState(new TransactionProcessingState(atm));
        }
    }

    void withdraw(int amount) override {
        std::cout << "Validate Card First, to withdraw amount" << std::endl;
    }

    void ejectCard() override {
        atm->setInsertedCard(nullptr);
        atm->setState(new IdleState(atm));
        std::cout << "Card ejected successfully" << std::endl;
    }
};

class TransactionProcessingState : public ATMState {
public:
    TransactionProcessingState(class ATM* atm) : ATMState(atm) {}

    void insertCard(Card* card) override {
        std::cout << "Ongoing transaction, please wait for transaction to complete." << std::endl;
    }

    void validatePin(int pin) override {
        std::cout << "Pin already validated." << std::endl;
    }

    void withdraw(int amount) override {
        Account* account = atm->getInsertedCard()->getLinkedAccount();
        if (atm->getAvailableCash() < amount) {
            std::cout << "ATM doesn't have enough cash" << std::endl;
        } else if (account->withdraw(amount)) {
            atm->dispenseCash(amount);
            std::cout << "Please collect your cash" << std::endl;
        } else {
            std::cout << "Your account doesn't have enough money" << std::endl;
        }
    }

    void ejectCard() override {
        atm->setInsertedCard(nullptr);
        atm->setState(new IdleState(atm));
        std::cout << "Card ejected successfully" << std::endl;
    }
};

class ATM {
private:
    ATMState* atmState;
    Card* card;
    double availableCash;

public:
    ATM(double availableCash) : availableCash(availableCash), card(nullptr), atmState(new IdleState(this)) {}

    void setState(ATMState* atmState) {
        this->atmState = atmState;
    }

    void insertCard(Card* card) {
        atmState->insertCard(card);
    }

    void validatePin(int pin) {
        atmState->validatePin(pin);
    }

    void withdraw(int amount) {
        atmState->withdraw(amount);
    }

    void ejectCard() {
        atmState->ejectCard();
    }

    double getAvailableCash() { return availableCash; }
    bool dispenseCash(double amount) {
        if (availableCash >= amount) {
            availableCash -= amount;
            return true;
        } else {
            return false;
        }
    }

    Card* getInsertedCard() { return card; };
    void setInsertedCard(Card* card) { this->card = card; };
};

int main() {
    Account* myAccount = new Account(123456, 780);
    Card* card = new Card("123-4556-234-342", 1234, myAccount);
    ATM* atm = new ATM(100000);

    atm->insertCard(card);
    atm->validatePin(1234);
    atm->withdraw(100);
    cout << atm->getInsertedCard()->getLinkedAccount()->getAcccountBalance() << endl;
    atm->ejectCard();

    return 0;
}



/*
#include <bits/stdc++.h>
using namespace std;

class Account {
private:
    long accountNo;
    double availableAmount;

public:
    Account(long accountNo, double availableAmount) : accountNo(accountNo), availableAmount(availableAmount) {}

    long getAccountNo() { return accountNo; };
    double getAcccountBalance() { return availableAmount; };
    bool withdraw(int amount) {
        if (availableAmount >= amount) {
            availableAmount -= amount;
            return true;
        } else {
            std::cout << "Available amount with bank is not sufficient.";
            return false;
        }
    }
};

class Card {
private:
    std::string cardNo;
    Account* account;
    int pinNo;

public:
    Card(std::string cardNo, int pin, Account* account) : cardNo(cardNo), pinNo(pin), account(account) {}

    Account* getLinkedAccount() { return account; };
    bool validatePin(int pinNo) { return this->pinNo == pinNo; };
};

class ATMState {
protected:
    class ATM* atm;

public:
    ATMState(class ATM* atm) : atm(atm) {}
    virtual ~ATMState() = default;
    virtual void insertCard(Card* card) = 0;
    virtual void validatePin(int pin) = 0;
    virtual void withdraw(int amount) = 0;
    virtual void ejectCard() = 0;
};

class IdleState : public ATMState {
public:
    IdleState(class ATM* atm) : ATMState(atm) {}

    void insertCard(Card* card) override;
    void validatePin(int pin) override;
    void withdraw(int amount) override;
    void ejectCard() override;
};

class CardInsertedState : public ATMState {
public:
    CardInsertedState(class ATM* atm) : ATMState(atm) {}

    void insertCard(Card* card) override;
    void validatePin(int pin) override;
    void withdraw(int amount) override;
    void ejectCard() override;
};

class TransactionProcessingState : public ATMState {
public:
    TransactionProcessingState(class ATM* atm) : ATMState(atm) {}

    void insertCard(Card* card) override;
    void validatePin(int pin) override;
    void withdraw(int amount) override;
    void ejectCard() override;
};

class ATM {
private:
    ATMState* atmState;
    Card* card;
    double availableCash;

public:
    ATM(double availableCash) : availableCash(availableCash), card(nullptr), atmState(new IdleState(this)) {}

    void setState(ATMState* atmState) {
        this->atmState = atmState;
    }

    void insertCard(Card* card) {
        atmState->insertCard(card);
    }

    void validatePin(int pin) {
        atmState->validatePin(pin);
    }

    void withdraw(int amount) {
        atmState->withdraw(amount);
    }

    void ejectCard() {
        atmState->ejectCard();
    }

    double getAvailableCash() { return availableCash; }
    bool dispenseCash(double amount) {
        if (availableCash >= amount) {
            availableCash -= amount;
            return true;
        } else {
            return false;
        }
    }

    Card* getInsertedCard() { return card; };
    void setInsertedCard(Card* card) { this->card = card; };
};

void IdleState::insertCard(Card* card) {
    atm->setInsertedCard(card);
    atm->setState(new CardInsertedState(atm));
    std::cout << "Card inserted successfully" << std::endl;
}

void IdleState::validatePin(int pin) {
    std::cout << "Insert Card First, and then validate pin" << std::endl;
}

void IdleState::withdraw(int amount) {
    std::cout << "Insert Card First, to withdraw amount" << std::endl;
}

void IdleState::ejectCard() {
    std::cout << "Insert Card First" << std::endl;
}

void CardInsertedState::insertCard(Card* card) {
    std::cout << "Card already inserted" << std::endl;
}

void CardInsertedState::validatePin(int pin) {
    bool result = atm->getInsertedCard()->validatePin(pin);
    if (!result) {
        std::cout << "Pin Validation Failed, please reenter" << std::endl;
    } else {
        std::cout << "Pin Validation succeeded." << std::endl;
        atm->setState(new TransactionProcessingState(atm));
    }
}

void CardInsertedState::withdraw(int amount) {
    std::cout << "Valdate Card First, to withdraw amount" << std::endl;
}

void CardInsertedState::ejectCard() {
    atm->setInsertedCard(nullptr);
    atm->setState(new IdleState(atm));
    std::cout << "Card ejected successfully" << std::endl;
}

void TransactionProcessingState::insertCard(Card* card) {
    std::cout << "Ongoing transaction, please wait for transaction to complete." << std::endl;
}

void TransactionProcessingState::validatePin(int pin) {
    std::cout << "Pin already validated." << std::endl;
}

void TransactionProcessingState::withdraw(int amount) {
    Account* account = atm->getInsertedCard()->getLinkedAccount();
    if (atm->getAvailableCash() < amount) {
        std::cout << "ATM doesn't have enough cash" << std::endl;
    } else if (account->withdraw(amount)) {
        atm->dispenseCash(amount);
        std::cout << "Please collect your cash" << std::endl;
    } else {
        std::cout << "Your account doesn't have enough money" << std::endl;
    }
}

void TransactionProcessingState::ejectCard() {
    atm->setInsertedCard(nullptr);
    atm->setState(new IdleState(atm));
    std::cout << "Card ejected successfully" << std::endl;
}

int main() {
    Account* myAccount = new Account(123456, 780);
    Card* card = new Card("123-4556-234-342", 1234, myAccount);
    ATM* atm = new ATM(100000);

    atm->insertCard(card);
    atm->validatePin(1234);
    atm->withdraw(100);
    cout<<atm->getInsertedCard()->getLinkedAccount()->getAcccountBalance()<<endl;
    atm->ejectCard();

    return 0;
}
*/