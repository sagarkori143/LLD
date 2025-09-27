#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Enhanced Vehicle class with operator overloading
class Vehicle {
protected:
    string model;
    int price;
    string type;
    
public:
    Vehicle(string m, int p, string t) : model(m), price(p), type(t) {}
    
    // Getters
    string getModel() const { return model; }
    int getPrice() const { return price; }
    string getType() const { return type; }
    
    // Virtual functions
    virtual void start() = 0;
    virtual ~Vehicle() = default;
    
    // =========================================================================
    // OPERATOR OVERLOADING EXAMPLES
    // =========================================================================
    
    // 1. Arithmetic Operators
    // + operator: Combine two vehicles (price addition)
    Vehicle operator+(const Vehicle& other) const {
        return Vehicle(model + "+" + other.model, 
                      price + other.price, 
                      type + "+" + other.type);
    }
    
    // 2. Comparison Operators
    // == operator: Compare vehicles by price
    bool operator==(const Vehicle& other) const {
        return price == other.price;
    }
    
    // < operator: Compare vehicles by price (for sorting)
    bool operator<(const Vehicle& other) const {
        return price < other.price;
    }
    
    // > operator: Compare vehicles by price
    bool operator>(const Vehicle& other) const {
        return price > other.price;
    }
    
    // 3. Assignment Operator
    Vehicle& operator=(const Vehicle& other) {
        if (this != &other) {  // Self-assignment check
            model = other.model;
            price = other.price;
            type = other.type;
        }
        return *this;
    }
    
    // 4. Stream Operators (friend functions)
    // << operator: Output stream
    friend ostream& operator<<(ostream& os, const Vehicle& vehicle) {
        os << "[" << vehicle.type << "] " << vehicle.model 
           << " - ₹" << vehicle.price;
        return os;
    }
    
    // >> operator: Input stream
    friend istream& operator>>(istream& is, Vehicle& vehicle) {
        cout << "Enter model: ";
        is >> vehicle.model;
        cout << "Enter price: ";
        is >> vehicle.price;
        cout << "Enter type: ";
        is >> vehicle.type;
        return is;
    }
    
    // 5. Increment/Decrement Operators
    // Prefix ++: Increase price by 10%
    Vehicle& operator++() {
        price = price * 1.1;
        return *this;
    }
    
    // Postfix ++: Increase price by 10% (returns old value)
    Vehicle operator++(int) {
        Vehicle temp = *this;
        price = price * 1.1;
        return temp;
    }
    
    // 6. Subscript Operator
    // [] operator: Get character from model name
    char operator[](int index) const {
        if (index >= 0 && index < model.length()) {
            return model[index];
        }
        return '\0';
    }
    
    // 7. Function Call Operator
    // () operator: Calculate EMI
    double operator()(int months, double interestRate) const {
        double monthlyRate = interestRate / 12 / 100;
        return (price * monthlyRate * pow(1 + monthlyRate, months)) / 
               (pow(1 + monthlyRate, months) - 1);
    }
};

// Concrete Car class
class Car : public Vehicle {
public:
    Car(string model, int price) : Vehicle(model, price, "Car") {}
    
    void start() override {
        cout << "🚗 " << model << " car engine started!" << endl;
    }
    
    // Additional operators specific to Car
    // += operator: Add features (increases price)
    Car& operator+=(int featurePrice) {
        price += featurePrice;
        model += "+Feature";
        return *this;
    }
};

// Concrete Bike class
class Bike : public Vehicle {
public:
    Bike(string model, int price) : Vehicle(model, price, "Bike") {}
    
    void start() override {
        cout << "🏍️ " << model << " bike engine started!" << endl;
    }
    
    // -= operator: Apply discount
    Bike& operator-=(int discount) {
        price -= discount;
        return *this;
    }
};

// ============================================================================
// FUNCTION OVERLOADING EXAMPLES
// ============================================================================

class VehicleManager {
public:
    // Function overloading - same name, different parameters
    
    // 1. Search by model name
    void search(const string& model) {
        cout << "🔍 Searching for vehicle model: " << model << endl;
    }
    
    // 2. Search by price range
    void search(int minPrice, int maxPrice) {
        cout << "🔍 Searching for vehicles in price range: ₹" 
             << minPrice << " - ₹" << maxPrice << endl;
    }
    
    // 3. Search by type and price
    void search(const string& type, int maxPrice) {
        cout << "🔍 Searching for " << type 
             << " under ₹" << maxPrice << endl;
    }
    
    // 4. Add single vehicle
    void addVehicle(const Vehicle& vehicle) {
        cout << "➕ Added single vehicle: " << vehicle << endl;
    }
    
    // 5. Add multiple vehicles
    void addVehicle(const vector<Vehicle>& vehicles) {
        cout << "➕ Added " << vehicles.size() << " vehicles to inventory" << endl;
    }
    
    // 6. Display vehicle info
    void display(const Vehicle& vehicle) {
        cout << "📋 Vehicle: " << vehicle << endl;
    }
    
