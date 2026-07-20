#include<bits/stdc++.h>
using namespace std;

// ---------- Content: abstract, each type builds its own message ----------
class Notification{
    protected:
    int notificationId;
    public:
    Notification(int id): notificationId(id){}
    int getId(){ return notificationId; }
    virtual string getContent()=0;          // each subtype defines its content
    virtual ~Notification()=default;
};

class OrderNotification: public Notification{
    string orderId, status;
    public:
    OrderNotification(int id, string order, string status)
        : Notification(id), orderId(order), status(status){}
    string getContent() override{
        return "Order "+orderId+" is now "+status;
    }
};

class PromoNotification: public Notification{
    int discount; string code;
    public:
    PromoNotification(int id, int discount, string code)
        : Notification(id), discount(discount), code(code){}
    string getContent() override{
        return "Sale: "+to_string(discount)+"% off! Use code "+code;
    }
};

// ---------- Channels: how it's delivered (Strategy) ----------
class NotificationChannel{
    public:
    virtual void send(Notification* n)=0;
    virtual ~NotificationChannel()=default;
};

class EmailChannel: public NotificationChannel{
    string address;
    public:
    EmailChannel(string addr): address(addr){}
    void send(Notification* n) override{
        cout<<"[EMAIL -> "<<address<<"] "<<n->getContent()<<endl;
    }
};

class SMSChannel: public NotificationChannel{
    string phone;
    public:
    SMSChannel(string p): phone(p){}
    void send(Notification* n) override{
        cout<<"[SMS -> "<<phone<<"] "<<n->getContent()<<endl;
    }
};

class PushChannel: public NotificationChannel{
    string device;
    public:
    PushChannel(string d): device(d){}
    void send(Notification* n) override{
        cout<<"[PUSH -> "<<device<<"] "<<n->getContent()<<endl;
    }
};

// ---------- Observer: a subscriber that reacts to a notification ----------
class Observer{
    public:
    virtual void update(Notification* n)=0;
    virtual ~Observer()=default;
};

class User: public Observer{
    string name;
    vector<NotificationChannel*> channels;   // how this user wants to be reached
    public:
    User(string name): name(name){}
    void addChannel(NotificationChannel* c){ channels.push_back(c); }
    void update(Notification* n) override{
        cout<<"-- Notifying "<<name<<" --"<<endl;
        for(auto c: channels) c->send(n);
    }
};

// ---------- Subject: the service that fans out ----------
class NotificationService{
    vector<Observer*> subscribers;
    int nextId=1;
    public:
    void subscribe(Observer* o){ subscribers.push_back(o); }
    void unsubscribe(Observer* o){
        subscribers.erase(remove(subscribers.begin(), subscribers.end(), o), subscribers.end());
    }
    int nextNotificationId(){ return nextId++; }
    void notifyAll(Notification* n){
        for(auto s: subscribers) s->update(n);
    }
};

int main(){
    NotificationService service;

    User* alice=new User("Alice");
    alice->addChannel(new EmailChannel("alice@mail.com"));
    alice->addChannel(new PushChannel("alice-phone"));

    User* bob=new User("Bob");
    bob->addChannel(new SMSChannel("+91-99999"));

    service.subscribe(alice);
    service.subscribe(bob);

    service.notifyAll(new OrderNotification(service.nextNotificationId(), "A123", "shipped"));

    service.unsubscribe(bob);
    service.notifyAll(new PromoNotification(service.nextNotificationId(), 50, "SAVE50"));
    return 0;
}
