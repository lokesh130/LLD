#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <unistd.h> 

using namespace std;

class Vehicle {
private:
    string licensePlate;

public:
    Vehicle(const string& plate) : licensePlate(plate) {}

    const string& getLicensePlate() const {
        return licensePlate;
    }
};

class ParkingTicket {
private:
    Vehicle* vehicle;
    chrono::time_point<chrono::system_clock> parkTime;

public:
    ParkingTicket(Vehicle* v) : vehicle(v), parkTime(chrono::system_clock::now()) {}

    Vehicle* getVehicle() const {
        return vehicle;
    }

    chrono::time_point<chrono::system_clock> getParkTime() const {
        return parkTime;
    }
};

class ParkingSpace {
private:
    bool occupied;
    Vehicle* vehicle;
    int parkingSpaceId;

public:
    ParkingSpace(int id) : occupied(false), vehicle(nullptr), parkingSpaceId(id) {}

    bool isOccupied() const {
        return occupied;
    }

    void parkVehicle(Vehicle* v) {
        occupied = true;
        vehicle = v;
    }

    void removeVehicle() {
        occupied = false;
        vehicle = nullptr;
    }

    Vehicle* getVehicle() const {
        return vehicle;
    }

    int getParkingSpaceId() const {
        return parkingSpaceId;
    }
};

class ParkingLot {
private:
    vector<ParkingSpace*> spaces;

    double calculateCost(ParkingTicket* ticket) const {
        if (ticket) {
            auto currentTime = chrono::system_clock::now();
            auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - ticket->getParkTime());
            return elapsedTime.count() * 0.1;
        }
        return 0.0;
    }

public:
    ParkingLot(int numSpaces) {
        for (int i = 1; i <= numSpaces; ++i) {
            spaces.push_back(new ParkingSpace(i));
        }
    }

    ~ParkingLot() {
        for (auto space : spaces) {
            delete space;
        }
    }

    double exitVehicle(ParkingTicket* ticket) {
        if (ticket) {
            Vehicle* v = ticket->getVehicle();
            for (auto space : spaces) {
                if (space->isOccupied() && space->getVehicle() == v) {
                    space->removeVehicle();
                    double cost = calculateCost(ticket);
                    delete ticket;
                    return cost;
                }
            }
        }
        return 0.0;
    }

    ParkingTicket* parkVehicle(Vehicle* v) {
        for (auto space : spaces) {
            if (!space->isOccupied()) {
                space->parkVehicle(v);
                return new ParkingTicket(v);
            }
        }
        return nullptr;
    }

    void displayParkingLot() const {
        cout << "Parking Lot:" << endl;
        for (const auto space : spaces) {
            cout << "Parking Space ID: " << space->getParkingSpaceId() << " - Vehicle ID: ";
            if (space->isOccupied()) {
                cout << space->getVehicle()->getLicensePlate();
            } else {
                cout << "''";
            }
            cout << endl;
        }
    }
};

int main() {
    ParkingLot lot(10);

    Vehicle v1("ABC123");
    Vehicle v2("XYZ789");

    ParkingTicket* ticket1 = lot.parkVehicle(&v1);
    ParkingTicket* ticket2 = lot.parkVehicle(&v2);

    if (ticket1) {
        cout << "Vehicle " << ticket1->getVehicle()->getLicensePlate() << " parked successfully." << endl;
    } else {
        cout << "No available space for vehicle " << v1.getLicensePlate() << endl;
    }

    if (ticket2) {
        cout << "Vehicle " << ticket2->getVehicle()->getLicensePlate() << " parked successfully." << endl;
    } else {
        cout << "No available space for vehicle " << v2.getLicensePlate() << endl;
    }

    lot.displayParkingLot();
    sleep(5); // Introduce a delay of 5 seconds
    double cost1 = lot.exitVehicle(ticket1);
    cout << "Cost for vehicle " << v1.getLicensePlate() << ": $" << cost1 << endl;

    double cost2 = lot.exitVehicle(ticket2);
    cout << "Cost for vehicle " << v2.getLicensePlate() << ": $" << cost2 << endl;

    lot.displayParkingLot();

    return 0;
}
