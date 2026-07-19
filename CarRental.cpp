#include<bits/stdc++.h>
using namespace std;
enum class VehicleType{
    BIKE,
    CAR,
    TRUCK
};
enum class VehicleStatus{
    AVAILABLE,
    BOOKED,
    IN_SERVICE
};
enum class ReservationStatus{
    ACTIVE,
    CANCELLED,
    COMPLETED
};
class User{
    private:
        string name;
        string drivingLicence;
        string DOB;
        long contactNumber;
        int userId;
    public:
        User(int id, string name, string licence, string dob, long contact){
            this->userId=id;
            this->drivingLicence=licence;
            this->DOB=dob;
            this->contactNumber=contact;
            this->name=name;
        }
        string getName(){ return name; }
        string getDrivingLicence(){ return drivingLicence; }
        string getDOB(){ return DOB; }
        long getContactNumber(){ return contactNumber; }
        int getUserId(){ return userId; }
};

class Location{
    private:
        string address;
        string city;
        string state;
        string pinCode;
    public:
        Location(string addr, string city, string state, string pin){
            this->address=addr;
            this->city=city;
            this->state=state;
            this->pinCode=pin;
        }
        string getAddress(){ return address; }
        string getCity(){ return city; }
        string getState(){ return state; }
        string getPinCode(){ return pinCode; }
};

class Vehicle{
    private:
    int id;
    VehicleType type;
    VehicleStatus status;
    string numberPlate;
    public:
    Vehicle(VehicleType type, int id){
        this->id=id;
        this->type=type;
        this->status=VehicleStatus::AVAILABLE;
    }
    virtual ~Vehicle(){};
    VehicleType getType(){ return type; }
    int getId(){ return id; }
    VehicleStatus getStatus(){ return status; }
    void setStatus(VehicleStatus newStatus){ status=newStatus; }
    string getNumberPlate(){ return numberPlate; }
};

class Car : public Vehicle{
    private:
        int numberOfSeats;
    public:
        Car(int id, int seats): Vehicle(VehicleType::CAR, id), numberOfSeats(seats){}
        int getNumberOfSeats(){ return numberOfSeats; }
};

class Bike: public Vehicle{
    private:
        bool hasCarrier;
    public:
        Bike(int id, bool carrier): Vehicle(VehicleType::BIKE, id), hasCarrier(carrier){}
        bool getHasCarrier(){ return hasCarrier; }
};

class VehicleFactory{
    public:
        Vehicle* createVehicle(VehicleType type, int id){
            switch(type){
                case VehicleType::CAR:
                    return new Car(id, 4); // default 4 seats
                case VehicleType::BIKE:
                    return new Bike(id, false); // default no carrier
                default:
                    return nullptr;
            }
        }
};

class Store{
    private:
    int storeId;
    Location location;
    vector<Vehicle*> inventory;
    public:
    Store(int id, Location loc): storeId(id), location(loc){}
    int getStoreId(){ return storeId; }
    Location getLocation(){ return location; }
    void addVehicle(Vehicle* vehicle){
        inventory.push_back(vehicle);
    }
    vector<Vehicle*> getAvailableVehicles(VehicleType type){
        vector<Vehicle*> available;
        for(Vehicle* v: inventory){
            if(v->getType()==type && v->getStatus()==VehicleStatus::AVAILABLE){
                available.push_back(v);
            }
        }
        return available;
    }
    void removeVehicle(int vehicleId){
        inventory.erase(
            remove_if(inventory.begin(), inventory.end(),
                [vehicleId](Vehicle* v){ return v->getId()==vehicleId; }),
            inventory.end());
    }

};

