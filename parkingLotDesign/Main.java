package parkingLotDesign;


import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        // Create parking spots
        List<ParkingSpot> spots = new ArrayList<>();
        for(int i = 1; i <= 5; i++) {
            spots.add(new ParkingSpot("A" + i));
        }
        
        // Create factory and manager
        parkingSpotmanagerFactory factory = new parkingSpotmanagerFactory();
        ParkingSpotManager manager = factory.getSpotManager("fourWheeler", spots);
        
        // Create strategy and gates
        ParkingSpotStrategy strategy = new NearToLiftStrategy();
        EntryGate entryGate = new EntryGate();
        ExitGate exitGate = new ExitGate();
        
        // Test vehicle entry
        Vehicle car = new Vehicle("ABC123", "fourWheeler");
        Ticket ticket = entryGate.generateTicket(car, strategy, manager);
        
        if(ticket != null) {
            // Simulate some parking time
            try {
                Thread.sleep(3000); // 3 seconds
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            
            // Process exit
            exitGate.processExit(ticket);
        } else {
            System.out.println("No parking spot available!");
        }
    }
}
