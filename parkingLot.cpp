#include<bits/stdc++.h>
using namespace std;
enum class VehicleType { Car, Bike, Truck };
enum class SpotType { Car, Bike, Truck };

class PricingStrategy {
    public:
    virtual double calculate(int hours) = 0;
    virtual ~PricingStrategy() = default;
};

class HourlyPricing : public PricingStrategy {
    public: double calculate(int hours) override { return hours * 10; }
};
class DailyPricing : public PricingStrategy {
    public: double calculate(int hours) override { return ((hours + 23) / 24) * 100; }
};


class Vehicle{
    private:
     int id;
     VehicleType type;
    public: Vehicle(VehicleType type, int id){
        this->id=id;
        this->type=type;
    }
    VehicleType getType(){ return type; }
};

class VehicleFactory {
    public:
    Vehicle* create(VehicleType type, int id){
        return new Vehicle(type, id);   // later: Car->1 spot, Truck->2 spots, etc.
    }
};

class ParkingSpot{
    private:
     int id;
     SpotType type;
     bool isAvailable;
     Vehicle* vehicle;
    public:
    ParkingSpot(int id, SpotType type){
        this->id=id;
        this->type=type;
        isAvailable=true;
        this->vehicle=nullptr;
    }
    void assignVehicle(Vehicle* vehicle){
        this->vehicle=vehicle;
        isAvailable=false;
        
    }
    void removeVehicle(){
        this->vehicle=nullptr;
        isAvailable=true;
    }
    SpotType getType(){ return type; }
    bool isFree(){ return isAvailable; }
};

class Ticket{
    private:
    int id;
    Vehicle* vehicle;
    ParkingSpot* spot;
    string entryTime;
    public:
    Ticket(int id,Vehicle* vehicle, ParkingSpot* spot, string entryTime){
        this->id=id;
        this->vehicle=vehicle;
        this->spot=spot;
        this->entryTime=entryTime;
    }
    ParkingSpot* getSpot(){ return spot; }
};

class Level{
    private:
     int floor;
     vector<ParkingSpot*> spots;
    public:
    Level(int floor ){
        this->floor=floor;
    }
    vector<ParkingSpot*> listSpots(){
        return spots;
    }
    void addSpot(ParkingSpot* spot){
        spots.push_back(spot);
    }
    void removeSpot(ParkingSpot* spot){
        // find and remove
    }
};

class ParkingLot{
    private:
    vector<Level*> levels;
    int ticketId;
    PricingStrategy* pricing;
    public:
    ParkingLot(){ ticketId = 0; pricing = nullptr; }
    void setPricing(PricingStrategy* p){ pricing = p; }
    void addLevel(Level* level){
        levels.push_back(level);
    }
    SpotType spotFor(VehicleType v){
        switch(v){
            case VehicleType::Car:   return SpotType::Car;
            case VehicleType::Bike:  return SpotType::Bike;
            case VehicleType::Truck: return SpotType::Truck;
        }
        return SpotType::Car; // fallback so all paths return
    }

    ParkingSpot* findParkingSpot(VehicleType type){
        for(Level* lvl: levels){
            vector<ParkingSpot*> tempList= lvl->listSpots();
            for(ParkingSpot* ps: tempList){
                if(ps->getType()==spotFor(type) && ps->isFree())return ps;
            }
        }
        return nullptr;
    }

    Ticket* generateTicket(Vehicle* vehicle, ParkingSpot* spot){
        string entryTime = "2026-07-18 10:00"; // placeholder; use a clock later
        Ticket* ticket= new Ticket(ticketId++,vehicle, spot, entryTime );
        return ticket;
    }

    Ticket* bookSpot(ParkingSpot* spot, Vehicle* vehicle){
        spot->assignVehicle(vehicle);
        return generateTicket(vehicle, spot);
    }

    // Car leaves: free the spot and charge based on the chosen strategy
    double unpark(Ticket* ticket, int hoursStayed){
        ticket->getSpot()->removeVehicle();   // frees the spot (isAvailable = true)
        return pricing->calculate(hoursStayed);
    }
};

int main(){
    // 1. Build the lot + pick a pricing strategy
    ParkingLot lot;
    lot.setPricing(new HourlyPricing());   // swap to new DailyPricing() to change billing

    // 2. Add a level and put some spots on it
    Level* l1 = new Level(1);
    l1->addSpot(new ParkingSpot(101, SpotType::Car));
    l1->addSpot(new ParkingSpot(102, SpotType::Bike));
    l1->addSpot(new ParkingSpot(103, SpotType::Car));
    lot.addLevel(l1);

    // 3. A car arrives (built via the factory, not `new Vehicle`)
    VehicleFactory factory;
    Vehicle* car = factory.create(VehicleType::Car, 1);

    // 4. Find a matching free spot
    ParkingSpot* spot = lot.findParkingSpot(VehicleType::Car);
    if(spot == nullptr){
        cout << "No spot available\n";
        return 0;
    }

    // 5. Book it -> get a ticket
    Ticket* ticket = lot.bookSpot(spot, car);
    cout << "Parked. Ticket issued.\n";

    // 6. Car leaves after 3 hours -> spot freed, fee charged via strategy
    double fee = lot.unpark(ticket, 3);
    cout << "Car left. Fee = " << fee << "\n";

    // 7. Now that spot is free again, another car should find it
    Vehicle* car2 = factory.create(VehicleType::Car, 2);
    ParkingSpot* spot2 = lot.findParkingSpot(VehicleType::Car);
    cout << (spot2 ? "Second car found a spot\n" : "No spot for second car\n");

    return 0;
}



