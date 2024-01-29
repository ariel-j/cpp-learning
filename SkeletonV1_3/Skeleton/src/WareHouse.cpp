#include <iostream>
#include <fstream>
#include "../include/WareHouse.h"
#include "../include/Action.h"
using namespace std;
#include <sstream>


WareHouse::WareHouse(const string &configFilePath)
:  isOpen(false), customerCounter(-1), volunteerCounter(-1),orderCounter(-1), file(configFilePath)  {Initiatefile();}

void WareHouse::Initiatefile(){
    string line;
    vector<string> words;
    string word;
    ifstream MyReadFile(file);
    while (getline (MyReadFile, line)) {
        words.clear();     
        istringstream iss(line);
        iss >> word;
        if(word == "customer") {
            for(int i = 0; i < 4; i++) {
                iss >> word;
                words.push_back(word);
            } 
            if (words[1] == "soldier") {
                customers.push_back(new SoldierCustomer(getNewcustomerID(), words[0], stoi(words[2]),stoi(words[3])));
                }
            else {
                customers.push_back(new CivilianCustomer(getNewcustomerID(), words[0], stoi(words[2]),stoi(words[3])));
            }     
        }
        if(word == "volunteer") {
            for(int i = 0; i < 5; i++) {
                iss >> word;
                words.push_back(word);
            } 
            if (words[1] == "collector") {
                volunteers.push_back(new CollectorVolunteer(getNewvolunteerID(), words[0],stoi(words[2])));
                }
            else if(words[1] == "LimitedCollectorVolunteer") {
                volunteers.push_back(new LimitedCollectorVolunteer(getNewvolunteerID(), words[0], stoi(words[2]),stoi(words[3])));
            }
            else if (words[1] == "driver") {
                volunteers.push_back(new DriverVolunteer(getNewvolunteerID(), words[0],stoi(words[2]),stoi(words[3])));
            }
             else if(words[1] == "LimitedDriverVolunteer") {
                volunteers.push_back(new LimitedDriverVolunteer(getNewvolunteerID(), words[0], stoi(words[2]),stoi(words[3]), stoi(words[4])));
            }     
        }
    }
    MyReadFile.close();


}


void WareHouse::start(){
    open();
    cout << "Warehouse is open!" << endl;
    while(isOpen){
        

    }
}

void WareHouse::addOrder(Order* order){
        pendingOrders.push_back(order);
}

void WareHouse::addCustomer(Customer* customer){
    customers.push_back(customer);
}

int WareHouse::getNewcustomerID() {
    customerCounter++;
    return customerCounter;
}

int WareHouse::getNewvolunteerID() {
    volunteerCounter++;
    return volunteerCounter;
}

int WareHouse::getNewOrderId(){
    orderCounter++;
    return orderCounter;
}

vector<Order*>& WareHouse::getPendingOrders(){
    return pendingOrders;
}

vector<Order*>& WareHouse::getInProcessOrders(){
    return inProcessOrders;
}

vector<Order*>& WareHouse::getCompletedOrders(){
    return completedOrders;
}

vector<Customer*>& WareHouse::getCustomers(){
    return customers;
}
vector<Volunteer*>& WareHouse::getVolunteers(){
    return volunteers;
}

void WareHouse::operator=(const WareHouse &other){
    if(&other != this) {
        
    }

}


int main(int argc, char** argv) {
    cout << "Entering main:" << endl;
    string configurationFile = argv[1];
    cout << configurationFile << endl;
    WareHouse wareHouse(configurationFile);
    wareHouse.start();
    return 0;
}



/*
    for (Volunteer* v : volunteers) {
        cout << (*v).toString() << endl;
    }
    for (Customer* c : customers) {
        cout << (*c).toString() << endl;
    }*/