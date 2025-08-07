package parkingLotDesign;

import java.util.List;

public class parkingSpotmanagerFactory {
    
    public ParkingSpotManager getSpotManager(String vehicleType, List<ParkingSpot> list){
        if("twoWheeler".equals(vehicleType)){
            return new TwoWheelerParkingSpotManager(list);
        }else if("fourWheeler".equals(vehicleType)){
            return new fourWheelerParkingSpotManager(list);
        }
        return null;
    }
}
