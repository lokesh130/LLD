#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class User {
private:
    string id;
    string name;

public:
    User(const string& id, const string& name) : id(id), name(name) {}

    string getId() const { return id; }
    string getName() const { return name; }
};

class Split {
private:
    User* user;
    double amount;

public:
    Split(User* user, double amount) : user(user), amount(amount) {}

    User* getUser() const { return user; }
    double getAmount() const { return amount; }
};

class Expense {
private:
    string expenseId;
    double totalAmount;
    User* paidBy;
    vector<Split*> splits;

public:
    Expense(const string& expenseId, double totalAmount, User* paidBy, const vector<Split*>& splits) 
        : expenseId(expenseId), totalAmount(totalAmount), paidBy(paidBy), splits(splits) {}

    string getExpenseId() const { return expenseId; }
    double getTotalAmount() const { return totalAmount; }
    User* getPaidBy() const { return paidBy; }
    const vector<Split*>& getSplits() const { return splits; }
};

class ExpenseManager {
private:
    unordered_map<string, User*> userMap;
    unordered_map<string, unordered_map<string, double>> balanceSheet;

    ExpenseManager() {}

public:
    static ExpenseManager& getInstance() {
        static ExpenseManager instance;
        return instance;
    }

    void addUser(User* user) {
        userMap[user->getId()] = user;
        balanceSheet[user->getId()] = unordered_map<string, double>();
    }

    void addExpense(const string& expenseId, double totalAmount, User* paidBy, const vector<Split*>& splits) {
        Expense* expense = new Expense(expenseId, totalAmount, paidBy, splits);
        
        for (Split* split : splits) {
            string paidTo = split->getUser()->getId();
            if (paidBy->getId() != paidTo) {
                balanceSheet[paidBy->getId()][paidTo] += split->getAmount();
                balanceSheet[paidTo][paidBy->getId()] -= split->getAmount();
            }
        }
    }

    void showBalances() {
        for (auto userBalance : balanceSheet) {
            string userId = userBalance.first;
            for (auto balance : userBalance.second) {
                if (balance.second != 0) {
                    cout << userMap[userId]->getName() << " owes " << userMap[balance.first]->getName() << ": " << balance.second << endl;
                }
            }
        }
    }
};

int main() {
    User* u1 = new User("u1", "User1");
    User* u2 = new User("u2", "User2");
    User* u3 = new User("u3", "User3");

    ExpenseManager& expenseManager = ExpenseManager::getInstance();
    expenseManager.addUser(u1);
    expenseManager.addUser(u2);
    expenseManager.addUser(u3);

    vector<Split*> splits = { new Split(u2, 50), new Split(u3, 50) };
    expenseManager.addExpense("e1", 100, u1, splits);

    expenseManager.showBalances();

    return 0;
}
