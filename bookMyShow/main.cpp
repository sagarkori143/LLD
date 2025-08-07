#include <bits/stdc++.h>
using namespace std;

enum class SeatCategory
{
    Economy,
    Silver,
    Gold
};

enum class City
{
    Delhi,
    Saharanpur,
    Indore
};

enum class SeatStatus
{
    Available,
    Booked,
    Blocked
};

enum class BookingStatus
{
    Confirmed,
    Cancelled,
    Pending
};

// Forward declarations
class Show;
class Theatre;

class Seat
{
private:
    int seatID;
    int row;
    int seatNumber;
    SeatCategory category;
    SeatStatus status;
    double price;

public:
    Seat(int id, int row, int seatNum, SeatCategory category, double price)
    {
        seatID = id;
        this->row = row;
        this->seatNumber = seatNum;
        this->category = category;
        this->status = SeatStatus::Available;
        this->price = price;
    }
    
    // Getters
    int getSeatID() { return seatID; }
    int getSeatRow() { return row; }
    int getSeatNumber() { return seatNumber; }
    SeatCategory getSeatCategory() { return category; }
    SeatStatus getStatus() { return status; }
    double getPrice() { return price; }
    
    // Setters
    void setStatus(SeatStatus newStatus) { status = newStatus; }
    void setPrice(double newPrice) { price = newPrice; }
    
    // Helper methods
    bool isAvailable() { return status == SeatStatus::Available; }
    void bookSeat() { status = SeatStatus::Booked; }
    void releaseSeat() { status = SeatStatus::Available; }
};

class Screen
{
private:
    int screenID;
    vector<Show*> shows;
    vector<Seat*> seats;
    int totalSeats;

public:
    // Default constructor
    Screen(){};
    Screen(int id, int totalSeats) : screenID(id), totalSeats(totalSeats) {
        initializeSeats();
    }
    
    void initializeSeats() {
        // Initialize seats with different categories and prices
        int seatID = 1;
        int economyRows = 5, silverRows = 3, goldRows = 2;
        int seatsPerRow = totalSeats / (economyRows + silverRows + goldRows);
        
        // Economy seats
        for (int row = 1; row <= economyRows; row++) {
            for (int seatNum = 1; seatNum <= seatsPerRow; seatNum++) {
                Seat* seat = new Seat(seatID++, row, seatNum, SeatCategory::Economy, 150.0);
                seats.push_back(seat);
            }
        }
        
        // Silver seats
        for (int row = economyRows + 1; row <= economyRows + silverRows; row++) {
            for (int seatNum = 1; seatNum <= seatsPerRow; seatNum++) {
                Seat* seat = new Seat(seatID++, row, seatNum, SeatCategory::Silver, 250.0);
                seats.push_back(seat);
            }
        }
        
        // Gold seats
        for (int row = economyRows + silverRows + 1; row <= economyRows + silverRows + goldRows; row++) {
            for (int seatNum = 1; seatNum <= seatsPerRow; seatNum++) {
                Seat* seat = new Seat(seatID++, row, seatNum, SeatCategory::Gold, 400.0);
                seats.push_back(seat);
            }
        }
    }
    
    // Getters
    int getScreenID() { return screenID; }
    vector<Show*> getAllShows() { return shows; }
    vector<Seat*> getAllSeats() { return seats; }
    
    // Show management
    void addShow(Show* show) {
        shows.push_back(show);
        // Add all seats to the show
        for (Seat* seat : seats) {
            show->addSeat(seat);
        }
    }
    
    void removeShow(int showID) {
        shows.erase(
            remove_if(shows.begin(), shows.end(),
                [showID](Show* show) { return show->getShowID() == showID; }),
            shows.end());
    }
    
    Show* getShowByID(int showID) {
        for (Show* show : shows) {
            if (show->getShowID() == showID) {
                return show;
            }
        }
        return nullptr;
    }
};

class Theatre
{
private:
    int theatreID;
    string theatreName;
    string address;
    vector<Show*> shows;
    vector<Screen*> screens;
    City city;

public:
    Theatre(int id, string name, string addr, City city) 
        : theatreID(id), theatreName(name), address(addr), city(city) {}
    
    // Getters
    int getTheatreID() { return theatreID; }
    string getTheatreName() { return theatreName; }
    string getAddress() { return address; }
    City getCity() { return city; }
    vector<Show*> getAllShows() { return shows; }
    vector<Screen*> getAllScreens() { return screens; }
    
    // Screen management
    void addScreen(Screen* screen) {
        screens.push_back(screen);
    }
    
    // Show management
    void addShow(Show* show) {
        shows.push_back(show);
    }
    
