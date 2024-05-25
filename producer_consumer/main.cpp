#include<bits/stdc++.h> 
#include<condition_variable> 
#include<mutex> 

using namespace std; 


class Pipeline {
    private: 
        mutex mtx; 
        condition_variable spaces, items; 
        queue<int> qu; 
        int capacity; 
        static int currItems;
    public:
        Pipeline(int capacity) { 
            this->capacity = capacity;
        }

        void produce(int value) { 
            unique_lock<mutex> lock(mtx);
            spaces.wait(lock, [this](){return currItems < capacity;});
            qu.push(value);
            currItems++; 
            cout<<"Produced value: "<<value<<endl;
            items.notify_one();
        } 

        int consume() { 
            unique_lock<mutex> lock(mtx); 
            items.wait(lock, [this](){return currItems > 0;});
            int tpValue = qu.front();
            qu.pop();
            currItems--;
            cout<<"Consumed value: "<<tpValue<<endl;
            spaces.notify_one();
            return tpValue;
        }
};

int Pipeline::currItems = 0;


int main() { 
    Pipeline pipeline(3); 
    vector<thread> producerThreads, consumeThreads;

    for(int i=0; i<50; i++) {
        producerThreads.emplace_back(&Pipeline::produce, &pipeline, i+1);
    }

    this_thread::sleep_for(chrono::seconds(2));
    
    for(int i=0; i<50; i++) {
        consumeThreads.emplace_back(&Pipeline::consume, &pipeline);
    }


    for(auto& thread: producerThreads) {
        thread.join();
    }

    for(auto& thread: consumeThreads) {
        thread.join();
    }

    return 0;
}





