package parkingLotDesign;

public class ParkingSpot {
    private String id;
    private Boolean empty=true;;
    private Vehicle vehicle;

    ParkingSpot(String id){
        this.id=id;
    }
    public Boolean isEmpty(){
        return empty;
    }
    public Vehicle getVehicle(){
        return vehicle;
    }
    public void assignVehicle(Vehicle vehicle){
        this.vehicle=vehicle;
        empty=false;
    }
    public void removeVehicle(){
        this.vehicle=null;
        empty=true;
    }
    public String getID(){return id;}
};

// Now the parking spot can be for different type of vehicles.. accordinly we will assign the parking spots
