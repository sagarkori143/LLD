#include<bits/stdc++.h>
using namespace std;
enum class VehicleType { Car, Bike, Truck };
enum class SpotType { Car, Bike, Truck };


class Vehicle{
    private:
     int id;
     VehicleType type;
    public: Vehicle(VehicleType type, int id){
        this->id=id;
        this->type=type;
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
    public:
    ParkingLot(){};
    void addLevel(Level* level){
        levels.push_back(level);
        ticketId=0;
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
};

int main(){
    // 1. Build the lot
    ParkingLot lot;

    // 2. Add a level and put some spots on it
    Level* l1 = new Level(1);
    l1->addSpot(new ParkingSpot(101, SpotType::Car));
    l1->addSpot(new ParkingSpot(102, SpotType::Bike));
    l1->addSpot(new ParkingSpot(103, SpotType::Car));
    lot.addLevel(l1);

    // 3. A car arrives
    Vehicle* car = new Vehicle(VehicleType::Car, 1);

    // 4. Find a matching free spot
    ParkingSpot* spot = lot.findParkingSpot(VehicleType::Car);
    if(spot == nullptr){
        cout << "No spot available\n";
        return 0;
    }

    // 5. Book it -> get a ticket
    Ticket* ticket = lot.bookSpot(spot, car);
    cout << "Parked. Ticket issued.\n";

    // 6. Try booking another car -> should take spot 103
    Vehicle* car2 = new Vehicle(VehicleType::Car, 2);
    ParkingSpot* spot2 = lot.findParkingSpot(VehicleType::Car);
    cout << (spot2 ? "Second car found a spot\n" : "No spot for second car\n");

    return 0;
}



