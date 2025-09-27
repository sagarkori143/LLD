#include<bits/stdc++.h>
using namespace std;

// Base Vehicle class with virtual methods
class Vehicle {
public:
    // Virtual destructor for proper cleanup
    virtual ~Vehicle() = default;
    
    // Pure virtual function - makes Vehicle abstract
    virtual void run() = 0;
    
    // Virtual function for getting vehicle type
    virtual string getType() = 0;
};

// Maruti implementation
class Maruti: public Vehicle {
public:
    void run() override {
        cout << "Maruti lere road paad denge chore!" << endl; 
    }
    
    string getType() override {
        return "Maruti";
    }
    
    // Destructor
    ~Maruti() {
        cout << "Maruti destroyed" << endl;
    }
};

// BMW implementation  
class BMW: public Vehicle {
public:
    void run() override {
        cout << "BMW hai mitter" << endl;
    }
    
    string getType() override {
        return "BMW";
    }
    
    // Destructor
    ~BMW() {
        cout << "BMW destroyed" << endl;
    }
};

// Factory class
class Factory {
public:
    // Return pointer to Vehicle (proper polymorphism)
    Vehicle* giveVehicle(string type) {
        if(type == "maruti" || type == "Maruti") {
            return new Maruti();
        }
        else if(type == "bmw" || type == "BMW") {
            return new BMW();
        }
        else {
            cout << "Unknown vehicle type: " << type << endl;
            return nullptr;
        }
    }
    
    // Factory method with smart pointer (modern C++)
    shared_ptr<Vehicle> giveVehicleSmartPtr(string type) {
        if(type == "maruti" || type == "Maruti") {
            return make_shared<Maruti>();
        }
        else if(type == "bmw" || type == "BMW") {
            return make_shared<BMW>();
        }
        else {
            cout << "Unknown vehicle type: " << type << endl;
            return nullptr;
        }
    }
};

// Demo function
void demonstrateFactoryPattern() {
    cout << "=== Factory Pattern Demonstration ===" << endl;
    
    Factory factory;
    
    // Test with raw pointers
    cout << "\n--- Using Raw Pointers ---" << endl;
    Vehicle* maruti = factory.giveVehicle("maruti");
    Vehicle* bmw = factory.giveVehicle("BMW");
    
    if(maruti) {
        cout << "Created: " << maruti->getType() << endl;
        maruti->run();
    }
    
    if(bmw) {
        cout << "Created: " << bmw->getType() << endl;
        bmw->run();
    }
    
    // Clean up raw pointers
    delete maruti;
    delete bmw;
    
    // Test with smart pointers (recommended)
    cout << "\n--- Using Smart Pointers ---" << endl;
    auto smartMaruti = factory.giveVehicleSmartPtr("Maruti");
    auto smartBMW = factory.giveVehicleSmartPtr("bmw");
    
    if(smartMaruti) {
        cout << "Created: " << smartMaruti->getType() << endl;
        smartMaruti->run();
    }
    
    if(smartBMW) {
        cout << "Created: " << smartBMW->getType() << endl;
        smartBMW->run();
    }
    
    // Smart pointers automatically clean up
    cout << "\nSmart pointers will automatically clean up..." << endl;
}

int main() {
    demonstrateFactoryPattern();
    return 0;
}
