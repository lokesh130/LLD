#include<bits/stdc++.h> 
#include<thread> 
#include<mutex> 
#include<chrono> 

using namespace std; 

class DiningRoom { 
    private: 
        vector<mutex> forks;
        vector<thread> philosophers;
        mutex logMtx;
        int entityCount;
        
        void startEatThinkProcess(int philosopherId) { 
            while(true) {
                {
                    mutex& fork1 = forks[philosopherId];
                    mutex& fork2 = forks[(philosopherId + 1)%(int)forks.size()];
                    lock(fork1, fork2); 
                    lock_guard<mutex> lock1(fork1, std::adopt_lock);
                    lock_guard<mutex> lock2(fork2, std::adopt_lock);
                    this_thread::sleep_for(chrono::seconds(1));
                    lock_guard<mutex> logLock(logMtx);
                    cout<<"Philosopher "<<philosopherId<<" Eating And Thinking..."<<endl;
                }
                this_thread::sleep_for(chrono::seconds(1));
            }
        }

    public:
        DiningRoom(int entityCount) : forks(entityCount) { 
            this->entityCount = entityCount;
        }

        ~DiningRoom() { 
            for(auto& thread : philosophers) { 
                thread.join();
            }
        }

        void startDining() { 
             for(int i=0; i<entityCount; i++) {
                philosophers.emplace_back(&DiningRoom::startEatThinkProcess, this, i);
             }
    
        }

};

int main() { 
    DiningRoom* diningRoom = new DiningRoom(5); 
    diningRoom->startDining();
    delete diningRoom;
    return 0;
}