#include<bits/stdc++.h>
using namespace std;

class basePizza{
    public:
    virtual int cost()=0;
};

class Margherita: public basePizza{
    public:
    int cost(){
        return 40;
    }
};

class ToppingDecorator: public basePizza{
    protected:
    basePizza* pizza;
    public:
    ToppingDecorator(basePizza* pizza){
        this-> pizza=pizza;
    }
    
};

class Cheese: public ToppingDecorator{
    public:
    Cheese(basePizza* pizza): ToppingDecorator(pizza){
    }
    int cost(){
        return pizza->cost()+20;
    }
};

class Mushroom : public ToppingDecorator{
    public:
    
};

main(){
    basePizza* pizza= new Cheese(new Margherita());
    cout<<pizza->cost();
}


