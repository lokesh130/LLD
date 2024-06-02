#include<bits/stdc++.h> 
#include<thread> 
#include<chrono> 
#include<mutex> 
#include<condition_variable> 

using namespace std;

class BarrierSyncer { 
    private:
        mutex mtx;
        condition_variable cv; 
        int totalEntities; 
        int entityCounter;

    public:
        BarrierSyncer(int totalEntities) : totalEntities(totalEntities), entityCounter(0) { 

        }

        void wait() { 
            unique_lock<mutex> lock(mtx); 
            entityCounter++; 
            if(entityCounter == totalEntities) {
                entityCounter = 0; 
                cv.notify_all();
            } else {
                cv.wait(lock, [this]{return entityCounter == 0;});
            }
        }
};



mutex logMtx;

void print(string message) { 
    lock_guard<mutex> lock(logMtx);
    cout<<message<<endl;
}

void executeProcess(int threadId, int taskSeconds, BarrierSyncer* barrierSyncer) { 
    while(true) { 
        print("Thread " + to_string(threadId) + " performing its task");
        this_thread::sleep_for(chrono::seconds(taskSeconds)); 
        print("Thread " + to_string(threadId) + " performed its task");
        barrierSyncer->wait();
    }
}


int main() { 
    int threadCount = 5;
    BarrierSyncer* barrierSyncer = new BarrierSyncer(threadCount);

    vector<thread> threads;

    for(int i=0; i<threadCount; i++) { 
        threads.emplace_back(executeProcess, i, i, barrierSyncer);
    }

    for(auto& thread : threads) { 
        thread.join();
    }
    
    delete barrierSyncer;
    return 0;
}