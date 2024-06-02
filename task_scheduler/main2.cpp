#include<bits/stdc++.h> 
#include<thread> 
#include<condition_variable> 
#include<mutex> 
#include<chrono> 

using namespace std; 

mutex logMtx;
void print(string msg) { 
    lock_guard<mutex> lock(logMtx); 
    cout<<msg<<endl;
}

class Task { 
    private: 
        int taskId;

    public: 
        Task(int taskId) : taskId(taskId) { 

        }

        void execute() { 
            print("Executing Task-" + to_string(taskId));
            this_thread::sleep_for(chrono::seconds(1));
            print("Task-" + to_string(taskId) + " executed successfully.");
        }

        int getId() {
            return taskId;
        }
};

class TaskScheduler { 
    private: 
        queue<Task*> taskQu; 
        vector<thread> workers;
        vector<int> executedTaskIds; 
        bool stop;
        condition_variable cv; 
        mutex mtx, stopMtx, execTaskMtx; 
        int taskCounter = 10;

        void startWorker(int workerId) { 
            while(!stop) { 
                Task* task = nullptr;
                {
                    unique_lock<mutex> lock(mtx); 
                    cv.wait(lock, [this]{return (stop || !taskQu.empty());});

                    if(stop) {
                        return;
                    }

                    task = taskQu.front(); 
                    taskQu.pop(); 
                }

                try {
                    task->execute();
                } catch(exception exp) { 
                    {
                        lock_guard<mutex> lock(stopMtx);
                        stop = true;
                    }
                    cv.notify_all();
                }

                {
                    lock_guard<mutex> lock(execTaskMtx);
                    executedTaskIds.push_back(task->getId());
                }

                {
                    unique_lock<mutex> lock(mtx); 
                    vector<Task*> nextTasks = getNextTasks();
                    for(auto& nextTask : nextTasks) {
                        taskQu.push(nextTask);
                    }
                }
            }
        }

        vector<Task*> getNextTasks() { 
            return {new Task(taskCounter++)};
        }

    public: 
        TaskScheduler(int workerCount) { 
            stop = false;
            for(int i=0; i<workerCount; i++) { 
                workers.emplace_back(&TaskScheduler::startWorker, this, i);
            }
        }

        ~TaskScheduler() { 
            for(auto& worker : workers) { 
                worker.join();
            }
        }

        void enqueue(Task* task) { 
            lock_guard<mutex> lock(mtx); 
            taskQu.push(task);
            cv.notify_one();
        }
};


int main() { 
    TaskScheduler* taskScheduler = new TaskScheduler(3);
    
    this_thread::sleep_for(chrono::seconds(2));
    for(int i=0; i<10; i++) {
        taskScheduler->enqueue(new Task(i));
    }

    delete taskScheduler;
}