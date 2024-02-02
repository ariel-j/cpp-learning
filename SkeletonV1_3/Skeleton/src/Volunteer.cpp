#include <string>
#include <vector>
#include "../include/Volunteer.h"
using namespace std;

//Volunteer class
Volunteer::Volunteer(int id, const string &name)
    : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id),name(name){}

int Volunteer::getId() const {
    return id;
}

const string& Volunteer::getName() const {
    return name;
}

int Volunteer::getActiveOrderId() const {
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const {
    return completedOrderId;
}

bool Volunteer::isBusy() const {
    return getActiveOrderId() != -1;
}

Volunteer::~Volunteer() {}
//CollectorVolunteer class
CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown):
     Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}

CollectorVolunteer* CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() {
    // If there is an order in place
    if(isBusy()) {
        if(decreaseCoolDown()) { // Move one step of time
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

int CollectorVolunteer:: getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown(){
    timeLeft--;
    return timeLeft==0;
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const {
   return !isBusy();
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    if(canTakeOrder(order)) {
        activeOrderId = order.getId();
        timeLeft = coolDown;
    }
}

string CollectorVolunteer::toString() const {
    return "VolunteerID: " + to_string(getId()) +
           "\nisBusy: " + (isBusy()?"true":"false") +
           "\nOrderId: " + ((activeOrderId == -1) ? "None" : to_string(activeOrderId)) +
           "\ntimeLeft: " + to_string(timeLeft) + 
           "\nordersLeft: No Limit";
}

//LimitedCollectorVolunteer
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders):
     CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders),ordersLeft(maxOrders) {}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    return hasOrdersLeft();
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order){
    // Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
    if(canTakeOrder(order)){
        CollectorVolunteer::acceptOrder(order);
        ordersLeft--;
    }
}

int LimitedCollectorVolunteer::getMaxOrders() const {
    return maxOrders;
}
        
int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const {
    return "VolunteerID: " + to_string(getId()) +
           "\nisBusy: " + (isBusy()?"true":"false") +
           "\nOrderId: " + ((activeOrderId == -1) ? "None" : to_string(activeOrderId)) +
           "\ntimeLeft: " + to_string(getTimeLeft()) + 
           "\nordersLeft: " + to_string(maxOrders);
}
//DriverVolunteer 
DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep): 
    Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

DriverVolunteer* DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}  

bool DriverVolunteer::decreaseDistanceLeft(){
     //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
    distanceLeft = distanceLeft-distancePerStep;
    return distanceLeft <= 0;
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const {
// Signal if the volunteer is not busy and the order is within the maxDistance
    return (!isBusy() && order.getDistance() <= maxDistance);
} 

void DriverVolunteer::acceptOrder(const Order &order) {
    // Assign distanceLeft to order's distance
    // Prepare for new order(Reset activeOrderId,DistanceLeft,OrdersLeft depends on the volunteer type)
    if(canTakeOrder(order)){
        distanceLeft = order.getDistance();
        activeOrderId = order.getId();
          
    } 
} 

void DriverVolunteer::step() {// Decrease distanceLeft by distancePerStep
    if(decreaseDistanceLeft()) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
        distanceLeft = 0;   
    }
} 

string DriverVolunteer::toString() const {
    return "VolunteerID: " + to_string(getId()) +
           "\nisBusy: " + (isBusy()?"true":"false") +
           "\nOrderId: " + ((activeOrderId == -1) ? "None" : to_string(activeOrderId)) +
           "\ndistanceLeft: " + to_string(distanceLeft) + 
           "\nordersLeft: No Limit";
}

//LimitedDriverVolunteer
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
    DriverVolunteer(id,name,maxDistance,distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
     // Signal if the volunteer is not busy, the order is within the maxDistance.
     return (!isBusy()) && (order.getDistance() <= getMaxDistance()) && (hasOrdersLeft());
}

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    // Assign distanceLeft to order's distance and decrease ordersLeft
    // Prepare for new order(Reset activeOrderId,DistanceLeft,OrdersLeft depends on the volunteer type)
    if(canTakeOrder(order)) {
        DriverVolunteer::acceptOrder(order);
        ordersLeft--;
    }
}
 
string LimitedDriverVolunteer::toString() const {
    return "VolunteerID: " + to_string(getId()) +
           "\nisBusy: " + (isBusy()?"true":"false") +
           "\nOrderId: " + ((activeOrderId == -1) ? "None" : to_string(activeOrderId)) +
           "\ndistanceLeft: " + to_string(getDistanceLeft()) + 
           "\nordersLeft: " + to_string(getNumOrdersLeft());
}

