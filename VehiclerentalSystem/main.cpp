#include<bits/stdc++.h>
using namespace std;

enum class VehicleType {
    TWO_WHEELER,
    FOUR_WHEELER
};

enum class VehicleStatus {
    AVAILABLE,
    BOOKED,
    MAINTENANCE
};

enum class ReservationStatus {
    ACTIVE,
    COMPLETED,
    CANCELLED
};

class Vehicle{
private:
    int id;
    string numberPlate;
    string model;
    string company;
    string manufacturingDate;
    VehicleType vehicleType;
    VehicleStatus status;
    double dailyRentalCost;

public:
    Vehicle(int id, string numberPlate, string model, string company, 
            string manuDate, VehicleType type, double cost) {
        this->id = id;
        this->numberPlate = numberPlate;
        this->model = model;
        this->company = company;
        this->manufacturingDate = manuDate;
        this->vehicleType = type;
        this->status = VehicleStatus::AVAILABLE;
        this->dailyRentalCost = cost;
    }

    // Getters
    int getID() { return id; }
    string getNumberPlate() { return numberPlate; }
    string getModel() { return model; }
    string getCompany() { return company; }
    string getManufacturingDate() { return manufacturingDate; }
    VehicleType getVehicleType() { return vehicleType; }
    VehicleStatus getStatus() { return status; }
    double getDailyRentalCost() { return dailyRentalCost; }

    // Setters
    void setStatus(VehicleStatus newStatus) { status = newStatus; }
    void setDailyRentalCost(double cost) { dailyRentalCost = cost; }
};

class User{
private:
    string name;
    string drivingLicence;
    string DOB;
    long contactNumber;
    int userId;

public:
    User(int id, string name, string licence, string dob, long contact) {
        this->userId = id;
        this->name = name;
        this->drivingLicence = licence;
        this->DOB = dob;
        this->contactNumber = contact;
    }

    // Getters
    int getUserId() { return userId; }
    string getName() { return name; }
    string getDrivingLicence() { return drivingLicence; }
    string getDOB() { return DOB; }
    long getContactNumber() { return contactNumber; }
};

class Location{
private:
    string address;
    string city;
    string state;
    string pinCode;

public:
    Location(string addr, string city, string state, string pin) {
        this->address = addr;
        this->city = city;
        this->state = state;
        this->pinCode = pin;
    }

    // Getters
    string getAddress() { return address; }
    string getCity() { return city; }
    string getState() { return state; }
    string getPinCode() { return pinCode; }

    string getFullAddress() {
        return address + ", " + city + ", " + state + " - " + pinCode;
    }
};

class Store{
private:
    int storeId;
    Location location;
    vector<Vehicle*> inventory;

public:
    Store(int id, Location loc) : storeId(id), location(loc) {}

    // Getters
    int getStoreId() { return storeId; }
    Location getLocation() { return location; }
    vector<Vehicle*> getInventory() { return inventory; }

    // Add/Remove vehicles
    void addVehicle(Vehicle* vehicle) {
        inventory.push_back(vehicle);
    }

    void removeVehicle(int vehicleId) {
        inventory.erase(
            remove_if(inventory.begin(), inventory.end(),
                [vehicleId](Vehicle* v) { return v->getID() == vehicleId; }),
            inventory.end());
    }

    // Find available vehicles by type
    vector<Vehicle*> getAvailableVehicles(VehicleType type) {
        vector<Vehicle*> available;
        for (Vehicle* v : inventory) {
            if (v->getVehicleType() == type && v->getStatus() == VehicleStatus::AVAILABLE) {
                available.push_back(v);
            }
        }
        return available;
    }
};

// Base Store Inventory class
class StoreInventory{
protected:
    vector<Vehicle*> vehicles;

public:
    virtual ~StoreInventory() {}
    
    virtual void addVehicle(Vehicle* vehicle) = 0;
    virtual void removeVehicle(int vehicleId) = 0;
    virtual vector<Vehicle*> getAvailableVehicles() = 0;
    
    int getTotalCount() { return vehicles.size(); }
};

class TwoWheelerInventory: public StoreInventory{
public:
    void addVehicle(Vehicle* vehicle) override {
        if (vehicle->getVehicleType() == VehicleType::TWO_WHEELER) {
            vehicles.push_back(vehicle);
        }
    }
    
