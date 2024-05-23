#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>     

using namespace std;

struct Request {
    int floor;
};

class Elevator {
private:
    int elevatorId;
    int currentFloor;
    vector<Request> upRequests;
    vector<Request> downRequests;

public:
    Elevator(int elevatorId) : elevatorId(elevatorId), currentFloor(0) {}
    
    void handleRequest(Request request) {
        if (request.floor > currentFloor)
            upRequests.push_back(request);
        else
            downRequests.push_back(request);
    }

    int getIndex() {
        return elevatorId;
    }

    void executeRequests() {
        cout<<"Currently lift-id "<<elevatorId<<" is at: "<<currentFloor<<endl;
        vector<Request> seekSequence;
        int seekCount = 0;
        int distance;

        sort(upRequests.begin(), upRequests.end(), [](const Request &a, const Request &b) {
            return a.floor < b.floor;
        });
        sort(downRequests.begin(), downRequests.end(), [](const Request &a, const Request &b) {
            return a.floor > b.floor;
        });

        while (!upRequests.empty() || !downRequests.empty()) {
            for (auto it = upRequests.begin(); it != upRequests.end();) {
                distance = abs(it->floor - currentFloor);
                seekCount += distance;
                currentFloor = it->floor;
                seekSequence.push_back(*it);
                it = upRequests.erase(it);
            }

            for (auto it = downRequests.begin(); it != downRequests.end();) {
                distance = abs(it->floor - currentFloor);
                seekCount += distance;
                currentFloor = it->floor;
                seekSequence.push_back(*it);
                it = downRequests.erase(it);
            }
        }

        cout << "Seek Sequence is : ";
        for (auto &request : seekSequence) {
            cout << request.floor <<" ";
        }
        cout<<endl;
        cout << "Total number of seek operations = " << seekCount << endl;
        cout<<"Now lift-id: "<<elevatorId<<" is at: "<<currentFloor<<endl;
        cout<<endl;
    }
};

class ElevatorSystem {
private:
    vector<Elevator> elevators;

public:
    ElevatorSystem(vector<Elevator> elevs) : elevators(elevs) {}

    void requestElevator(int elevatorIndex, int floor) {
        bool processed = false;
        for(Elevator elevator : elevators) {
            if(elevator.getIndex() == elevatorIndex) {
                elevators[elevatorIndex].handleRequest({floor});
                processed = true;
            }
        }
        if(!processed) {
            cout << "Invalid elevator index: "<<elevatorIndex<< endl;
        }
    }

    void executeElevators() {
        for (auto &elevator : elevators) {
            elevator.executeRequests();
        }
    }
};

int main() {
    // Create two Elevator objects
    Elevator elevator1(0);
    Elevator elevator2(1);

    // Initialize ElevatorSystem with the given elevators
    ElevatorSystem system({elevator1, elevator2});

    // Simulate elevator requests
    system.requestElevator(0, 5);
    system.requestElevator(0, 4);
    system.requestElevator(1, 3);
    system.requestElevator(1, 2);
    // Execute elevators' requests
    system.executeElevators();

    system.requestElevator(0, 6);
    system.requestElevator(0, 2);
    system.requestElevator(1, 2);
    system.requestElevator(1, 5);

        system.executeElevators();

    return 0;
}
