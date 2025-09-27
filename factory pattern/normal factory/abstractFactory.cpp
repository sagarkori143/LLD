#include <iostream>
#include <string>
using namespace std;

/**
 * ABSTRACT FACTORY PATTERN
 * 
 * Purpose: Provides an interface for creating families of related or dependent objects
 * without specifying their concrete classes.
 * 
 * Key Components:
 * 1. Abstract Products - Interfaces for products (Car, Bike)
 * 2. Concrete Products - Specific implementations (LuxuryCar, EconomyCar, etc.)
 * 3. Abstract Factory - Interface for creating product families
 * 4. Concrete Factories - Create specific product families (LuxuryFactory, EconomyFactory)
 * 5. Client - Uses abstract interfaces only
 * 
 * Benefits:
 * - Ensures product compatibility within families
 * - Isolates concrete classes from client
 * - Easy to exchange product families
 * - Promotes consistency among products
 */

// ============================================================================
// ABSTRACT PRODUCTS - Define interfaces for different product types
// ============================================================================

// Abstract Car interface
class Car {
public:
    virtual ~Car() = default;
    virtual void drive() = 0;
    virtual string getModel() = 0;
    virtual int getPrice() = 0;
    virtual string getCategory() = 0;
};

// Abstract Bike interface
class Bike {
public:
    virtual ~Bike() = default;
    virtual void ride() = 0;
    virtual string getModel() = 0;
    virtual int getPrice() = 0;
    virtual string getCategory() = 0;
};

// ============================================================================
// CONCRETE PRODUCTS - LUXURY FAMILY
// ============================================================================

class LuxuryCar : public Car {
private:
    string model;
    int price;
    
public:
    LuxuryCar(string m, int p) : model(m), price(p) {}
    
    void drive() override {
        cout << "🚗 Driving luxury car: " << model << " with premium comfort!" << endl;
        cout << "   Features: Leather seats, sunroof, premium sound system" << endl;
    }
    
    string getModel() override { return model; }
    int getPrice() override { return price; }
    string getCategory() override { return "Luxury"; }
    
    ~LuxuryCar() {
        cout << "   Luxury car " << model << " serviced and parked safely" << endl;
    }
};

class LuxuryBike : public Bike {
private:
    string model;
    int price;
    
public:
    LuxuryBike(string m, int p) : model(m), price(p) {}
    
    void ride() override {
        cout << "🏍️  Riding luxury bike: " << model << " with superior performance!" << endl;
        cout << "   Features: Carbon fiber, advanced suspension, premium exhaust" << endl;
    }
    
    string getModel() override { return model; }
    int getPrice() override { return price; }
    string getCategory() override { return "Luxury"; }
    
    ~LuxuryBike() {
        cout << "   Luxury bike " << model << " cleaned and stored" << endl;
    }
};

// ============================================================================
// CONCRETE PRODUCTS - ECONOMY FAMILY
// ============================================================================

class EconomyCar : public Car {
private:
    string model;
    int price;
    
public:
    EconomyCar(string m, int p) : model(m), price(p) {}
    
    void drive() override {
        cout << "🚙 Driving economy car: " << model << " with great fuel efficiency!" << endl;
        cout << "   Features: Efficient engine, basic comfort, reliable performance" << endl;
    }
    
    string getModel() override { return model; }
    int getPrice() override { return price; }
    string getCategory() override { return "Economy"; }
    
    ~EconomyCar() {
        cout << "   Economy car " << model << " returned to fleet" << endl;
    }
};

class EconomyBike : public Bike {
private:
    string model;
    int price;
    
public:
    EconomyBike(string m, int p) : model(m), price(p) {}
    
    void ride() override {
        cout << "🛵 Riding economy bike: " << model << " with excellent mileage!" << endl;
        cout << "   Features: Fuel efficient, lightweight, easy maintenance" << endl;
    }
    
    string getModel() override { return model; }
    int getPrice() override { return price; }
    string getCategory() override { return "Economy"; }
    
    ~EconomyBike() {
        cout << "   Economy bike " << model << " refueled and ready" << endl;
    }
};

// ============================================================================
// ABSTRACT FACTORY - Interface for creating product families
// ============================================================================

class VehicleFactory {
public:
    virtual ~VehicleFactory() = default;
    
    // Factory methods for creating related products
    virtual Car* createCar() = 0;
    virtual Bike* createBike() = 0;
    
    // Additional factory method for product information
    virtual string getFactoryType() = 0;
    virtual void showCatalog() = 0;
};

// ============================================================================
// CONCRETE FACTORIES - Create specific product families
// ============================================================================

class LuxuryVehicleFactory : public VehicleFactory { //  this is tyhe extention of the normal factory
public:
    Car* createCar() override {
        return new LuxuryCar("BMW X7", 8000000);
    }
    