    // 7. Display with additional details
    void display(const Vehicle& vehicle, bool showDetails) {
        cout << "📋 Vehicle: " << vehicle;
        if (showDetails) {
            cout << " | EMI (5 years): ₹" << vehicle(60, 8.5) << "/month";
        }
        cout << endl;
    }
};

// ============================================================================
// DEMONSTRATION FUNCTIONS
// ============================================================================

void demonstrateFunctionOverloading() {
    cout << "\n=== FUNCTION OVERLOADING DEMONSTRATION ===" << endl;
    
    VehicleManager manager;
    
    // Different functions called based on parameters
    manager.search("BMW");                    // search(string)
    manager.search(500000, 1000000);         // search(int, int)
    manager.search("Car", 800000);           // search(string, int)
    
    Car car("Maruti Swift", 700000);
    vector<Vehicle> vehicles;
    
    manager.addVehicle(car);                 // addVehicle(Vehicle)
    manager.addVehicle(vehicles);            // addVehicle(vector)
    
    manager.display(car);                    // display(Vehicle)
    manager.display(car, true);              // display(Vehicle, bool)
}

void demonstrateOperatorOverloading() {
    cout << "\n=== OPERATOR OVERLOADING DEMONSTRATION ===" << endl;
    
    Car car1("BMW X1", 4000000);
    Car car2("Audi A4", 4500000);
    Bike bike1("Ducati", 2000000);
    
    cout << "\n--- Basic Vehicle Info ---" << endl;
    cout << "Car 1: " << car1 << endl;        // << operator
    cout << "Car 2: " << car2 << endl;
    cout << "Bike 1: " << bike1 << endl;
    
    cout << "\n--- Comparison Operators ---" << endl;
    cout << "Car1 == Car2? " << (car1 == car2) << endl;  // == operator
    cout << "Car1 < Car2? " << (car1 < car2) << endl;    // < operator
    cout << "Car1 > Bike1? " << (car1 > bike1) << endl;  // > operator
    
    cout << "\n--- Arithmetic Operators ---" << endl;
    Vehicle combined = car1 + car2;           // + operator
    cout << "Combined vehicle: " << combined << endl;
    
    cout << "\n--- Increment Operators ---" << endl;
    cout << "Car1 before increment: " << car1 << endl;
    ++car1;                                   // Prefix increment
    cout << "Car1 after increment: " << car1 << endl;
    
    cout << "\n--- Compound Assignment Operators ---" << endl;
    car2 += 100000;                          // += operator (add feature)
    cout << "Car2 after adding feature: " << car2 << endl;
    
    bike1 -= 50000;                          // -= operator (discount)
    cout << "Bike1 after discount: " << bike1 << endl;
    
    cout << "\n--- Subscript Operator ---" << endl;
    cout << "First character of Car1 model: " << car1[0] << endl;  // [] operator
    
    cout << "\n--- Function Call Operator ---" << endl;
    cout << "Car1 EMI (5 years, 8.5%): ₹" << car1(60, 8.5) << "/month" << endl;  // () operator
}

void demonstratePolymorphismWithOverloading() {
    cout << "\n=== POLYMORPHISM + OVERLOADING ===" << endl;
    
    vector<Vehicle*> vehicles;
    vehicles.push_back(new Car("Tesla Model 3", 5000000));
    vehicles.push_back(new Bike("Harley Davidson", 1800000));
    vehicles.push_back(new Car("Maruti Alto", 400000));
    
    cout << "\n--- Vehicle Fleet ---" << endl;
    for (const auto& vehicle : vehicles) {
        cout << *vehicle << endl;             // Overloaded << operator
        vehicle->start();                     // Virtual function
    }
    
    cout << "\n--- Sorting by Price ---" << endl;
    // Using overloaded < operator for sorting
    sort(vehicles.begin(), vehicles.end(), 
         [](Vehicle* a, Vehicle* b) { return *a < *b; });
    
    for (const auto& vehicle : vehicles) {
        cout << *vehicle << endl;
    }
    
    // Cleanup
    for (auto vehicle : vehicles) {
        delete vehicle;
    }
}

int main() {
    cout << "========================================" << endl;
    cout << "  FUNCTION & OPERATOR OVERLOADING DEMO  " << endl;
    cout << "========================================" << endl;
    
    demonstrateFunctionOverloading();
    demonstrateOperatorOverloading();
    demonstratePolymorphismWithOverloading();
    
    cout << "\n========================================" << endl;
    cout << "             SUMMARY                    " << endl;
    cout << "========================================" << endl;
    
    cout << "\n📚 Function Overloading:" << endl;
    cout << "✓ Same function name, different parameters" << endl;
    cout << "✓ Compile-time polymorphism" << endl;
    cout << "✓ Improves code readability and usability" << endl;
    
    cout << "\n⚙️ Operator Overloading:" << endl;
    cout << "✓ Define custom behavior for operators" << endl;
    cout << "✓ Makes classes behave like built-in types" << endl;
    cout << "✓ Enhances code expressiveness" << endl;
    
    cout << "\n🎯 Benefits:" << endl;
    cout << "✓ Intuitive and natural syntax" << endl;
    cout << "✓ Code reusability and maintainability" << endl;
    cout << "✓ Better integration with STL algorithms" << endl;
    
    return 0;
}