    void removeVehicle(int vehicleId) override {
        vehicles.erase(
            remove_if(vehicles.begin(), vehicles.end(),
                [vehicleId](Vehicle* v) { return v->getID() == vehicleId; }),
            vehicles.end());
    }
    
    vector<Vehicle*> getAvailableVehicles() override {
        vector<Vehicle*> available;
        for (Vehicle* v : vehicles) {
            if (v->getStatus() == VehicleStatus::AVAILABLE) {
                available.push_back(v);
            }
        }
        return available;
    }
};

class FourWheelerInventory: public StoreInventory{
public:
    void addVehicle(Vehicle* vehicle) override {
        if (vehicle->getVehicleType() == VehicleType::FOUR_WHEELER) {
            vehicles.push_back(vehicle);
        }
    }
    
    void removeVehicle(int vehicleId) override {
        vehicles.erase(
            remove_if(vehicles.begin(), vehicles.end(),
                [vehicleId](Vehicle* v) { return v->getID() == vehicleId; }),
            vehicles.end());
    }
    
    vector<Vehicle*> getAvailableVehicles() override {
        vector<Vehicle*> available;
        for (Vehicle* v : vehicles) {
            if (v->getStatus() == VehicleStatus::AVAILABLE) {
                available.push_back(v);
            }
        }
        return available;
    }
};

// Factory to create appropriate inventory
class StoreInventoryFactory{
public:
    static StoreInventory* createInventory(VehicleType type) {
        switch (type) {
            case VehicleType::TWO_WHEELER:
                return new TwoWheelerInventory();
            case VehicleType::FOUR_WHEELER:
                return new FourWheelerInventory();
            default:
                return nullptr;
        }
    }
};

class Search{
private:
    vector<Store*> stores;

public:
    Search(vector<Store*> storeList) : stores(storeList) {}
    
    vector<Store*> getStoresByLocation(string city) {
        vector<Store*> result;
        for (Store* store : stores) {
            if (store->getLocation().getCity() == city) {
                result.push_back(store);
            }
        }
        return result;
    }
    
    Store* getNearestStore(Location userLocation) {
        // Simplified: return first store in same city
        for (Store* store : stores) {
            if (store->getLocation().getCity() == userLocation.getCity()) {
                return store;
            }
        }
        return stores.empty() ? nullptr : stores[0]; // Fallback
    }
};

class Reservation{
private:
    int reservationId;
    User* user;
    Vehicle* vehicle;
    string bookingDate;
    string fromDate;
    string toDate;
    Location pickupLocation;
    Location dropLocation;
    ReservationStatus status;
    double totalCost;

public:
    Reservation(int id, User* user, Vehicle* vehicle, string bookDate,
                string from, string to, Location pickup, Location drop) 
        : reservationId(id), user(user), vehicle(vehicle), bookingDate(bookDate),
          fromDate(from), toDate(to), pickupLocation(pickup), dropLocation(drop) {
        status = ReservationStatus::ACTIVE;
        calculateTotalCost();
    }

    void calculateTotalCost() {
        // Simple calculation: assume 1 day rental
        totalCost = vehicle->getDailyRentalCost();
    }

    // Getters
    int getReservationId() { return reservationId; }
    User* getUser() { return user; }
    Vehicle* getVehicle() { return vehicle; }
    string getFromDate() { return fromDate; }
    string getToDate() { return toDate; }
    ReservationStatus getStatus() { return status; }
    double getTotalCost() { return totalCost; }
    Location getPickupLocation() { return pickupLocation; }
    Location getDropLocation() { return dropLocation; }

    // Actions
    void confirmReservation() {
        vehicle->setStatus(VehicleStatus::BOOKED);
        cout << "Reservation confirmed for vehicle: " << vehicle->getNumberPlate() << endl;
    }

    void cancelReservation() {
        status = ReservationStatus::CANCELLED;
        vehicle->setStatus(VehicleStatus::AVAILABLE);
        cout << "Reservation cancelled for vehicle: " << vehicle->getNumberPlate() << endl;
    }

    void completeReservation() {
        status = ReservationStatus::COMPLETED;
        vehicle->setStatus(VehicleStatus::AVAILABLE);
        cout << "Reservation completed for vehicle: " << vehicle->getNumberPlate() << endl;
    }
};

