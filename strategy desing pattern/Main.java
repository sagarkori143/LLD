// Interface
 interface drivestrategy {
    void drive();
}

// Vehicle base class
 class vehicle {
    drivestrategy driver;

    vehicle(drivestrategy strategy){
        this.driver = strategy;
    }

    void drive(){
        driver.drive();  // Delegate to the strategy
    }
}

// A strategy implementation
class offRoadStrategy implements drivestrategy {
    @Override
    public void drive(){
        System.out.println("off road strategy hai mitttrrr!!");
    }
}

// A specific vehicle using the strategy
 class offRoadVehicle extends vehicle {
    offRoadVehicle(){
        super(new offRoadStrategy());
    }
}

// Main method
//public class Main {
//     static void main(String[] args) {
//        vehicle newVehicle = new offRoadVehicle();
//        newVehicle.drive();  // This will now print: off road strategy hai mitttrrr!!
//    }
//}
