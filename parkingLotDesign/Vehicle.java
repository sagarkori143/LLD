package parkingLotDesign;

public class Vehicle {
    private String vehicleNo;
    private String vehicleType;
    Vehicle(String number, String type){
        vehicleNo=number;
        vehicleType=type;
    }
    // NOw some getter and setters
    public String getVehicleNumber(){
        return vehicleNo;
    }
    public String getVehicleType(){
        return vehicleType;
    }
};