class Payment{
private:
    int paymentId;
    Reservation* reservation;
    double amount;
    string paymentDate;
    string paymentMethod;
    bool isCompleted;

public:
    Payment(int id, Reservation* res, double amt, string date, string method) 
        : paymentId(id), reservation(res), amount(amt), paymentDate(date), 
          paymentMethod(method), isCompleted(false) {}

    bool processPayment() {
        // Simulate payment processing
        cout << "Processing payment of $" << amount << " for reservation " 
             << reservation->getReservationId() << endl;
        isCompleted = true;
        return true;
    }

    // Getters
    int getPaymentId() { return paymentId; }
    double getAmount() { return amount; }
    bool getPaymentStatus() { return isCompleted; }
};class VehicleRentalSystem{
private:
    vector<Store*> stores;
    vector<User*> users;
    vector<Reservation*> reservations;
    Search* searchService;
    int nextReservationId;
    int nextPaymentId;

public:
    VehicleRentalSystem() : nextReservationId(1), nextPaymentId(1) {
        searchService = new Search(stores);
    }

    ~VehicleRentalSystem() {
        delete searchService;
        for (auto store : stores) delete store;
        for (auto user : users) delete user;
        for (auto reservation : reservations) delete reservation;
    }

    // Add store
    void addStore(Store* store) {
        stores.push_back(store);
        delete searchService;
        searchService = new Search(stores);
    }

    // Add user
    void addUser(User* user) {
        users.push_back(user);
    }

    // Search available vehicles
    vector<Vehicle*> searchVehicles(string city, VehicleType type) {
        vector<Store*> cityStores = searchService->getStoresByLocation(city);
        vector<Vehicle*> availableVehicles;

        for (Store* store : cityStores) {
            vector<Vehicle*> storeVehicles = store->getAvailableVehicles(type);
            availableVehicles.insert(availableVehicles.end(), 
                                   storeVehicles.begin(), storeVehicles.end());
        }
        return availableVehicles;
    }

    // Book vehicle
    Reservation* bookVehicle(User* user, Vehicle* vehicle, string fromDate, 
                           string toDate, Location pickup, Location drop) {
        if (vehicle->getStatus() != VehicleStatus::AVAILABLE) {
            cout << "Vehicle is not available for booking!" << endl;
            return nullptr;
        }

        Reservation* reservation = new Reservation(nextReservationId++, user, vehicle,
                                                  "2024-01-01", fromDate, toDate, 
                                                  pickup, drop);
        
        reservations.push_back(reservation);
        reservation->confirmReservation();
        
        return reservation;
    }

    // Process payment
    bool processPayment(Reservation* reservation, string paymentMethod) {
        Payment* payment = new Payment(nextPaymentId++, reservation, 
                                     reservation->getTotalCost(), 
                                     "2024-01-01", paymentMethod);
        
        return payment->processPayment();
    }

    // Complete booking
    void completeBooking(int reservationId) {
        for (Reservation* res : reservations) {
            if (res->getReservationId() == reservationId) {
                res->completeReservation();
                return;
            }
        }
    }

    // Cancel booking
    void cancelBooking(int reservationId) {
        for (Reservation* res : reservations) {
            if (res->getReservationId() == reservationId) {
                res->cancelReservation();
                return;
            }
        }
    }

    // Display available vehicles
    void displayAvailableVehicles(string city, VehicleType type) {
        vector<Vehicle*> vehicles = searchVehicles(city, type);
        cout << "\n=== Available Vehicles in " << city << " ===\n";
        
        for (Vehicle* v : vehicles) {
            cout << "ID: " << v->getID() 
                 << ", Model: " << v->getModel()
                 << ", Company: " << v->getCompany()
                 << ", Daily Cost: $" << v->getDailyRentalCost() << endl;
        }
    }
};

