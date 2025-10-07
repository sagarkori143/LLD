// Simple Observer Pattern Example (C++)
// - Subject (Observable) maintains list of observers and notifies them on state change
// - Observer defines update interface which concrete observers implement
// This is a minimal, educational implementation.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Observer interface
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(const string &message) = 0;
};

// Subject (Observable) interface
class ISubject {
public:
    virtual ~ISubject() = default;
    virtual void attach(IObserver *observer) = 0;
    virtual void detach(IObserver *observer) = 0;
    virtual void notify(const string &message) = 0;
};

// Concrete Subject: News Agency
class NewsAgency : public ISubject {
private:
    vector<IObserver *> observers;

public:
    void attach(IObserver *observer) override {
        observers.push_back(observer);
    }

    void detach(IObserver *observer) override {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notify(const string &message) override {
        for (auto obs : observers) {
            if (obs) obs->update(message);
        }
    }

    // Business logic that changes state and notifies observers
    void setNews(const string &news) {
        cout << "NewsAgency: New headline -> " << news << "\n";
        notify(news);
    }
};

// Concrete Observer: News Channel
class NewsChannel : public IObserver {
private:
    string name;

public:
    explicit NewsChannel(string n) : name(move(n)) {}

    void update(const string &message) override {
        cout << "[" << name << "] Breaking: " << message << "\n";
    }
};

// Demo
int main() {
    cout << "--- Observer Pattern Demo ---\n";

    NewsAgency agency;

    NewsChannel channel1("Channel One");
    NewsChannel channel2("Channel Two");
    NewsChannel channel3("Channel Three");

    // Observers subscribe
    agency.attach(&channel1);
    agency.attach(&channel2);

    // Publish news
    agency.setNews("Local elections scheduled for next month.");

    // Add another observer
    agency.attach(&channel3);
    agency.setNews("Severe weather warning issued for the coast.");

    // Unsubscribe one
    agency.detach(&channel2);
    agency.setNews("Sports team wins the regional championship.");

    cout << "--- Demo complete ---\n";
    return 0;
}
