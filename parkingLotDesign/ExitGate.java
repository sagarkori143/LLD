package parkingLotDesign;

public class ExitGate {
    public void processExit(Ticket ticket){
        // put some logic to calculate the hours..
        // do the payments using some strategy
        // then clear the parking spot
        ticket.getParkingSpot().removeVehicle();
    }
}
