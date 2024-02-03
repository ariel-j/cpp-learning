#include <string>
#include <vector>
#include "../include/Action.h"
using namespace std;

extern WareHouse* backup;

//BaseAction
BaseAction::BaseAction(): errorMsg(""), status(ActionStatus::COMPLETED){}

ActionStatus BaseAction::getStatus() const{
    return status;
}

BaseAction::~BaseAction() {}

void  BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg){
    this->errorMsg = errorMsg;
    status = ActionStatus::ERROR;
}

string BaseAction::getErrorMsg() const{
    return errorMsg;
}

string BaseAction::statusToString() const{
    string strStatus;
    switch (status)
    {
    case ActionStatus::COMPLETED:
        strStatus = "COMPLETED";
        break;
    
    case ActionStatus::ERROR:
        strStatus = "ERROR";
        break;
    }
    return strStatus;
}

//SimulateStep
SimulateStep::SimulateStep(int numOfSteps): numOfSteps(numOfSteps){}

void SimulateStep::act(WareHouse &wareHouse) {
    for(int i = 0; i < numOfSteps; i++) {
        cout << "inside SimulateStep" << endl;
        // Step 1 - Itirate the pending orders
        int j=0;
        bool didPop;
        //Getting the order's vectors
        vector<Volunteer*> &volunteers = wareHouse.getVolunteers();
        vector<Order*> &pending_orders = wareHouse.getPendingOrders();
        vector<Order*> &in_Pro_ord = wareHouse.getInProcessOrders();
        vector<Order*> &completed_Orders = wareHouse.getCompletedOrders();
        //looping through the orders
        vector<Order*>::iterator iterOrder = pending_orders.begin();
        while(iterOrder != pending_orders.end()) {
            Order* order = *iterOrder;
            didPop = false;
            cout <<"orderid: " << order->getId();
            if(order->getStatus() == OrderStatus::PENDING) {
                cout << "|| sending to collector|| " ;
                for(Volunteer* vol:volunteers){
                    if (typeid(*vol) == typeid(CollectorVolunteer) || typeid(*vol) == typeid(LimitedCollectorVolunteer)) {
                        //checks if the volunteer is free to accepet the order and sets the order and volunteeer idws
                        if(vol->canTakeOrder(*order)) {
                            // Set the order
                            order->setCollectorId(vol->getId());
                            order->setStatus(OrderStatus::COLLECTING);
                            // Set the collector
                            cout << " we set the collector ||";
                            vol->acceptOrder(*order);
                            cout << " vol accepeted order ||";
                            in_Pro_ord.push_back(order); // pushs the order to the in_process
                            cout << " order in inprocess vector ||";
                            wareHouse.popOrder(j, pending_orders);
                            didPop = true;
                            cout << " poping order from pending  vector || didpop: " << didPop <<endl;
                            break;
                        }
                    }  
                }
            }
            
            else {
                cout << " send to driver ";
                //send to driver
                int size = volunteers.size();
                for(int i = 0; i < size; i++) {
                    didPop = false;
                    if((typeid(*volunteers[i]) == typeid(DriverVolunteer) || typeid(*volunteers[i]) == typeid(LimitedDriverVolunteer)) && volunteers[i]->canTakeOrder(*order)) {
                        cout << "|| were in the if ||" << endl;
                        // Set the driver
                        volunteers[i]->acceptOrder(*order);
                        cout << " aceepting order driver ||";
                        // Set the order
                        cout << " seting driver id ||" ;
                        order->setDriverId(volunteers[i]->getId());
                        cout << " setting order status ||" ;
                        order->setStatus(OrderStatus::DELIVERING);
                        // Update the order vector
                        cout << " pushing in progress order ||";
                        in_Pro_ord.push_back(order);
                        cout << " poping order ||";
                        wareHouse.popOrder(j, pending_orders);
                        didPop = true;
                        break;
                    }
                }
            }
            if(!didPop) {iterOrder++; j++;}
        }
        cout << "end stage 1" <<endl;

        // Step 2, 3, 4:
        int size = volunteers.size();
        for(int i = 0; i < size; i++){  
            cout << "step 2" << endl;   
            cout << "current volunteer:\n" << volunteers[i]->toString() << endl; 
            int oId = volunteers[i]->getActiveOrderId(); 
            volunteers[i]->step(); // Step 2
            if(oId != -1 && oId == volunteers[i]->getCompletedOrderId()) {                                    // step 3: Check if volunteer finished the order 
                cout << "the volunteer finished the order" << endl;
                Order& o = wareHouse.getOrder(oId);                                       
            
                if(o.getStatus()==OrderStatus::COLLECTING){pending_orders.push_back(&o);} // reorganize accordingly:  place the order in right vector   
                else {                                                                    
                    completed_Orders.push_back(&o);
                    o.setStatus(OrderStatus::COMPLETED);
                }
                wareHouse.removeOrderByValue(in_Pro_ord,o);                                          // erase the order from in proccess orders vector
                
                if(!volunteers[i]->hasOrdersLeft()) {wareHouse.deleteVolunteer(volunteers[i], i); size--;}                  // step 4: remove any limited volunteer that reahces his maximum               
            }           
        }
    }   
}

std::string SimulateStep::toString() const {return "step";}

SimulateStep* SimulateStep::clone() const {return new SimulateStep(*this);}

//addorder
AddOrder::AddOrder(int id) : customerId(id) {}

