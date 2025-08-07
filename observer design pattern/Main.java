import java.util.ArrayList;
import java.util.List;

interface Observer {
    void update();
}

class EmailObserver implements Observer{
    // I can also recieve the observable while creating the
     StockObservable stockObservable;
     String userEmail;
    public EmailObserver(String email, StockObservable observable){
        stockObservable=observable;
        userEmail=email;
    }
    @Override
    public void update(){
        System.out.println("Email Observer saying that stock has been updated on this email: "+userEmail);
    }
}

class PhoneObserver implements Observer{
    String userPhone;
    StockObservable stockObservable;
    PhoneObserver(String phoneNumber, StockObservable observable){
        userPhone=phoneNumber;
        stockObservable=observable;
    }
    @Override
    public void update(){
        System.out.println("Phone Observer saying that stock has been updated on this number: "+ userPhone);
    }
}

interface StockObservable {
    // it will contain the following methods that ara required to be overridden int the implementation
    void add(Observer o);
    void remove(Observer o);
    void notifyObservers();
    void setStock(int newStock);
}

// nopw I will use this interface to create a new class for Iphone

class IphoneObservable implements StockObservable {
    // it needs to override all those methods
    int currStock = 0;
    List<Observer> observerList= new ArrayList<>();
    @Override
    public void add(Observer o){
        //  add it to the list. Where is the list. create it
        observerList.add(o);
    }
    @Override
    public void remove(Observer o){
              observerList.remove(o);
    }
    @Override
    public void setStock(int newStock){
        if(currStock==0){
            notifyObservers();
        }
        currStock+= newStock;
    }
    @Override
    public void notifyObservers(){
        for(Observer o: observerList){
            o.update();
        }
    }
}

// Now I can write a main function to use tall these implemnetnasjflhf
//public class Main{
//    public static void main(String[] args){
//        IphoneObservable iphone = new IphoneObservable();
//
//        EmailObserver email = new EmailObserver("sagarkoriup11@gmail.com",iphone);
//        PhoneObserver phone = new PhoneObserver("9917870933", iphone);
//
//        iphone.add(email);
//        iphone.add(phone);
//
//        iphone.setStock(100);
//
//    }
//}