/*

int main(int argc, char** argv) {
    LimitedCollectorVolunteer ariel(111,"ariel",2,4);
    Order order(0,111,25);
    cout << ariel.getActiveOrderId() << endl;
    cout << ariel.getCompletedOrderId() << endl;
    cout << ariel.getCoolDown() << endl;
    cout << ariel.getId() << endl;
    cout << ariel.getName() << endl;
    cout << ariel.getTimeLeft() << endl;
    cout << ariel.getName() + "status:\n " + ariel.toString() << endl;
    cout << ariel.hasOrdersLeft() << endl;
    cout << ariel.canTakeOrder(order) << endl;
    ariel.acceptOrder(order);
    cout << ariel.hasOrdersLeft() << endl;
    cout << ariel.getMaxOrders() << endl;
    cout << ariel.getNumOrdersLeft() << endl;
    ariel.acceptOrder(order);
    ariel.acceptOrder(order);
    cout << ariel.getNumOrdersLeft() << endl;
    ariel.acceptOrder(order);
    cout << ariel.hasOrdersLeft() << endl;
    cout << ariel.getNumOrdersLeft() << endl;
    ariel.acceptOrder(order);

    DriverVolunteer ariel(111,"ariel",20,4);
    Order order(0,111,5);
    cout <<ariel.getActiveOrderId() <<endl;
    cout <<ariel.getDistancePerStep() <<endl;
    cout <<ariel.getId() <<endl;
    cout <<ariel.getMaxDistance() <<endl;
    cout <<ariel.getName() <<endl;
    cout <<ariel.toString() <<endl;
    cout << "------------------------------------------------------" <<endl;
    cout <<to_string(ariel.canTakeOrder(order)) + "can take order " <<endl;
    cout<< order.getDistance() << endl; 
    cout <<to_string(ariel.hasOrdersLeft()) + "has order left" <<endl;
    cout <<to_string(ariel.isBusy()) + "is busy" <<endl;
    ariel.acceptOrder(order);
    cout << "------------------------------------------------------" <<endl;
    cout <<ariel.toString() <<endl;
    cout << "------------------------------------------------------" <<endl;
    ariel.step();
    ariel.step();
    cout <<ariel.toString() <<endl;
    cout << "------------------------------------------------------" <<endl;
    cout <<to_string(ariel.hasOrdersLeft()) + "has order left" <<endl;
    DriverVolunteer yuval (222,"yuvi",20,4);
    Order order1(1,45687,1);
    DriverVolunteer* ariel_clone(ariel.clone());
    cout << ariel_clone->toString() << endl;
    cout << "------------------------------------------------------" <<endl;
    ariel.acceptOrder(order1);
    cout <<ariel.toString() <<endl;
    cout <<(*ariel_clone).toString() <<endl;
    cout << "------------------------------------------------------" <<endl;
    cout << "------------------------------------------------------" <<endl;
    

    LimitedDriverVolunteer yossi(1,"yossi",40, 2,3);
    cout <<"meet yossi:\n" << yossi.toString() << endl;
    
    cout << "------------------------------------------------------" <<endl;

    bool flag = true;
    //Methods
    if(yossi.getId() != 1) {flag = false; cout << "getId doesn't work" << endl;}
    if(yossi.getName() != "yossi") {flag = false; cout << "getname doesn't work" << endl;}
    if(yossi.getMaxDistance() != 40) {flag = false; cout << "getMaxDistance doesn't work" << endl;}
    if(yossi.getDistancePerStep() != 2) {flag = false; cout << "getDistancePerStep doesn't work" << endl;}
    if(yossi.getMaxOrders() != 3) {flag = false; cout << "getMaxOrder doesn't work" << endl;}
    if(yossi.getNumOrdersLeft() != 3) {flag = false; cout << "getNumOrdersLeft doesn't work" << endl;}
    if(yossi.hasOrdersLeft() == false) {flag = false; cout << "hasOrdersLeft doesn't work" << endl;}
    if(yossi.canTakeOrder(order) == false) {flag = false; cout << "canTakeOrder doesn't work" << endl;}
    //New order
    yossi.acceptOrder(order);
    if(yossi.getNumOrdersLeft() != 2 || yossi.getActiveOrderId() != order.getId() || yossi.getDistanceLeft() != order.getDistance()) {flag = false; cout << "accept order doesn't work" << endl;}
    cout <<"new order:\n" << yossi.toString() << endl;
    //Step
    yossi.step();
    if(yossi.getDistanceLeft() != 3 || yossi.getActiveOrderId() != order.getId() || yossi.getCompletedOrderId() != NO_ORDER) {flag = false; cout << "step doesn't work" << endl;}
    //Finish order
    yossi.step();
    yossi.step();
    if(yossi.getDistanceLeft() != 0 || yossi.getCompletedOrderId() != order.getId() || yossi.getActiveOrderId() != NO_ORDER) {flag = false; cout << "finish step doesn't work" << endl;}
    
    cout << "finish 'new order' " << yossi.toString() << endl;
    cout << to_string(flag) << endl;
    return 0;
}*/



    