/*
FR: 
-> Create a thread-safe singleton AppointmentService class.
-> Don't create any methods, just an illustration of thread safety
*/

#include<bits/stdc++.h> 
#include<thread>
#include<mutex> 
using namespace std; 


class AppointmentService { 
    private:
        static mutex mtx;
        static AppointmentService* appointmentService;
        AppointmentService() { 
            cout<<"Object creation happened"<<endl;
        }

    public:
        static AppointmentService* getInstance() { 
            lock_guard<mutex> lock(mtx);
            if(appointmentService == nullptr) {
                appointmentService = new AppointmentService();
            }
            // cout<<"Got instance of appointmentService"<<endl;
            return appointmentService;
        }
        
};

AppointmentService* AppointmentService::appointmentService = nullptr;
mutex AppointmentService::mtx;

int main() { 

    vector<thread> threads; 

    for(int i=0; i<10; i++) {
        threads.emplace_back(&AppointmentService::getInstance);
    }

    for(auto& thread : threads) {
        thread.join();
    }

    return 0;
}