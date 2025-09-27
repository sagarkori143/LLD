
#include<mutex>
#include<chrono>
#include<thread>
#include<bits/stdc++.h>
using namespace std;

enum class SeatStatus { AVAILABLE, LOCKED, BOOKED };

struct Seat {
    int id;
    SeatStatus status;
    string lockedBy;
    chrono::steady_clock::time_point lockExpire;
};

Seat seat = {1, SeatStatus::AVAILABLE, "", {}};
mutex seatMutex;


void tryLocking(string user, int second){
    lock_guard<mutex> lg(mutex);
    if(seat.status==SeatStatus::LOCKED && seat.lockExpire>chrono:: steady_clock:: now()){
        seat.status=SeatStatus:: AVAILABLE;
        seat.lockedBy="";
    }

    if(seat.status==SeatStatus::AVAILABLE){
        seat.status=SeatStatus::LOCKED;
        seat.lockedBy=user;
        seat.lockExpire=chrono::steady_clock:: now()+ chrono::seconds(second);
    }else{
        cout<<"the seat is already locked"<<endl;
    }
}

void tryBooking(Seat seat,string user){
    if(seat.status==SeatStatus::LOCKED && seat.lockedBy==user){
        cout<<"Your seat has been seccessfully booked!!"<<endl;
    }else{
        cout<<"The seat has been locked by someone else"<<endl;
    }
}

void userBookingFlow(string user) {
    if (tryLocking(user, 3)) {
        // Simulate payment time
        this_thread::sleep_for(chrono::seconds(2));
        tryBooking(user);
    }
}

void main(){
 thread t1(userBookingFlow,"Sagar");
 thread t2(userBookingFlow, "Omkar");
 t1.join();
 t2.join();
 return;
}
