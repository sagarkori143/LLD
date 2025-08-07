package parkingLotDesign;

public class Ticket {
    long entryTime;
    ParkingSpot spot;
    Vehicle vehicle;

    Ticket(ParkingSpot spot, Vehicle vehicle){
        this.spot=spot;
        this.vehicle=vehicle;
        entryTime= System.currentTimeMillis();
    }
    public long getEntryTime(){return entryTime;}
    public ParkingSpot getParkingSpot(){return spot;}
    public Vehicle getVehicle(){return vehicle;}
    public long getArrivalTime(){return entryTime;}
}
