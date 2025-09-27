#include <bits/stdc++.h>
using namespace std;

enum class Direction {
    Upwards,
    Downwards
};

enum class Status {
    Moving,
    Idle  // Fixed typo: "Idol" -> "Idle"
};

// Forward declarations
class ElevatorController;

// Base Dispatcher class
class Dispatcher {
protected:
    ElevatorController* elevatorController;
    
public:
    Dispatcher(ElevatorController* controller) : elevatorController(controller) {}
    virtual ~Dispatcher() = default;
    virtual void addDestination(int floor) = 0;  // Pure virtual function
};

// Internal Dispatcher (for buttons inside elevator)
class InternalDispatcher : public Dispatcher {
private:
    queue<int> internalRequests;
    
public:
    InternalDispatcher(ElevatorController* controller) : Dispatcher(controller) {}
    
    void addDestination(int floor) override {
        cout << "Internal request for floor: " << floor << endl;
        internalRequests.push(floor);
        // Process immediately since it's from inside the elevator
        processRequest(floor);
    }
    
    void processRequest(int floor) {
        // Add to elevator controller's queue
        // elevatorController->addInternalRequest(floor);
    }
    
    bool hasRequests() {
        return !internalRequests.empty();
    }
    
    int getNextRequest() {
        if (!internalRequests.empty()) {
            int request = internalRequests.front();
            internalRequests.pop();
            return request;
        }
        return -1;
    }
};

// External Dispatcher (for floor buttons)
class ExternalDispatcher : public Dispatcher {
private:
    queue<pair<int, Direction>> externalRequests;  // floor and direction
    
public:
    ExternalDispatcher(ElevatorController* controller) : Dispatcher(controller) {}
    
    void addDestination(int floor) override {
        // For external, we need direction too
        addRequest(floor, Direction::Upwards);  // Default direction
    }
    
    void addRequest(int floor, Direction direction) {
        cout << "External request for floor: " << floor 
             << " in direction: " << (direction == Direction::Upwards ? "Up" : "Down") << endl;
        externalRequests.push({floor, direction});
        processRequest(floor, direction);
    }
    
    void processRequest(int floor, Direction direction) {
        // Add to elevator controller's queue
        // elevatorController->addExternalRequest(floor, direction);
    }
    
    bool hasRequests() {
        return !externalRequests.empty();
    }
    
    pair<int, Direction> getNextRequest() {
        if (!externalRequests.empty()) {
            auto request = externalRequests.front();
            externalRequests.pop();
            return request;
        }
        return {-1, Direction::Upwards};
    }
};

class Display {
public:
    void show(int currFloor, Direction direction) {
        cout << "Current floor: " << currFloor << endl;
        string dir = direction == Direction::Upwards ? "Upwards" : "Downwards";
        cout << "Moving: " << dir << endl;
    }
};

class InternButtonPanel {
private:
    InternalDispatcher* dispatcher;
    
public:
    InternButtonPanel(ElevatorController* controller) {
        this->dispatcher = new InternalDispatcher(controller);
    }
    
    ~InternButtonPanel() {
        delete dispatcher;
    }
    
    void pressButton(int destination) {
        dispatcher->addDestination(destination);
    }
    
    InternalDispatcher* getDispatcher() {
        return dispatcher;
    }
};

class ExternalButtonPanel {
private:
    ExternalDispatcher* dispatcher;
    int floorNumber;
    
public:
    ExternalButtonPanel(int floor, ElevatorController* controller) : floorNumber(floor) {
        this->dispatcher = new ExternalDispatcher(controller);
    }
    
    ~ExternalButtonPanel() {
        delete dispatcher;
    }
    
    void pressUpButton() {
        dispatcher->addRequest(floorNumber, Direction::Upwards);
    }
    
    void pressDownButton() {
        dispatcher->addRequest(floorNumber, Direction::Downwards);
    }
    
    ExternalDispatcher* getDispatcher() {
        return dispatcher;
    }
};

class ElevatorDoor {
private:
    bool isOpen;
    
public:
    ElevatorDoor() : isOpen(false) {}
    
    void open() {
        isOpen = true;
        cout << "Door opened" << endl;
    }
    
    void close() {
        isOpen = false;
        cout << "Door closed" << endl;
    }
    
    bool getStatus() {
        return isOpen;
    }
};

// Forward declaration for ElevatorCar
class ElevatorCar;

class ElevatorController {
private:
    // LOOK algorithm implementation
    priority_queue<int, vector<int>, greater<int>> upRequests;   // Min heap for upward
    priority_queue<int> downRequests;  // Max heap for downward
    ElevatorCar* elevator;
    Direction currentDirection;
    
public:
    ElevatorController(ElevatorCar* elev) : elevator(elev), currentDirection(Direction::Upwards) {}
    
    void addInternalRequest(int floor) {
        cout << "Controller: Adding internal request for floor " << floor << endl;
        if (currentDirection == Direction::Upwards) {
            upRequests.push(floor);
        } else {
            downRequests.push(floor);
        }
    }
    
