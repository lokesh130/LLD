#include<bits/stdc++.h> 
#include<mutex> 
#include<shared_mutex> 
using namespace std; 

class User { 
    private: 
        static int idCounter;
        int userId;
        string userName; 
        mutex mtx;
    public: 
        unordered_map<int, int> balanceSheet; 

        User(string userName) { 
            lock_guard<mutex> lock(mtx);
            idCounter++;
            this->userId = idCounter;
            this->userName = userName;
        }

        int getId() { 
            return this->userId;
        }
};
int User::idCounter = 0;

class Group { 
    private: 
        static int idCounter;
        int groupId;
        string groupName; 
        mutex mtx;
    public: 
        unordered_map<int, unordered_map<int, int>> balanceSheet;

        Group(string groupName) { 
            lock_guard<mutex> lock(mtx);
            idCounter++;
            this->groupId = idCounter;
            this->groupName = groupName;
        }

        int getId() { 
            return this->groupId;
        }
};
int Group::idCounter = 0;


struct Expense { 
    int userIdPaid; 
    vector<int> userBorrowed; 
    int amount; 
    Expense(int userIdPaid, vector<int> userBorrowed, int amount) { 
        this->userIdPaid = userIdPaid; 
        this->userBorrowed = userBorrowed; 
        this->amount = amount; 
    }
};

class SplitwiseService {
    private: 
        static mutex singletonMtx;
        shared_mutex sharedMtx; 

        static SplitwiseService* splitwiseService; 
        unordered_map<int, User*> userMp; 
        unordered_map<int, Group*> groupMp;
        
        SplitwiseService() { 

        }
        
        void createUserExpenseHelper(Expense expense) { 
            int individualAmount = expense.amount/(int)expense.userBorrowed.size();

            for(int i=0; i<expense.userBorrowed.size(); i++) {
                userMp[expense.userIdPaid]->balanceSheet[expense.userBorrowed[i]] += individualAmount; 
                userMp[expense.userBorrowed[i]]->balanceSheet[expense.userIdPaid] -= individualAmount; 
            }
        }   

    public:
        static SplitwiseService* getInstance() { 
            lock_guard<mutex> lock(singletonMtx);
            if(splitwiseService == nullptr) { 
                splitwiseService = new SplitwiseService();
            } 
            return splitwiseService;
        }

        int addUser(string userName) { 
            User* user = new User(userName); 
            userMp[user->getId()] = user; 
            return user->getId();
        }

        int addGroup(string groupName) {
            Group* group = new Group(groupName); 
            groupMp[group->getId()] = group;
            return group->getId();
        }

        void createUserExpense(Expense expense) { 
            unique_lock<shared_mutex> writeLock(sharedMtx);
            createUserExpenseHelper(expense);
        }   

        void createGroupExpense(int groupId, Expense expense) { 
            unique_lock<shared_mutex> writeLock(sharedMtx);
            int individualAmount = expense.amount/(int)expense.userBorrowed.size();

            for(int i=0; i<expense.userBorrowed.size(); i++) {
                groupMp[groupId]->balanceSheet[expense.userIdPaid][expense.userBorrowed[i]] += individualAmount; 
                groupMp[groupId]->balanceSheet[expense.userBorrowed[i]][expense.userIdPaid] -= individualAmount; 
            }
            createUserExpenseHelper(expense);
        }

        void viewUserBalanceSheet(int userId) { 
            shared_lock<shared_mutex> readLock(sharedMtx);
            for(auto& ele : userMp[userId]->balanceSheet) { 
                if(ele.second != 0) {
                    cout<<"UserId: "<<ele.first<<" Balance: "<<ele.second<<endl;
                }
            }
        }

        void viewGroupBalanceSheet(int groupId) {
            shared_lock<shared_mutex> readLock(sharedMtx);
            for(auto& ele : groupMp[groupId]->balanceSheet) { 
                cout<<"For user-id: "<<ele.first<<endl;
                if(!ele.second.empty()) {
                    for(auto& ele1 : groupMp[groupId]->balanceSheet[ele.first]) {
                        cout<<"UserId: "<<ele1.first<<" Balance: "<<ele1.second<<endl;
                    }
                }
            }
        }
};

SplitwiseService* SplitwiseService::splitwiseService = nullptr;
mutex SplitwiseService::singletonMtx;

int main() { 
    SplitwiseService* splitwiseService = SplitwiseService::getInstance(); 
    
    int userId1 = splitwiseService->addUser("lokesh"); 
    int userId2 = splitwiseService->addUser("yogesh"); 
    int userId3 = splitwiseService->addUser("tanuja"); 
    int userId4 = splitwiseService->addUser("rakesh"); 
    int userId5 = splitwiseService->addUser("rohan"); 
    int userId6 = splitwiseService->addUser("kajal"); 

    Expense expense1({userId1, {userId2, userId3}, 100});
    cout<<"Adding user Expense"<<endl;
    splitwiseService->createUserExpense(expense1);
    cout<<endl;

    cout<<"BalanceSheet printing for user-id-1"<<endl;
    splitwiseService->viewUserBalanceSheet(userId1);
    cout<<endl;

    int groupId1 = splitwiseService->addGroup("travel");
    
    Expense expense2({userId2, {userId4, userId5, userId6}, 200});
    cout<<"Adding group Expense"<<endl;
    splitwiseService->createGroupExpense(groupId1, expense2);
    cout<<endl;

    cout<<"BalanceSheet printing for group-id-1"<<endl;
    splitwiseService->viewGroupBalanceSheet(groupId1);
    cout<<endl;

    cout<<"BalanceSheet printing for user-id-2"<<endl;
    splitwiseService->viewUserBalanceSheet(userId2);
    cout<<endl;

    cout<<"BalanceSheet printing for user-id-1"<<endl;
    splitwiseService->viewUserBalanceSheet(userId1);
    cout<<endl;
    return 0;
}