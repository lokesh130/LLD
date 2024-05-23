#include<bits/stdc++.h> 
#include<semaphore>

using namespace std; 


class MessageQueue { 
    private: 
        queue<string> mq; 
        mutex mtx; 
        unsigned int maxSize;
        counting_semaphore<> items; 
        counting_semaphore<> spaces; 
    
    public: 
        MessageQueue(int maxSize) : maxSize(maxSize), items(0), spaces(maxSize) {}

        void enqueue(string message) {
            spaces.acquire();
            mtx.lock();
            this_thread::sleep_for(chrono::seconds(1)); 
            mq.push(message);
            cout<<"Enqueued message: "<<message<<endl;
            mtx.unlock();
            items.release();
           
        }

        string dequeue() {
            items.acquire(); 
            mtx.lock(); 
            this_thread::sleep_for(chrono::seconds(1)); 
            string message = mq.front(); 
            mq.pop(); 
            cout<<"Dequeued message: "<<message<<endl;
            mtx.unlock(); 
            spaces.release(); 
           
            return message;
        }
}; 

class Producer { 
    private: 
        MessageQueue* mq; 

    public: 
        Producer(MessageQueue* mq) : mq(mq) {}

        void produce(string message) {
            mq->enqueue(message);
        }
};


class Consumer {
    private: 
        MessageQueue* mq; 
    
    public: 
        Consumer(MessageQueue* mq) : mq(mq) {}
        
        void consume() {
            string message = mq->dequeue();
        }
};


int main() {
    MessageQueue* mq = new MessageQueue(20); 
    Producer* producer1 = new Producer(mq); 
    // Producer* producer2 = new Producer(mq); 
    // Producer* producer3 = new Producer(mq); 
    // Producer* producer4 = new Producer(mq); 
    
    Consumer* consumer1 = new Consumer(mq);
    // Consumer* consumer2 = new Consumer(mq);
    // Consumer* consumer3 = new Consumer(mq);
    // Consumer* consumer4 = new Consumer(mq);


    thread producerThread1(&Producer::produce, producer1, "Custom Message 1");
    thread producerThread2(&Producer::produce, producer1, "Custom Message 2");
    thread producerThread3(&Producer::produce, producer1, "Custom Message 3");
    thread producerThread4(&Producer::produce, producer1, "Custom Message 4");
    thread producerThread5(&Producer::produce, producer1, "Custom Message 5");


    thread consumerThread1(&Consumer::consume, consumer1);
    thread consumerThread2(&Consumer::consume, consumer1);
    thread consumerThread3(&Consumer::consume, consumer1);
    thread consumerThread4(&Consumer::consume, consumer1);
    thread consumerThread5(&Consumer::consume, consumer1);

    producerThread1.join();
    producerThread2.join();
    producerThread3.join();
    producerThread4.join();
    producerThread5.join();

    consumerThread1.join();
    consumerThread2.join();
    consumerThread3.join();
    consumerThread4.join();
    consumerThread5.join();

    return 0;
}