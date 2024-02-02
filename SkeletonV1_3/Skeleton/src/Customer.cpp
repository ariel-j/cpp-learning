#include <string>
#include <vector>
#include <iostream>
#include "../include/Customer.h"
using namespace std;

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders)
    : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId(vector<int>()){
}

const string& Customer::getName() const{
    return name;
}

int Customer::getId() const {
    return id;
}

int Customer::getCustomerDistance() const {
    return locationDistance;
}

int Customer::getMaxOrders() const {
    return maxOrders;
}

int Customer::getNumOrders() const {
    return ordersId.size();
}

bool Customer::canMakeOrder() const {
    return maxOrders > getNumOrders();
}

const vector<int>& Customer::getOrdersIds() const {
    return ordersId;
}

int Customer::addOrder(int orderId) {  //return OrderId if order was added successfully, -1 otherwise
    if(canMakeOrder()){
        ordersId.push_back(orderId);
        return orderId; 
    } 
    else {
        return -1; 
    }     
}
Customer::~Customer() {}


 SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders) : 
 Customer(id,name,locationDistance,maxOrders)
 {}

 SoldierCustomer* SoldierCustomer::clone()const {
    return new SoldierCustomer(*this);
}

CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders) : 
 Customer(id,name,locationDistance,maxOrders)
 {}

CivilianCustomer* CivilianCustomer::clone() const{
    return new CivilianCustomer(*this);
}
/*
int main(int argc, char** argv) {
    SoldierCustomer a(1, "abc", 10, 2);
    cout << a.getName() << endl;
    cout << a.getMaxOrders() << endl;
    cout << a.getNumOrders() << endl;
    cout << a.getCustomerDistance() << endl;
    cout << a.canMakeOrder() << endl;

    
    CivilianCustomer b(10, "asd", 24, 4);
    for(int i=0;i<10;i++){
        b.addOrder(i*10);
    }

    for (const auto &element : b.getOrdersIds()) {
        cout << element << " ";
    }
        cout << endl;

    return 0;  // Added return statement

}
*/  
        
