/*
schedule:

Executes a command after a given delay. A separate thread is created that sleeps for the specified delay and then runs the command.
scheduleAtFixedRate:

Executes a command at a fixed rate after an initial delay. The function calculates the time taken to execute the command and adjusts the sleep time to maintain a fixed rate.
scheduleWithFixedDelay:

Executes a command after an initial delay and then with a fixed delay between the termination of one execution and the start of the next.
stop:

Stops all scheduled tasks by setting the done flag to true.
*/

#include<bits/stdc++.h> 
#include<thread> 
#include<chrono> 

using namespace std; 


class ScheduledExecutorService { 
private: 
    bool done;
public: 
    ScheduledExecutorService() { 
        this->done = false;
    }

    void schedule(function<void()> command, int delay, int timeUnit) {
        thread([command, delay, timeUnit]{
            this_thread::sleep_for(chrono::milliseconds(delay*timeUnit));
            command();
        }).detach();
    }

    void scheduleAtFixedRate(function<void()> command, int initialDelay, int period, int timeUnit) {
        thread([this, command, initialDelay, period, timeUnit]{
             this_thread::sleep_for(chrono::milliseconds(initialDelay*timeUnit));
            while(!done) {
                chrono::system_clock::time_point startTime = chrono::system_clock::now();
                command();
                this_thread::sleep_until(startTime + chrono::milliseconds(period*timeUnit));
            }
        }).detach();
    }

    void scheduleAtFixedDelay(function<void()> command, int initialDelay, int period, int timeUnit) {
        thread([this, command, initialDelay, period, timeUnit]{
            this_thread::sleep_for(chrono::milliseconds(initialDelay*timeUnit));
            while(!done) { 
                command();
                this_thread::sleep_for(chrono::milliseconds(period*timeUnit));
            }
        }).detach();
    }

    void stop() { 
        done = true;
    }
};

void exampleTask() {
    this_thread::sleep_for(chrono::seconds(1));
    std::cout << "Task executed at " << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << std::endl;
}


int main() { 
    std::cout << "Start Time " << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << std::endl;
    ScheduledExecutorService scheduledExecutorService; 
    // scheduledExecutorService.schedule(exampleTask, 3000, 1);

    // scheduledExecutorService.scheduleAtFixedRate(exampleTask, 1000, 3000, 1);

    scheduledExecutorService.scheduleAtFixedDelay(exampleTask, 1000, 3000, 1);
    this_thread::sleep_for(chrono::seconds(10));
    std::cout << "End Time " << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << std::endl;
    scheduledExecutorService.stop();
}