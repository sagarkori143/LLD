package parkingLotDesign;

public class EntryGate {
    public Ticket generateTicket(Vehicle vehicle, ParkingSpotStrategy strategy, ParkingSpotManager manager){
        ParkingSpot spot= manager.getParkingSpot(strategy);
        if(spot!=null){
            spot.assignVehicle(vehicle);
            System.out.println("We have assigned"+spot.getID()+"for your vehicle!");
            return new Ticket(spot, vehicle);
        }
        return null;
    }
}
