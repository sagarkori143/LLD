// bookMyShow.cpp — many people book the SAME seat, only ONE wins.
// Build & run:  g++ -std=c++17 -pthread bookMyShow.cpp -o bms && ./bms

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

vector<bool> booked(10, false);   // shared: is seat i booked?
mutex mtx;                        // guards booked

void bookSeat(int seat, string user) {
    lock_guard<mutex> guard(mtx);          // one thread at a time

    if (booked[seat]) {
        cout << user << " FAILED\n";
        return;
    }
    booked[seat] = true;
    cout << user << " SUCCESS\n";
}   // lock auto-released

int main() {
    vector<thread> users;
    for(int i=0; i<8; i++){
        users.push_back(thread(bookSeat,5,"Sagar"));
    }
    for(auto & t: users)t.join();
}
