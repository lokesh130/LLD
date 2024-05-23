/*
FR 
-> user able to book appointment
    -> Appointement Details
        -> apointment-id
        -> date 
        -> startTime & endTime 
     -> return appointment-id if booked
     -> return Fialure, if not possible

-> user should be able to cancel appointement -> given appointment-id 

*/



#include<bits/stdc++.h> 
#include<mutex> 
#include<thread>
#include<chrono> 
using namespace std;

class User {
    private:
        string userName; 
        string userEmail;
        string userContactNo; 
    public:
        User(string userName, string userEmail, string userContactNo) {
            this->userName = userName; 
            this->userEmail = userEmail; 
            this->userContactNo = userContactNo; 
        }
};

class Appointment {
    private:
        mutex mtx; 
        static int idCounter; 
        int appointmentId; 
        string day; // "yyy-mm-dd" format
        double startTime; // "HH:MM" format
        double endTime; // ""HH:MM" fomrat
        User* user; 
    public: 
        Appointment(string day, double startTime, double endTime, User* user) {
            lock_guard<mutex> lock(mtx);
            this->appointmentId = idCounter;
            idCounter++;
            this->day = day; 
            this->startTime = startTime; 
            this->endTime = endTime; 
            this->user = user; 
        }

        int getId() {
            return appointmentId;
        }

        double getStartTime() {
            return startTime; 
        }

        double getEndTime() {
            return endTime;
        }

        string getDay() {
            return day;
        }
};

int Appointment::idCounter = 0;

class AppointmentManager {
    private: 
        mutex mtx; 
        // mutex bookAppointmentMtx, cancelAppointmentMtx, logMtx;
        static mutex instanceMtx;
        unordered_map<int, Appointment*> appointmentsMap;
        unordered_map<string, map<double, bool>> dayVsTimeSlots;
        static AppointmentManager* appointmentManager;
        
        bool checkFeasibility(string day, double startTime, double endTime) {
            double currTime = startTime; 

            while(currTime != endTime) {
                if(dayVsTimeSlots[day][currTime]) {
                    return false;
                }
                currTime += 0.5; 
            }
            return true;
        }

        void fillTimeSlots(string day, double startTime, double endTime, bool value) {
            double currTime = startTime; 

            while(currTime != endTime) {
                dayVsTimeSlots[day][currTime] = value;
                currTime += 0.5; 
            }
        }

        AppointmentManager() {
            
        }

    public: 
        static AppointmentManager* getInstance() { 
           lock_guard<mutex> lock(instanceMtx);
           if(appointmentManager == nullptr) {
                appointmentManager = new AppointmentManager();
           }
           return appointmentManager;
        }

        int bookAppointment(User* user, string day, double startTime, double endTime) {
            lock_guard<mutex> lock(mtx);
            bool isFeasible = checkFeasibility(day, startTime, endTime);
            if(!isFeasible) {
                cout<<"Not feasible to schedule appointment on day: "<<day<<" at time: "<<startTime<<"-"<<endTime<<endl;
                return -1;
            }

            Appointment* appointment = new Appointment(day, startTime, endTime, user);
            appointmentsMap[appointment->getId()] = appointment;
            fillTimeSlots(day, startTime, endTime, true);
            cout<<"Appoitment Booked from "<<startTime<<"-"<<endTime<<", appointment-id: "<<appointment->getId()<<endl;
            return appointment->getId();
        }

        void printAppoitmentDetails(string day) {
            lock_guard<mutex> lock(mtx);
            cout<<"Priniting booked slots for day: "<<day<<endl;
            for(auto entry : dayVsTimeSlots[day]) {
                if(entry.second) {
                    cout<<"TimeSlot: "<<entry.first<<" to "<<entry.first + 0.5<<endl;
                }
            }
            cout<<endl;
        }

        void cancelAppointment(User* user, int appointmentId) {
            lock_guard<mutex> lock(mtx);
            Appointment* appointment = appointmentsMap[appointmentId]; 
            fillTimeSlots(appointment->getDay(), appointment->getStartTime(), appointment->getEndTime(), false);
            appointmentsMap.erase(appointmentId);
            delete appointment;
            cout<<"Appointment-id: "<<appointmentId<<" cancelled successfully."<<endl;
        }
};

AppointmentManager* AppointmentManager::appointmentManager = nullptr;
mutex AppointmentManager::instanceMtx;

int bookAppointment(User* user, string day, double startTime, double endTime) {
    AppointmentManager* appointmentManager = AppointmentManager::getInstance();
    return appointmentManager->bookAppointment(user, day, startTime, endTime); 
}

void appointmentSimulator(User* user, string day, double startTime, double endTime) {
    AppointmentManager* appointmentManager = AppointmentManager::getInstance();
    int appointmentId = appointmentManager->bookAppointment(user, day, startTime, endTime); 
    this_thread::sleep_for(chrono::seconds(2));
    appointmentManager->printAppoitmentDetails(day);
    this_thread::sleep_for(chrono::seconds(2));
    appointmentManager->cancelAppointment(user, appointmentId); 
    this_thread::sleep_for(chrono::seconds(1));
    appointmentManager->printAppoitmentDetails(day);
}

int main() { 
    User* user = new User("lokesh", "mlokesh.mamta@gmail.com", "81300331107");
    
    // user, 
    // future<int> appointmentCreatorFuture1 = async(bookAppointment, user, "2024-05-24", 9.5, 10.5); 
    // future<int> appointmentCreatorFuture2 = async(bookAppointment, user, "2024-05-24", 10, 11);

    // int appointementId1= appointmentCreatorFuture1.get();
    // int appointementId2= appointmentCreatorFuture2.get();

    // cout<<"AppoitmentId for 1st Thread: "<<appointementId1<<endl;
    // cout<<"AppoitmentId for 2nd Thread: "<<appointementId2<<endl;
    

    thread appoitmentCreatorThread1(appointmentSimulator, user, "2024-05-24", 9.5, 10.5);
    thread appoitmentCreatorThread2(appointmentSimulator, user, "2024-05-24", 10, 11);
    thread appoitmentCreatorThread3(appointmentSimulator, user, "2024-05-24", 10.5, 11.5);
    thread appoitmentCreatorThread4(appointmentSimulator, user, "2024-05-24", 12, 13);
    thread appoitmentCreatorThread5(appointmentSimulator, user, "2024-05-24", 12.5, 13.5);

    appoitmentCreatorThread1.join();
    appoitmentCreatorThread2.join();
    appoitmentCreatorThread3.join();
    appoitmentCreatorThread4.join();
    appoitmentCreatorThread5.join();

    return 0;
}