    void addExternalRequest(int floor, Direction direction) {
        cout << "Controller: Adding external request for floor " << floor << endl;
        if (direction == Direction::Upwards) {
            upRequests.push(floor);
        } else {
            downRequests.push(floor);
        }
    }
    
    int getNextDestination(int currentFloor) {
        if (currentDirection == Direction::Upwards && !upRequests.empty()) {
            int nextFloor = upRequests.top();
            if (nextFloor >= currentFloor) {
                upRequests.pop();
                return nextFloor;
            }
        }
        
        if (currentDirection == Direction::Downwards && !downRequests.empty()) {
            int nextFloor = downRequests.top();
            if (nextFloor <= currentFloor) {
                downRequests.pop();
                return nextFloor;
            }
        }
        
        // Switch direction if needed
        if (currentDirection == Direction::Upwards && upRequests.empty() && !downRequests.empty()) {
            currentDirection = Direction::Downwards;
            return getNextDestination(currentFloor);
        } else if (currentDirection == Direction::Downwards && downRequests.empty() && !upRequests.empty()) {
            currentDirection = Direction::Upwards;
            return getNextDestination(currentFloor);
        }
        
        return -1; // No requests
    }
    
    bool hasRequests() {
        return !upRequests.empty() || !downRequests.empty();
    }
    
    Direction getCurrentDirection() {
        return currentDirection;
    }
};

class ElevatorCar {
private:
    int id;
    Direction direction;
    Status status;
    Display* display;
    InternButtonPanel* buttons;
    int currentFloor;
    ElevatorDoor* door;
    ElevatorController* controller;
    
public:
    ElevatorCar(int id, Direction direction = Direction::Upwards, Status status = Status::Idle) 
        : id(id), direction(direction), status(status), currentFloor(0) {
        this->display = new Display();
        this->controller = new ElevatorController(this);
        this->buttons = new InternButtonPanel(controller);
        this->door = new ElevatorDoor();
    }
    
    ~ElevatorCar() {
        delete display;
        delete buttons;
        delete door;
        delete controller;
    }
    
    void showDisplay() {
        display->show(currentFloor, direction);
    }
    
    void pressButton(int destination) {
        buttons->pressButton(destination);
        controller->addInternalRequest(destination);
    }
    
    void moveElevator(int destination, Direction direction) {
        this->direction = direction;
        status = Status::Moving;
        
        cout << "\nMoving to floor " << destination << endl;
        
        while (currentFloor != destination) {
            currentFloor = currentFloor + (direction == Direction::Downwards ? -1 : 1);
            showDisplay();
            // Simulate movement time
            // this_thread::sleep_for(chrono::milliseconds(500));
        }
        
        status = Status::Idle;
        door->open();
        cout << "Arrived at floor " << destination << endl;
        // this_thread::sleep_for(chrono::milliseconds(1000));
        door->close();
    }
    
    void processRequests() {
        while (controller->hasRequests()) {
            int nextFloor = controller->getNextDestination(currentFloor);
            if (nextFloor != -1) {
                Direction moveDirection = (nextFloor > currentFloor) ? Direction::Upwards : Direction::Downwards;
                moveElevator(nextFloor, moveDirection);
            }
        }
    }
     
    int getCurrentFloor() {
        return currentFloor;
    }
    
    ElevatorController* getController() {
        return controller;
    }
};

// Building class to manage everything
class Building {
private:
    vector<ElevatorCar*> elevators;
    vector<ExternalButtonPanel*> floorPanels;
    int totalFloors;
    
public:
    Building(int floors, int numElevators) : totalFloors(floors) {
        // Create elevators
        for (int i = 0; i < numElevators; i++) {
            elevators.push_back(new ElevatorCar(i + 1));
        }
        
        // Create floor panels (external buttons)
        for (int i = 0; i < floors; i++) {
            floorPanels.push_back(new ExternalButtonPanel(i, elevators[0]->getController()));
        }
    }
    
    ~Building() {
        for (auto elevator : elevators) {
            delete elevator;
        }
        for (auto panel : floorPanels) {
            delete panel;
        }
    }
    
    void callElevator(int floor, Direction direction) {
        cout << "\nCalling elevator to floor " << floor << endl;
        if (direction == Direction::Upwards) {
            floorPanels[floor]->pressUpButton();
        } else {
            floorPanels[floor]->pressDownButton();
        }
        
        // Find best elevator (simplified - just use first one)
        elevators[0]->getController()->addExternalRequest(floor, direction);
    }
    
    void simulateElevatorMovement() {
        for (auto elevator : elevators) {
            elevator->processRequests();
        }
    }
};

int main() {
    // Create a 10-floor building with 1 elevator
    Building building(10, 1);
    
    cout << "=== Elevator System Simulation ===" << endl;
    
    // Simulate some requests
    building.callElevator(5, Direction::Upwards);
    building.callElevator(8, Direction::Upwards);
    building.callElevator(3, Direction::Downwards);
    
    // Process all requests
    building.simulateElevatorMovement();
    
    return 0;
}