    Bike* createBike() override {
        return new LuxuryBike("Ducati Panigale", 2500000);
    }
    
    string getFactoryType() override {
        return "Luxury Vehicle Factory";
    }
    
    void showCatalog() override {
        cout << "\n🏆 === LUXURY VEHICLE CATALOG ===" << endl;
        cout << "🚗 Cars: BMW X7, Mercedes S-Class, Audi A8" << endl;
        cout << "🏍️  Bikes: Ducati Panigale, Harley Davidson, Kawasaklatopi Ninja" << endl;
        cout << "💰 Price Range: ₹25L - ₹80L" << endl;
    }
};

class EconomyVehicleFactory : public VehicleFactory {
public:
    Car* createCar() override {
        return new EconomyCar("Maruti Swift", 800000);
    }
    
    Bike* createBike() override {
        return new EconomyBike("Hero Splendor", 80000);
    }
    
    string getFactoryType() override {
        return "Economy Vehicle Factory";
    }
    
    void showCatalog() override {
        cout << "\n💰 === ECONOMY VEHICLE CATALOG ===" << endl;
        cout << "🚙 Cars: Maruti Swift, Hyundai i20, Tata Tiago" << endl;
        cout << "🛵 Bikes: Hero Splendor, Bajaj Platina, TVS Star" << endl;
        cout << "💵 Price Range: ₹80K - ₹15L" << endl;
    }
};

// ============================================================================
// CLIENT CODE - Uses only abstract interfaces
// ============================================================================

class VehicleShowroom {
private:
    VehicleFactory* factory;
    
public:
    VehicleShowroom(VehicleFactory* f) : factory(f) {}
    
    ~VehicleShowroom() {
        delete factory;
    }
    
    void displayShowroom() {
        cout << "\n🏢 Welcome to " << factory->getFactoryType() << endl;
        factory->showCatalog();
    }
    
    void demonstrateVehicles() {
        cout << "\n🎯 === VEHICLE DEMONSTRATION ===" << endl;
        
        // Create and test car
        Car* car = factory->createCar();
        cout << "\n📋 Car Details:" << endl;
        cout << "   Model: " << car->getModel() << endl;
        cout << "   Category: " << car->getCategory() << endl;
        cout << "   Price: ₹" << car->getPrice() << endl;
        car->drive();
        
        // Create and test bike
        Bike* bike = factory->createBike();
        cout << "\n📋 Bike Details:" << endl;
        cout << "   Model: " << bike->getModel() << endl;
        cout << "   Category: " << bike->getCategory() << endl;
        cout << "   Price: ₹" << bike->getPrice() << endl;
        bike->ride();
        
        cout << "\n🔧 Maintenance and cleanup:" << endl;
        delete car;
        delete bike;
        cout << "   Vehicles returned to showroom" << endl;
    }
    
    void compareWithOtherCategory(VehicleFactory* otherFactory) {
        cout << "\n⚖️  === CATEGORY COMPARISON ===" << endl;
        
        Car* ourCar = factory->createCar();
        Car* theirCar = otherFactory->createCar();
        
        cout << "Our " << ourCar->getCategory() << " car: " << ourCar->getModel() 
             << " (₹" << ourCar->getPrice() << ")" << endl;
        cout << "Their " << theirCar->getCategory() << " car: " << theirCar->getModel() 
             << " (₹" << theirCar->getPrice() << ")" << endl;
        
        if (ourCar->getPrice() > theirCar->getPrice()) {
            cout << "💎 Our vehicles are premium with higher features" << endl;
        } else {
            cout << "💰 Our vehicles offer better value for money" << endl;
        }
        
        delete ourCar;
        delete theirCar;
    }
};

// ============================================================================
// FACTORY SELECTOR - Chooses appropriate factory based on criteria
// ============================================================================

class FactorySelector {
public:
    static VehicleFactory* getFactory(string category) {
        if (category == "luxury" || category == "Luxury") {
            return new LuxuryVehicleFactory();
        } else if (category == "economy" || category == "Economy") {
            return new EconomyVehicleFactory();
        } else {
            cout << "⚠️  Unknown category: " << category << ". Defaulting to Economy." << endl;
            return new EconomyVehicleFactory();
        }
    }
    
    static VehicleFactory* getFactoryByBudget(int budget) {
        if (budget >= 2000000) {
            cout << "💎 Budget allows luxury vehicles!" << endl;
            return new LuxuryVehicleFactory();
        } else {
            cout << "💰 Recommending economy vehicles for your budget" << endl;
            return new EconomyVehicleFactory();
        }
    }
};

// ============================================================================
// DEMONSTRATION FUNCTIONS
// ============================================================================

