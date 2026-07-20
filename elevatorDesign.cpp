#include<bits/stdc++.h>
using namespace std;
enum class Direction{
    UP,
    DOWN
};


class Display{
    private:
    int displayId;
    int floorNumber;
    Direction direction;
    public:
    Display(int id, int floor, Direction dir){
        this->displayId=id;
        this->floorNumber=floor;
        this->direction=dir;
    }
    void showFloor(){
        cout<<"Display "<<displayId<<" showing floor "<<floorNumber<<" direction "<<(direction==Direction::UP?"UP":"DOWN")<<endl;
    }
    void goToFloor(int floor, Direction dir){
        floorNumber=floor;
        direction=dir;
        cout<<"Display "<<displayId<<" going to floor "<<floor<<" direction "<<(dir==Direction::UP?"UP":"DOWN")<<endl;
    }
};
class InternalButton;
class Elevater{
    private:
    int elevatorId;
    int currentFloor;
    Direction direction;
    Display* display;
    InternalButton* internalButton;
    public:
    Elevater(int id, int floor, Direction dir, InternalButton* button){
        this->elevatorId=id;
        this->currentFloor=floor;
        this->direction=dir;
        this->display=new Display(1, floor, dir); // Assuming a simple display initialization
        this->internalButton=button;
    }
    int getElevatorId(){ return elevatorId; }
    int getCurrentFloor(){ return currentFloor; }
    Direction getDirection(){ return direction; }
    void goToFloor(int floor, Direction dir){
        display->showFloor();
        display->goToFloor(floor, dir);
        currentFloor=floor;
        direction=dir;
        display->showFloor();
    }

};

class Controller{
    private:
    int contollerId;
    Display* display;
    Elevater* elevator;
    priority_queue<int, vector<int>, greater<int>> upQueue; // Min-heap for UP direction
    priority_queue<int> downQueue; // Max-heap for DOWN direction
    public:
    Controller(int id, Display* disp, Elevater* elev){
        this->contollerId=id;
        this->display=disp;
        this->elevator=elev;
    }
    void requestElevator(int floor){
        // algorrith to add it to the min hep or max heap based on the direction and floor number
        if(floor>elevator->getCurrentFloor()){
            upQueue.push(floor);
        }else{
            downQueue.push(floor);      
        }
    }
    void processRequests(){
        // LOOK: sweep up serving all up-requests, then sweep down
        while(!upQueue.empty()){
            int f=upQueue.top(); upQueue.pop();
            elevator->goToFloor(f, Direction::UP);
        }
        while(!downQueue.empty()){
            int f=downQueue.top(); downQueue.pop();
            elevator->goToFloor(f, Direction::DOWN);
        }
    }
};

class InternalButton{
    protected:
    int buttonId;
    int floorNumber;
    Controller* controller;
    public:
    InternalButton(int id, int floor, Controller* ctrl){
        this->buttonId=id;
        this->floorNumber=floor;
        this->controller=ctrl;
    }
    void pressButton(){
        controller->requestElevator(floorNumber); // Assuming a simple request for up direction
    }
};

class Dispatcher{
    private:
    int dispatcherId;
    vector<Controller*> controllers;
    public:
    Dispatcher(int id, vector<Controller*> ctrls){
        this->dispatcherId=id;
        this->controllers=ctrls;
    }
    void assignElevator(int floor, Direction dir){
        // logic to assign the best elevator based on the current state of elevators
        for(Controller* ctrl: controllers){
            ctrl->requestElevator(floor);
        }
    }
};

class ExternalButton{
    protected:
    int buttonId;
    Direction direction;
    Dispatcher* dispatcher;
    int floorNumber;
    public:
    ExternalButton(int id, Direction dir, int floor, Dispatcher* disp){
        this->buttonId=id;
        this->direction=dir;
        this->floorNumber=floor;
        this->dispatcher=disp;
    }
    void pressButton(){
        // should call the dispatcher to assign an elevator to this floor
        // go to dispatcher
        dispatcher->assignElevator(floorNumber, direction);
    }
};
int main(){
    Elevater* elevator=new Elevater(1, 0, Direction::UP, nullptr);
    Display* display=new Display(1, 0, Direction::UP);
    Controller* controller=new Controller(1, display, elevator);
    Dispatcher* dispatcher=new Dispatcher(1, {controller});

    // Round 1: external calls from floors 3 and 7 -> car sweeps up
    (new ExternalButton(1, Direction::UP, 3, dispatcher))->pressButton();
    (new ExternalButton(2, Direction::UP, 7, dispatcher))->pressButton();
    controller->processRequests();          // serves 3, then 7 (now at floor 7)

    // Round 2: rider inside presses floor 2 -> car sweeps down
    (new InternalButton(1, 2, controller))->pressButton();
    controller->processRequests();          // serves 2
    return 0;
}