    vector<Show*> getShowsByMovie(string movieName) {
        vector<Show*> movieShows;
        for (Show* show : shows) {
            if (show->getMovie().getMovieName() == movieName) {
                movieShows.push_back(show);
            }
        }
        return movieShows;
    }
    
    Show* getShowByID(int showID) {
        for (Show* show : shows) {
            if (show->getShowID() == showID) {
                return show;
            }
        }
        return nullptr;
    }
};

class Movie
{
private:
    string name;
    int duration;
    string cast;
    string genre;
    double rating;

public:
    // Default constructor
    Movie(){};
    Movie(string name, int duration, string cast, string genre = "", double rating = 0.0)
    {
        this->name = name;
        this->duration = duration;
        this->cast = cast;
        this->genre = genre;
        this->rating = rating;
    }
    
    // Getters
    string getMovieName() const { return name; }
    int getMovieDuration() const { return duration; }
    string getMovieCast() const { return cast; }
    string getGenre() const { return genre; }
    double getRating() const { return rating; }
    
    // Setters
    void setRating(double newRating) { rating = newRating; }
    
    // Operator overloading for set operations
    bool operator<(const Movie& other) const {
        return this->name < other.name;
    }
    
    bool operator==(const Movie& other) const {
        return this->name == other.name;
    }
};

class MovieController
{
private:
    set<Movie> movies;
    unordered_map<City, set<Movie>> cityToMovie;

public:
    MovieController() {};
    
    void addMovie(Movie movie, City city)
    {
        movies.insert(movie);
        cityToMovie[city].insert(movie);
    }
    
    void removeMovie(Movie movie, City city)
    {
        movies.erase(movie);
        cityToMovie[city].erase(movie);
    }
    
    vector<Movie> getAllMovies()
    {
        vector<Movie> temp(movies.begin(), movies.end());
        return temp;
    }
    
    vector<Movie> getMoviesByCity(City city)
    {
        vector<Movie> temp(cityToMovie[city].begin(), cityToMovie[city].end());
        return temp;
    }
    
    Movie* findMovie(string movieName) {
        for (const auto& movie : movies) {
            if (movie.getMovieName() == movieName) {
                return const_cast<Movie*>(&movie);
            }
        }
        return nullptr;
    }
};

class TheatreController
{
private:
    unordered_map<City, vector<Theatre*>> cityToTheatres;
    vector<Theatre*> allTheatres;

public:
    void addTheatre(Theatre* theatre, City city) {
        allTheatres.push_back(theatre);
        cityToTheatres[city].push_back(theatre);
    }
    
    vector<Theatre*> getTheatresByCity(City city) {
        return cityToTheatres[city];
    }
    
    vector<Theatre*> getAllTheatres() {
        return allTheatres;
    }
    
    Theatre* getTheatreByID(int theatreID) {
        for (Theatre* theatre : allTheatres) {
            if (theatre->getTheatreID() == theatreID) {
                return theatre;
            }
        }
        return nullptr;
    }
};
class Show
{
private:
    string startTime;
    string endTime;
    int showID;
    Movie movie;
    map<int, Seat*> seatMap; // seatID to Seat mapping
    Screen* screen;

public:
    Show(){} // Default constructor
    Show(int ID, string start, string end, Movie movie, Screen* screenPtr)
    {
        this->startTime = start;
        this->endTime = end;
        this->showID = ID;
        this->movie = movie;
        this->screen = screenPtr;
    }
    
    // Getters
    int getShowID() { return showID; }
    Movie getMovie() { return movie; }
    string getStartTime() { return startTime; }
    string getEndTime() { return endTime; }
    Screen* getScreen() { return screen; }
    
    // Seat management
    void addSeat(Seat* seat) {
        seatMap[seat->getSeatID()] = seat;
    }
    
    vector<Seat*> getAvailableSeats() {
        vector<Seat*> available;
        for (auto& pair : seatMap) {
            if (pair.second->isAvailable()) {
                available.push_back(pair.second);
            }
        }
        return available;
    }
    
    vector<Seat*> getBookedSeats() {
        vector<Seat*> booked;
        for (auto& pair : seatMap) {
            if (pair.second->getStatus() == SeatStatus::Booked) {
                booked.push_back(pair.second);
            }
        }
        return booked;
    }
    
    bool bookSeats(vector<int> seatIDs) {
        // Check if all seats are available
        for (int seatID : seatIDs) {
            if (seatMap.find(seatID) == seatMap.end() || !seatMap[seatID]->isAvailable()) {
                return false;
            }
        }
        
        // Book all seats
        for (int seatID : seatIDs) {
            seatMap[seatID]->bookSeat();
        }
        return true;
    }
    
