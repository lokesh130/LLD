/* 
FR
-> Implement Fixed Window Rate Limiter 
-> It should allow N Requests per second (N should be configurable), extra request should get discarded. 
-> Counter should get reset to 0 after 1 sec is done. 
*/


#include<bits/stdc++.h> 
#include<mutex> 
#include<thread> 
#include<chrono> 

using namespace std; 


class RateLimiter {
    private: 
        mutex mtx; 
        static int counter;
        int maxRequestCount;
        chrono::system_clock::time_point startTime;

        void reset() { 
            startTime = chrono::system_clock::now(); 
            counter = 0;
        }

    public: 
        RateLimiter(int maxRequest) {
            startTime = chrono::system_clock::now();
            maxRequestCount = maxRequest;
        }

        bool allow() { 
            lock_guard<mutex> lock(mtx);
            chrono::system_clock::time_point currTime = chrono::system_clock::now(); 
            int seconds = duration_cast<chrono::seconds>(currTime - startTime).count();

            if(seconds >= 1) {
                reset();
            }

            if(counter >= maxRequestCount) {
                cout<<"Not Allowed"<<endl;
                return false;
            }

            counter++;
            cout<<"Allowed"<<endl;
            return true;
        }


};

int RateLimiter::counter = 0;


int main() { 
    RateLimiter rateLimiter(2); 

    vector<thread> threads; 

    for(int i=0; i<10; i++) {
        threads.emplace_back(&RateLimiter::allow, &rateLimiter);
        this_thread::sleep_for(chrono::milliseconds(200));
    }

    for(int i=0; i<threads.size(); i++) {
        threads[i].join();
    }

    return 0;
}