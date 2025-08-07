// I want a factory system for the vehicles. First I will create an interface for the concrete vehicle class to
// tell them what they need to implement mandatoriyly

interface VehicleModel {
    // You need a function to tell what type of model you are
    public void tellModel();
};

class BMW implements VehicleModel {
    public void tellModel() {
        System.out.println("This is BMW babyyy!");
    }
};

class Audi implements VehicleModel {
    public void tellModel() {
        System.out.println("This is Audi!!");
    }
};

class VehicleFactory {
    VehicleModel vehicle;

    VehicleModel createVehicle(String vehicleType) {
        if (vehicleType == "Audi") {
            vehicle = new Audi();
        } else if (vehicleType == "BMW") {
            vehicle = new BMW();
        }
        return vehicle;
    }
};

public class Main {
    public static void main(String[] args) {
        // I cant do these checks
        // String model="BMWW";
        // VehicleModel vehicle;
        // if(model=="Audi"){
        // vehicle= new Audi();
        // vehicle.tellModel();
        // }else if(model=="BMW"){
        // vehicle= new BMW();
        // vehicle.tellModel();
        // }else{
        // System.out.println("No such vehicle type found!");
        // }
        // I cannot write these checks because this creation of the vehicles class
        // objects might be at 100
        // places in the code and I cannot go and change all of them if any modificaiton
        // needed. So what I do is
        // I create a factory subclass that has-a(bcz in the main fucn tehre will be an
        // instance used of this)
        // relation with the main code. WE just instantiate the factory
        // and provide it with the type of vehicle we need and rest is handled inside
        // factory. It becomes the ?
        // central point and in future if we want to make any chagnes.. we will do it
        // here in the factory and
        // not at all teh 100 places.

        // instantiate a factory 
        VehicleFactory factory= new VehicleFactory();
        VehicleModel vehicle= factory.createVehicle("Audi");
        vehicle.tellModel();
    }
}