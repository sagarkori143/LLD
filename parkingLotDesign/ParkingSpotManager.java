package parkingLotDesign;

import java.util.List;

public abstract class ParkingSpotManager {
    protected List<ParkingSpot> list;
    ParkingSpotManager(List<ParkingSpot> list){
        this.list=list;
    }
    public ParkingSpot getParkingSpot(ParkingSpotStrategy strategy){
        return strategy.findSpot(list);
    }
}