void AddOrder::act(WareHouse &wareHouse) {
    Customer* customer = &(wareHouse.getCustomer(customerId));
    if(customer->getId() != -1 && customer->canMakeOrder()) {
        int orderId = wareHouse.getNewOrderId();
        wareHouse.addOrder(new Order(orderId,customerId, customer->getCustomerDistance()));
        customer->addOrder(orderId);
    }
    else {
        error("Cannot place this order");
        cout << getErrorMsg() << endl;
    }
}

string AddOrder::toString() const {return "Order " + to_string(customerId) + " " + statusToString();}

AddOrder* AddOrder::clone() const {return new AddOrder(*this);}

//addcustomer
AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders)
: customerName(customerName), customerType(customerType == "soldier" ? CustomerType::Soldier : CustomerType::Civilian), distance(distance), maxOrders(maxOrders) {};

void AddCustomer::act(WareHouse &wareHouse) {
    Customer* customer;
    if(customerType == CustomerType::Soldier) {
        customer = new SoldierCustomer(wareHouse.getNewcustomerID(), customerName, distance, maxOrders);
    }
    else {
        customer = new CivilianCustomer(wareHouse.getNewcustomerID(), customerName, distance, maxOrders);
    }
    wareHouse.addCustomer(customer);
}
string AddCustomer::customerToString() const{
    string strCustomer;
    switch (customerType)
    {
    case CustomerType::Soldier:
        strCustomer = "soldier";
        break;
    
    case CustomerType::Civilian:
        strCustomer = "civilian";
        break;
    }
    return strCustomer;
}

string AddCustomer::toString() const {return "Customer " + customerName + " " + customerToString() + " " + to_string(distance) + " " + to_string(maxOrders) + " " + statusToString();}

AddCustomer* AddCustomer::clone() const {return new AddCustomer(*this);}

//PrintOrderStatus
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    Order order = wareHouse.getOrder(orderId);
    if(order.getId() != -1) {
        cout << order.toString() << endl;
        wareHouse.addAction(this);
    }
    else {
        error("Order doesn't exist");
        cout << getErrorMsg() << endl;
    }
}

PrintOrderStatus* PrintOrderStatus::clone() const {return new PrintOrderStatus(*this);}

string PrintOrderStatus::toString() const {return "orderStatus " + to_string(orderId) + " " + statusToString();} 

// PrintCustomerStatus
PrintCustomerStatus::PrintCustomerStatus(int customerId): customerId(customerId){};

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    Customer* customer = &(wareHouse.getCustomer(customerId));
    if(customer->getId() != -1) {
       cout << "CustomerID: " + to_string(customer->getId())<< endl;
        for (int O : customer->getOrdersIds()) {
            cout << "OrderId:" << O << endl;
            cout << "OrderStatus:"+ wareHouse.getOrder(O).switchStatusToString() << endl;
        }
        cout << "numOrdersLeft:" +to_string(customer->getMaxOrders()-customer->getNumOrders()) <<endl;
    }
    else {
        error("customer doesn't exist");
        cout << getErrorMsg() << endl;
    }    
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {return new PrintCustomerStatus(*this);}

string PrintCustomerStatus::toString() const {return "customerStatus " + to_string(customerId) + " " + statusToString();}

//PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int volunteerId): volunteerId(volunteerId){};

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    Volunteer* volunteer = &wareHouse.getVolunteer(volunteerId);
    if(volunteer->getId() != -1) {
        cout << volunteer->toString() << endl;
    }
    else {
        error("Volunteer doesn’t exist");
        cout << getErrorMsg() << endl;
    }
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {return new PrintVolunteerStatus(*this);}

string PrintVolunteerStatus::toString() const {return "volunteerStatus " + to_string(volunteerId) + " " + statusToString();}


//PrintActionsLog
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse) {
    for (BaseAction* A : wareHouse.getActions()) {
        cout << (*A).toString() << endl;
    }
}

PrintActionsLog* PrintActionsLog::clone() const {return new PrintActionsLog(*this);}

string PrintActionsLog::toString() const {return "log " + statusToString();}

//close
Close::Close(){}

void Close::act(WareHouse &wareHouse) {
    for(Order* p : wareHouse.getPendingOrders()){
        cout << "OrderID: " + to_string (p->getId()) + ", CustomerID" + to_string(p->getCustomerId()) +", OrderStatus: " + p->switchStatusToString();
    }
    for(Order* p : wareHouse.getInProcessOrders()){
        cout << "OrderID: " + to_string (p->getId()) + ", CustomerID" + to_string(p->getCustomerId()) +", OrderStatus: " + p->switchStatusToString();
    }
    for(Order* p : wareHouse.getCompletedOrders()){
        cout << "OrderID: " + to_string (p->getId()) + ", CustomerID" + to_string(p->getCustomerId()) +", OrderStatus: " + p->switchStatusToString();
    }
    wareHouse.close(); // Changes isOpen status and exit the loop and finishes the program(free all memory as well)
}

Close* Close::clone() const {return new Close(*this);}

string Close::toString() const {return "";} 

//backup
BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse) {
    delete(backup);
    backup = new WareHouse(wareHouse);

}

BackupWareHouse* BackupWareHouse::clone() const {return new BackupWareHouse(*this);}

string BackupWareHouse::toString() const {return "backup " + statusToString();}

//restore
RestoreWareHouse::RestoreWareHouse(){}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    wareHouse = *backup;
}

RestoreWareHouse* RestoreWareHouse::clone() const {return new RestoreWareHouse(*this); }

string RestoreWareHouse::toString() const { return "restore " + statusToString();}