int main(){
    cout << "=== Vehicle Rental System Demo ===" << endl;

    // Create locations
    Location mumbaiLoc("Andheri West", "Mumbai", "Maharashtra", "400058");
    Location delhiLoc("CP", "Delhi", "Delhi", "110001");
    Location bangaloreLoc("Koramangala", "Bangalore", "Karnataka", "560034");

    // Create stores
    Store* mumbaiStore = new Store(1, mumbaiLoc);
    Store* delhiStore = new Store(2, delhiLoc);
    Store* bangaloreStore = new Store(3, bangaloreLoc);

    // Create vehicles
    Vehicle* bike1 = new Vehicle(101, "MH01AB1234", "Activa", "Honda", "2023", VehicleType::TWO_WHEELER, 300.0);
    Vehicle* bike2 = new Vehicle(102, "MH01CD5678", "Splendor", "Hero", "2022", VehicleType::TWO_WHEELER, 250.0);
    Vehicle* car1 = new Vehicle(201, "MH01EF9012", "Swift", "Maruti", "2023", VehicleType::FOUR_WHEELER, 1200.0);
    Vehicle* car2 = new Vehicle(202, "DL01GH3456", "City", "Honda", "2024", VehicleType::FOUR_WHEELER, 1500.0);
    Vehicle* car3 = new Vehicle(203, "KA01IJ7890", "Innova", "Toyota", "2023", VehicleType::FOUR_WHEELER, 2000.0);

    // Add vehicles to stores
    mumbaiStore->addVehicle(bike1);
    mumbaiStore->addVehicle(car1);
    delhiStore->addVehicle(bike2);
    delhiStore->addVehicle(car2);
    bangaloreStore->addVehicle(car3);

    // Create users
    User* user1 = new User(1, "John Doe", "DL123456789", "1990-01-01", 9876543210);
    User* user2 = new User(2, "Jane Smith", "DL987654321", "1985-05-15", 8765432109);

    // Create rental system
    VehicleRentalSystem rentalSystem;
    rentalSystem.addStore(mumbaiStore);
    rentalSystem.addStore(delhiStore);
    rentalSystem.addStore(bangaloreStore);
    rentalSystem.addUser(user1);
    rentalSystem.addUser(user2);

    cout << "\n=== System Initialized Successfully ===" << endl;

    // Demo 1: Search for vehicles
    cout << "\n--- Searching for cars in Mumbai ---" << endl;
    rentalSystem.displayAvailableVehicles("Mumbai", VehicleType::FOUR_WHEELER);

    cout << "\n--- Searching for bikes in Delhi ---" << endl;
    rentalSystem.displayAvailableVehicles("Delhi", VehicleType::TWO_WHEELER);

    // Demo 2: Book a vehicle
    cout << "\n--- Booking a vehicle ---" << endl;
    vector<Vehicle*> mumbaiCars = rentalSystem.searchVehicles("Mumbai", VehicleType::FOUR_WHEELER);
    
    if (!mumbaiCars.empty()) {
        Vehicle* selectedCar = mumbaiCars[0];
        cout << "User " << user1->getName() << " is booking: " << selectedCar->getModel() << endl;
        
        Reservation* reservation = rentalSystem.bookVehicle(user1, selectedCar, "2024-01-15", "2024-01-18", 
                                                          mumbaiLoc, mumbaiLoc);
        
        if (reservation) {
            cout << "Booking successful! Reservation ID: " << reservation->getReservationId() << endl;
            cout << "Total Cost: $" << reservation->getTotalCost() << endl;
            
            // Process payment
            cout << "\n--- Processing Payment ---" << endl;
            bool paymentSuccess = rentalSystem.processPayment(reservation, "Credit Card");
            
            if (paymentSuccess) {
                cout << "Payment processed successfully!" << endl;
            }
            
            // Check availability after booking
            cout << "\n--- Checking availability after booking ---" << endl;
            rentalSystem.displayAvailableVehicles("Mumbai", VehicleType::FOUR_WHEELER);
            
            // Complete booking
            cout << "\n--- Completing the booking ---" << endl;
            rentalSystem.completeBooking(reservation->getReservationId());
            
            cout << "\n--- Final availability check ---" << endl;
            rentalSystem.displayAvailableVehicles("Mumbai", VehicleType::FOUR_WHEELER);
        }
    }

    // Demo 3: Try booking unavailable vehicle
    cout << "\n--- Trying to book the same vehicle again ---" << endl;
    if (!mumbaiCars.empty()) {
        Vehicle* sameCar = mumbaiCars[0];
        sameCar->setStatus(VehicleStatus::BOOKED); // Simulate booking
        Reservation* reservation2 = rentalSystem.bookVehicle(user2, sameCar, "2024-01-20", "2024-01-22", 
                                                           mumbaiLoc, mumbaiLoc);
    }

    cout << "\n=== Vehicle Rental System Demo Completed ===" << endl;

    // Cleanup
    delete mumbaiStore;
    delete delhiStore;
    delete bangaloreStore;
    delete bike1;
    delete bike2;
    delete car1;
    delete car2;
    delete car3;
    delete user1;
    delete user2;

    return 0;
}
