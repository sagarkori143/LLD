#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

// ---------- Product ----------
class Product{
    public:
    string name;
    int price;      // in rupees
    Product(){}
    Product(string name, int price): name(name), price(price){}
};

// ---------- Inventory: slot code -> {product, quantity} ----------
class Inventory{
    unordered_map<int, pair<Product,int>> slots;   // code -> (product, qty)
    public:
    void addSlot(int code, Product p, int qty){ slots[code]={p,qty}; }
    bool hasStock(int code){
        return slots.count(code) && slots[code].second>0;
    }
    Product getProduct(int code){ return slots[code].first; }
    void reduceStock(int code){ slots[code].second--; }
    void display(){
        cout<<"---- Inventory ----\n";
        for(auto& s: slots)
            cout<<"  ["<<s.first<<"] "<<s.second.first.name
                <<" Rs."<<s.second.first.price<<" x"<<s.second.second<<"\n";
    }
};

// Forward declare so states can talk to the machine
class VendingMachine;

// ---------- State interface ----------
class State{
    public:
    virtual void insertCoin(VendingMachine* m, int amount)=0;
    virtual void selectProduct(VendingMachine* m, int code)=0;
    virtual void dispense(VendingMachine* m, int code)=0;
    virtual string name()=0;
    virtual ~State()=default;
};

// ---------- The machine (context) ----------
class VendingMachine{
    Inventory inventory;
    State* currentState;
    int balance=0;        // money inserted so far
    int selectedCode=-1;
    public:
    // states (declared, created below after their definitions)
    State* idleState;
    State* hasMoneyState;
    State* dispenseState;

    VendingMachine();     // defined after states exist

    // context helpers used by states
    Inventory& getInventory(){ return inventory; }
    void setState(State* s){ currentState=s; cout<<">> state -> "<<s->name()<<"\n"; }
    void addBalance(int amt){ balance+=amt; }
    int getBalance(){ return balance; }
    void resetBalance(){ balance=0; }
    void setSelected(int code){ selectedCode=code; }
    int getSelected(){ return selectedCode; }

    // public API (delegates to current state)
    void insertCoin(int amount){ currentState->insertCoin(this, amount); }
    void selectProduct(int code){ currentState->selectProduct(this, code); }
    void dispense(){ currentState->dispense(this, selectedCode); }
    void showInventory(){ inventory.display(); }
};

// ---------- Concrete states ----------
class IdleState: public State{
    public:
    string name() override{ return "IDLE"; }
    void insertCoin(VendingMachine* m, int amount) override{
        m->addBalance(amount);
        cout<<"Inserted Rs."<<amount<<" (balance Rs."<<m->getBalance()<<")\n";
        m->setState(m->hasMoneyState);
    }
    void selectProduct(VendingMachine*, int) override{
        cout<<"Insert money first.\n";
    }
    void dispense(VendingMachine*, int) override{
        cout<<"Insert money and select a product first.\n";
    }
};

class HasMoneyState: public State{
    public:
    string name() override{ return "HAS_MONEY"; }
    void insertCoin(VendingMachine* m, int amount) override{
        m->addBalance(amount);
        cout<<"Added Rs."<<amount<<" (balance Rs."<<m->getBalance()<<")\n";
    }
    void selectProduct(VendingMachine* m, int code) override{
        if(!m->getInventory().hasStock(code)){
            cout<<"Slot "<<code<<" is out of stock.\n";
            return;
        }
        Product p=m->getInventory().getProduct(code);
        if(m->getBalance()<p.price){
            cout<<"Not enough money for "<<p.name<<" (need Rs."<<p.price
                <<", have Rs."<<m->getBalance()<<")\n";
            return;
        }
        m->setSelected(code);
        cout<<"Selected "<<p.name<<"\n";
        m->setState(m->dispenseState);
        m->dispense();     // proceed to dispense
    }
    void dispense(VendingMachine*, int) override{
        cout<<"Select a product first.\n";
    }
};

class DispenseState: public State{
    public:
    string name() override{ return "DISPENSE"; }
    void insertCoin(VendingMachine*, int) override{
        cout<<"Please wait, dispensing.\n";
    }
    void selectProduct(VendingMachine*, int) override{
        cout<<"Please wait, dispensing.\n";
    }
    void dispense(VendingMachine* m, int code) override{
        Product p=m->getInventory().getProduct(code);
        m->getInventory().reduceStock(code);
        int change=m->getBalance()-p.price;
        cout<<"Dispensed "<<p.name<<". Change returned: Rs."<<change<<"\n";
        m->resetBalance();
        m->setSelected(-1);
        m->setState(m->idleState);
    }
};

// ---------- VendingMachine ctor (states exist now) ----------
VendingMachine::VendingMachine(){
    idleState=new IdleState();
    hasMoneyState=new HasMoneyState();
    dispenseState=new DispenseState();
    currentState=idleState;
}

// ---------- Demo ----------
int main(){
    VendingMachine m;
    m.getInventory().addSlot(1, Product("Coke", 40), 2);
    m.getInventory().addSlot(2, Product("Chips", 25), 1);
    m.getInventory().addSlot(3, Product("Water", 20), 0);   // out of stock
    m.showInventory();

    cout<<"\n== Buy Coke ==\n";
    m.insertCoin(50);
    m.selectProduct(1);          // dispenses Coke, returns Rs.10

    cout<<"\n== Not enough money ==\n";
    m.insertCoin(10);
    m.selectProduct(2);          // Chips cost 25, only 10 -> rejected

    cout<<"\n== Out of stock ==\n";
    m.insertCoin(20);
    m.selectProduct(3);          // Water out of stock

    cout<<"\n";
    m.showInventory();
    return 0;
}
