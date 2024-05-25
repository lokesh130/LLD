/* 
FR: 
-> There are n forks
-> There is n philosophers who
    -> thinks
    -> pick two forks in both hands
    -> eat a bite
    -> put back two forks 
-> philosophers can't eat if he doesn't get two forks. 
-> philosophers are seated in circular dining table. 
*/

#include<bits/stdc++.h> 
#include<mutex> 
#include<thread> 

using namespace std; 


class DiningRoom {
    private: 
        vector<mutex> forks; 
        int philosopherCount;
    public:
        DiningRoom(int philosopherCount) : philosopherCount(philosopherCount), forks(philosopherCount) {
            // forks = vector<mutex>(philosopherCount);
        }

        void startProcess(int philosopherId) {
            for(int i=0; i<5; i++) {
                // think
                this_thread::sleep_for(chrono::seconds(2));
                {
                    lock(forks[philosopherId], forks[(philosopherId + 1)%philosopherCount]); 
                    lock_guard<mutex> fork1(forks[philosopherId], adopt_lock); 
                    lock_guard<mutex> fork2(forks[(philosopherId + 1)%philosopherCount], adopt_lock);

                    // eat
                    cout<<"Philosopher "<<philosopherId<<" is eating.."<<endl;
                    this_thread::sleep_for(chrono::seconds(1));
                }
            }
        }
};


int main() { 

    DiningRoom diningRoom(5); 

    vector<thread> philosopherThreads; 

    for(int i=0; i<5; i++) {
        philosopherThreads.emplace_back(&DiningRoom::startProcess, &diningRoom, i);
    }

    for(auto& thread : philosopherThreads) { 
        thread.join();
    }

    return 0;
}