    void releaseSeats(vector<int> seatIDs) {
        for (int seatID : seatIDs) {
            if (seatMap.find(seatID) != seatMap.end()) {
                seatMap[seatID]->releaseSeat();
            }
        }
    }
    
    double calculateTotalPrice(vector<int> seatIDs) {
        double total = 0.0;
        for (int seatID : seatIDs) {
            if (seatMap.find(seatID) != seatMap.end()) {
                total += seatMap[seatID]->getPrice();
            }
        }
        return total;
    }
};

class User
{
private:
    int userID;
    string name;
    string email;
    string phoneNumber;

public:
    User(int id, string name, string email, string phone) 
        : userID(id), name(name), email(email), phoneNumber(phone) {}
    
    // Getters
    int getUserID() { return userID; }
    string getName() { return name; }
    string getEmail() { return email; }
    string getPhoneNumber() { return phoneNumber; }
};

class Payment
{
private:
    int paymentID;
    double amount;
    string paymentMethod;
    string paymentDate;
    bool isCompleted;

public:
    Payment(int id, double amt, string method, string date) 
        : paymentID(id), amount(amt), paymentMethod(method), paymentDate(date), isCompleted(false) {}
    
    bool processPayment() {
        // Simulate payment processing
        cout << "Processing payment of $" << amount << " via " << paymentMethod << endl;
        isCompleted = true;
        return true;
    }
    
    // Getters
    int getPaymentID() { return paymentID; }
    double getAmount() { return amount; }
    bool getPaymentStatus() { return isCompleted; }
};

class Booking
{
private:
    int bookingID;
    User* user;
    Show* show;
    vector<Seat*> bookedSeats;
    Payment* payment;
    BookingStatus status;
    string bookingDate;
    double totalAmount;

public:
    Booking(int id, User* user, Show* show, vector<int> seatIDs, string date) 
        : bookingID(id), user(user), show(show), bookingDate(date), payment(nullptr) {
        
        status = BookingStatus::Pending;
        
        // Get seat objects from seat IDs
        for (int seatID : seatIDs) {
            for (Seat* seat : show->getScreen()->getAllSeats()) {
                if (seat->getSeatID() == seatID) {
                    bookedSeats.push_back(seat);
                    break;
                }
            }
        }
        
        totalAmount = show->calculateTotalPrice(seatIDs);
    }
    
    bool confirmBooking(string paymentMethod) {
        vector<int> seatIDs;
        for (Seat* seat : bookedSeats) {
            seatIDs.push_back(seat->getSeatID());
        }
        
        // Try to book seats
        if (show->bookSeats(seatIDs)) {
            // Process payment
            payment = new Payment(bookingID * 100, totalAmount, paymentMethod, bookingDate);
            if (payment->processPayment()) {
                status = BookingStatus::Confirmed;
                cout << "Booking confirmed! Booking ID: " << bookingID << endl;
                return true;
            }
        }
        
        status = BookingStatus::Cancelled;
        return false;
    }
    
    void cancelBooking() {
        if (status == BookingStatus::Confirmed) {
            vector<int> seatIDs;
            for (Seat* seat : bookedSeats) {
                seatIDs.push_back(seat->getSeatID());
            }
            show->releaseSeats(seatIDs);
            status = BookingStatus::Cancelled;
            cout << "Booking cancelled successfully!" << endl;
        }
    }
    
    // Getters
    int getBookingID() { return bookingID; }
    User* getUser() { return user; }
    Show* getShow() { return show; }
    vector<Seat*> getBookedSeats() { return bookedSeats; }
    BookingStatus getStatus() { return status; }
    double getTotalAmount() { return totalAmount; }
    
    void printBookingDetails() {
        cout << "\n=== Booking Details ===" << endl;
        cout << "Booking ID: " << bookingID << endl;
        cout << "User: " << user->getName() << endl;
        cout << "Movie: " << show->getMovie().getMovieName() << endl;
        cout << "Show Time: " << show->getStartTime() << " - " << show->getEndTime() << endl;
        cout << "Seats: ";
        for (Seat* seat : bookedSeats) {
            cout << "Row " << seat->getSeatRow() << " Seat " << seat->getSeatNumber() << " ";
        }
        cout << endl;
        cout << "Total Amount: $" << totalAmount << endl;
        cout << "Status: " << (status == BookingStatus::Confirmed ? "Confirmed" : 
                              status == BookingStatus::Cancelled ? "Cancelled" : "Pending") << endl;
    }
};