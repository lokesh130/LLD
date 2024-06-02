#include<bits/stdc++.h> 
#include<thread> 
#include<chrono> 
#include<mutex> 

using namespace std; 


class RateLimiter { 
    private: 
        chrono::system_clock::time_point startTime; 
        int maxAllowedRequests; 
        int timeWindowSize;
        int requestCount; 
        mutex mtx; 

        void reset() {
            startTime = chrono::system_clock::now();
            requestCount = 0;
        }

    public: 
        RateLimiter(int timeWindowSize, int maxAllowedRequests) : timeWindowSize(timeWindowSize), maxAllowedRequests(maxAllowedRequests) { 
            reset();
        }

        bool allow(int requestId) {
            lock_guard<mutex> lock(mtx); 
            int elpasedSeconds = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - startTime).count();
            if(elpasedSeconds >= timeWindowSize) { 
                reset();
            }

            if(requestCount >= maxAllowedRequests) { 
                cout<<"Rejected request "<<requestId<<endl;
                return false;
            } 
            cout<<"Allowing request "<<requestId<<endl;
            requestCount = requestCount + 1;
            return true; 
        }
};


int main() { 
    RateLimiter* rateLimiter = new RateLimiter(1, 4); 
    vector<thread> requestThreads; 

    for(int i=0; i<20; i++) { 
        requestThreads.emplace_back(&RateLimiter::allow, rateLimiter, i);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
    for(auto& request : requestThreads) {
        request.join();
    }

    delete rateLimiter;
}