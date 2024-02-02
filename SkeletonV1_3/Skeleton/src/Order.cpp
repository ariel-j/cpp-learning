#include <string>
#include <vector>
#include <iostream>
#include "../include/Order.h"
using namespace std;

Order::Order(int id, int customerId, int distance)
    : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {
}

Order* Order::clone() const{
    return new Order(*this);
}

int Order::getId() const {
    return id;
}

int Order::getCustomerId() const {
    return customerId;
}

void Order::setStatus(OrderStatus status) {
    this->status = status;
}

void Order::setCollectorId(int collectorId) {
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId) {
    this->driverId = driverId;
}

int Order::getCollectorId() const {
    return collectorId;
}

int Order::getDriverId() const {
    return driverId;
}

OrderStatus Order::getStatus() const {
    return status;
}

int Order::getDistance() const{
    return distance;
}

string Order::switchStatusToString() const{
    string statusStr;
    switch (status) {
        case OrderStatus::PENDING:
            statusStr = "PENDING";
            break;
        case OrderStatus::COLLECTING:
            statusStr = "COLLECTING";
            break;
        case OrderStatus::DELIVERING:
            statusStr = "DELIVERING";
            break;
        case OrderStatus::COMPLETED:
            statusStr = "COMPLETED";
            break;
    }
    return statusStr;
}


const string Order::toString() const{
    string ans = 
    "Order ID: " + to_string(id) + 
    "\nStatus: " + switchStatusToString() + 
    "\nCustomer ID: " + to_string(customerId);

    //Collector Id
    ans = (getCollectorId() == -1) ? ans + "\nCollector ID: " + "None" : ans + ",\nCollector ID: " + to_string(collectorId);
    // Driver Id
    ans = (getDriverId() == -1) ? ans + "\nDriver ID: " + "None" : ans + "\nDriver ID: " + to_string(driverId);
    
    return ans;
}


/*int main(int argc, char** argv) {
    Order a(1,2,4);
    cout << a.getId() << endl;
    cout << a.getCustomerId() << endl;
    cout << a.getCollectorId() << endl;
    cout << a.getDriverId() << endl;
    cout << "Order stattos:" + a.toString() << endl;


    a.setStatus(OrderStatus::COMPLETED);
    cout << (a.getStatus() != OrderStatus::COMPLETED) << endl;
    a.setDriverId(100);
    a.setCollectorId(2548);
    cout << a.getCollectorId() << endl;
    cout << a.getDriverId() << endl;
    cout << a.toString() << endl;

    return 0;
}

*/