void demonstrateAbstractFactory() {
    cout << "🏭 ================================" << endl;
    cout << "    ABSTRACT FACTORY PATTERN     " << endl;
    cout << "   Vehicle Manufacturing Demo    " << endl;
    cout << "================================" << endl;
    
    // Demonstrate Luxury Factory
    cout << "\n" << string(50, '=') << endl;
    cout << "SCENARIO 1: Luxury Vehicle Showroom" << endl;
    cout << string(50, '=') << endl;
    
    VehicleFactory* luxuryFactory = new LuxuryVehicleFactory();
    VehicleShowroom luxuryShowroom(luxuryFactory);
    luxuryShowroom.displayShowroom();
    luxuryShowroom.demonstrateVehicles();
    
    // Demonstrate Economy Factory
    cout << "\n" << string(50, '=') << endl;
    cout << "SCENARIO 2: Economy Vehicle Showroom" << endl;
    cout << string(50, '=') << endl;
    
    VehicleFactory* economyFactory = new EconomyVehicleFactory();
    VehicleShowroom economyShowroom(economyFactory);
    economyShowroom.displayShowroom();
    economyShowroom.demonstrateVehicles();
    
    // Demonstrate Factory Selection
    cout << "\n" << string(50, '=') << endl;
    cout << "SCENARIO 3: Dynamic Factory Selection" << endl;
    cout << string(50, '=') << endl;
    
    // Select by category
    VehicleFactory* selectedFactory = FactorySelector::getFactory("luxury");
    VehicleShowroom dynamicShowroom(selectedFactory);
    dynamicShowroom.displayShowroom();
    
    // Select by budget
    cout << "\n🏦 Budget-based factory selection:" << endl;
    VehicleFactory* budgetFactory1 = FactorySelector::getFactoryByBudget(5000000);
    VehicleFactory* budgetFactory2 = FactorySelector::getFactoryByBudget(1000000);
    
    // Show what each budget factory creates
    cout << "High budget selection: " << budgetFactory1->getFactoryType() << endl;
    cout << "Low budget selection: " << budgetFactory2->getFactoryType() << endl;
    
    // Demonstrate comparison
    cout << "\n" << string(50, '=') << endl;
    cout << "SCENARIO 4: Category Comparison" << endl;
    cout << string(50, '=') << endl;
    
    VehicleFactory* luxFactory = new LuxuryVehicleFactory();
    VehicleShowroom comparisonShowroom(new EconomyVehicleFactory());
    comparisonShowroom.compareWithOtherCategory(luxFactory);
    
    // Clean up remaining factories
    delete budgetFactory1;
    delete budgetFactory2;
    delete luxFactory;
}

void explainAbstractFactoryPattern() {
    cout << "\n📚 ================================" << endl;
    cout << "    ABSTRACT FACTORY EXPLANATION   " << endl;
    cout << "================================" << endl;
    
    cout << "\n🎯 Key Concepts:" << endl;
    cout << "1. 🏭 Abstract Factory: VehicleFactory interface" << endl;
    cout << "2. 🔧 Concrete Factories: LuxuryVehicleFactory, EconomyVehicleFactory" << endl;
    cout << "3. 📦 Abstract Products: Car, Bike interfaces" << endl;
    cout << "4. 🚗 Concrete Products: LuxuryCar, EconomyCar, LuxuryBike, EconomyBike" << endl;
    cout << "5. 👤 Client: VehicleShowroom (uses only abstractions)" << endl;
    
    cout << "\n✅ Benefits Demonstrated:" << endl;
    cout << "• Family Consistency: Luxury factory creates only luxury products" << endl;
    cout << "• Easy Switching: Change entire product family by changing factory" << endl;
    cout << "• Extensibility: Add new families (Sports, Electric) without changing existing code" << endl;
    cout << "• Client Isolation: Client doesn't know concrete classes" << endl;
    
    cout << "\n🔄 Pattern Flow:" << endl;
    cout << "Client -> Abstract Factory -> Concrete Factory -> Concrete Products" << endl;
    
    cout << "\n🆚 Abstract Factory vs Factory Method:" << endl;
    cout << "Factory Method: Creates ONE type of product" << endl;
    cout << "Abstract Factory: Creates FAMILIES of related products" << endl;
    
    cout << "\n🎪 Real-world Examples:" << endl;
    cout << "• GUI Libraries: Windows/Mac UI component families" << endl;
    cout << "• Database Drivers: MySQL/PostgreSQL connection families" << endl;
    cout << "• Game Engines: Medieval/Futuristic asset families" << endl;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    try {
        demonstrateAbstractFactory();
        explainAbstractFactoryPattern();
        
        cout << "\n🎉 Abstract Factory Pattern demonstration completed successfully!" << endl;
        cout << "💡 Try modifying the code to add a new factory family (Sports, Electric, etc.)" << endl;
        
    } catch (const exception& e) {
        cerr << "❌ Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