class storesManager{
    private:
    vector<Store*> stores;
    public:
    storesManager(vector<Store*> storeList): stores(storeList){}
    vector<Store*> getStoresByLocation(string city){
        vector<Store*> result;
        for(Store* store: stores){
            if(store->getLocation().getCity()==city){   
                result.push_back(store);
            }
        }
        return result;
    }
    void addStore(Store* store){
        stores.push_back(store);
    }
    Store* getNearestStore(Location userLocation){
        // Simplified: return first store in same city
        for(Store* store: stores){
            if(store->getLocation().getCity()==userLocation.getCity()){
                return store;
            }
        }
        return nullptr;
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
        ReservationStatus status;
    public:
        Reservation(int id, User* user, Vehicle* vehicle, string bookDate, string from, string to){
            this->reservationId=id;
            this->user=user;
            this->vehicle=vehicle;
            this->bookingDate=bookDate;
            this->fromDate=from;
            this->toDate=to;
            status=ReservationStatus::ACTIVE;
        }
        int getReservationId(){ return reservationId; }
        User* getUser(){ return user; }
        Vehicle* getVehicle(){ return vehicle; }
        string getFromDate(){ return fromDate; }
        string getToDate(){ return toDate; }
        ReservationStatus getStatus(){ return status; }
        void confirmReservation(){
            vehicle->setStatus(VehicleStatus::BOOKED);
            cout<<"Reservation confirmed for vehicle: "<<vehicle->getNumberPlate()<<endl;
        }
        void cancelReservation(){
            status=ReservationStatus::CANCELLED;
            vehicle->setStatus(VehicleStatus::AVAILABLE);
            cout<<"Reservation cancelled for vehicle: "<<vehicle->getNumberPlate()<<endl;
        }
        void completeReservation(){
            status=ReservationStatus::COMPLETED;
            vehicle->setStatus(VehicleStatus::AVAILABLE);
            cout<<"Reservation completed for vehicle: "<<vehicle->getNumberPlate()<<endl;
        }
};
class PaymentStrategy{
    public:
        virtual double pay()=0;
};
class PayByUPI : public PaymentStrategy{
    private:
        string upiId;
    public:
        PayByUPI(string id): upiId(id){}
        double pay(){
            cout<<"Paying via UPI: "<<upiId<<endl;
            return 0.0; // placeholder
        }
};
class PayByCard : public PaymentStrategy{
    private:
        string cardNumber;
    public:
        PayByCard(string number): cardNumber(number){}
        double pay(){
            cout<<"Paying via Card: "<<cardNumber<<endl;
            return 0.0; // placeholder
        }
};
class Bill{
    private:
        int billId;
        Reservation* reservation;
        double amount;
        string billingDate;
        PaymentStrategy* paymentMethod;
        string submitDate;
        int extraCharges;
        public: 
        Bill(int id, Reservation* res, double amt, string billDate, PaymentStrategy* method){
            this->billId=id;
            this->reservation=res;
            this->amount=amt;
            this->billingDate=billDate;
            this->paymentMethod=method;
            // prepare extra charges based on late return, damages, etc.
            this->extraCharges=0;

        }
    public:
        int getBillId(){ return billId; }
        Reservation* getReservation(){ return reservation; }
        double getAmount(){ return amount; }
        string getBillingDate(){ return billingDate; }
        PaymentStrategy* getPaymentMethod(){ return paymentMethod; }
        void payBill(){
            double paidAmount=paymentMethod->pay();
            cout<<"Paid amount: "<<paidAmount<<endl;
        }

};

class rentalManager{
    vector<Reservation*> reservations;
    vector<Bill*> bills;
    storesManager* storeMgr;
    public:
    rentalManager(storesManager* mgr): storeMgr(mgr){}
    vector<Vehicle*> searchAvailableVehicles(string city, VehicleType type){
        vector<Store*> cityStores= storeMgr->getStoresByLocation(city);
        vector<Vehicle*> availableVehicles;
        for(Store* store: cityStores){
            vector<Vehicle*> storeVehicles= store->getAvailableVehicles(type);
            availableVehicles.insert(availableVehicles.end(), 
                                    storeVehicles.begin(), storeVehicles.end());
        }
        return availableVehicles;
    }
    Reservation* bookVehicle(User* user, Vehicle* vehicle, string fromDate, string toDate){
        if(vehicle->getStatus()!=VehicleStatus::AVAILABLE){
            cout<<"Vehicle is not available for booking!"<<endl;
            return nullptr;
        }
        Reservation* reservation= new Reservation(reservations.size()+1, user, vehicle, "2024-01-01", fromDate, toDate);
        reservations.push_back(reservation);
        reservation->confirmReservation();
        return reservation;
    }
    void addStore(Store* store){
        storeMgr->addStore(store);
    }
};


int main(){
    cout<<"starting car rental system..."<<endl;
    rentalManager* rentalMgr= new rentalManager(new storesManager({})); 
    Store* store1= new Store(1, Location("123 Main St", "CityA", "StateA", "12345"));
    rentalMgr->addStore(store1);
    VehicleFactory* vehicleFactory= new VehicleFactory();
    Vehicle* BMW= vehicleFactory->createVehicle(VehicleType::CAR, 1);
    store1->addVehicle(BMW);
    // lets check available vehicles in CityA
    vector<Vehicle*> availableCars= rentalMgr->searchAvailableVehicles("CityA", VehicleType::CAR);
    cout<<"Available cars in CityA: "<<availableCars.size()<<endl;
    // create a user
    User* user1= new User(1, " Sagar Kori", "DL12345", "1990-01-01", 1234567890);
    // book a vehicle
    if(!availableCars.empty()){
        Reservation* reservation= rentalMgr->bookVehicle(user1, BMW, "2024-01-01", "2024-01-02");
        cout<<" reservation made for user: "<<user1->getName()<<endl;
        cout<<"Reservation ID: "<<reservation->getReservationId()<<endl;
        cout<<"------ Booking completed ---"<<endl;
    }

    // Now it is the time to submit the bill and pay for the reservation
    PaymentStrategy* method= new PayByUPI("sagar@upi");
    Bill* bill = new Bill(1, rentalMgr->bookVehicle(user1, BMW, "2024-01-01", "2024-01-02"), 100.0, "2024-01-01", method);
    bill->payBill();
    cout<<"Bill paid successfully!"<<endl;
    
}