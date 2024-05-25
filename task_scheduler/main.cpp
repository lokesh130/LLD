/* 
FR: 
-> method to insert task in pool 
-> There will be n no of workers, 
which will keep on taking task from pool and execute it, 
once a worker gets free it will again repeat the same thing.
-> if one worker is popping from the queue, another worker should not interfere. 
-> if no tasks, task should wait in queue. 
-> There should be a option to stop all threads. 
    Threads should stop gracefully, if there are still tasks to process in queue, 
    threads will not stop untill all tasks are not processed.
*/

#include<bits/stdc++.h> 
#include<thread> 
#include<mutex> 
#include<condition_variable> 
using namespace std; 


class ThreadPool { 
    private: 
        vector<thread> workers; 
        queue<function<void(int)>> taskQu; 
        mutex mtx, enqueMtx; 
        condition_variable cv; 
        bool stop; 

        void startWorkerProcess(int workerId) { 
            cout<<"worker "<<workerId<<" has started"<<endl;
            while(true) {
                function<void(int)> task; 
                {   
                    unique_lock<mutex> lock(mtx); 
                    cv.wait(lock, [this]{return (stop || !taskQu.empty()); });
                    if(stop && taskQu.empty()) return; 

                    task = std::move(taskQu.front()); 
                    taskQu.pop(); 
                    cout<<"Worker "<<workerId<<" has extracted out the task"<<endl;
                }

                task(workerId);
            }
        }

    public:
        ThreadPool(int workerCount) { 
            this->stop = false; 
            for(int i=0; i<workerCount; i++) {
                workers.emplace_back(&ThreadPool::startWorkerProcess, this, i);
            }
        }

        ~ThreadPool() { 
            stop = true; 
            cv.notify_all();

            for(auto& worker : workers) {
                worker.join();
            }
        }

        void enqueue(function<void(int)> task) {
            lock_guard<mutex> lock(enqueMtx); 
            taskQu.push(task); 
            cv.notify_one();
        }
    
};


int main() { 
    ThreadPool threadPool(3); 

    this_thread::sleep_for(chrono::seconds(2));

    for(int i=0; i<10; i++) {
        threadPool.enqueue([](int workerId) {
            cout<<"Worker "<<workerId<<" is executing a task."<<endl;
            this_thread::sleep_for(chrono::seconds(1));
        });
    }

    return 0;
}