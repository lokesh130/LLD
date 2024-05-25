/*
FR: 
    -> N threads should coordinate with each other, 
    after completing a task and before moving to the next task, 
    it should wait whehter all threads has completed thier part
    -> a shared object is passed to all the running threads for coordination.
*/


#include<bits/stdc++.h> 
#include<thread> 
#include<mutex> 
#include<condition_variable>

using namespace std; 

class Barrier { 
    private: 
        mutex mtx; 
        condition_variable cv;
        int waiting; 
        int totalThreads; 
    public: 
        Barrier(int threadCount) : waiting(0), totalThreads(threadCount) {

        }

        void wait() { 
            unique_lock<mutex> lock(mtx);
            waiting++;

            if(waiting == totalThreads) {
                waiting = 0;
                cv.notify_all();
            } else {
                cv.wait(lock, [this]{return waiting == 0;});
            }
        }
};


void process(int threadId, int waitSec, Barrier* barrier) { 
    // for(int i=0; i<4; i++) {
        this_thread::sleep_for(chrono::seconds(waitSec)); 
        cout<<"Thread "<<threadId<<" completed the task."<<endl;
        barrier->wait();
    // }
    cout<<"Thread "<<threadId<<" exiting now"<<endl;
}

int main() { 
    Barrier* barrier = new Barrier(4);
    vector<thread> threads; 

    for(int i=0; i<4; i++) {
        threads.emplace_back(process, i, i+1, barrier);
    }

    for(auto& thread : threads) {
        thread.join();
    }
    
    return 0;
}