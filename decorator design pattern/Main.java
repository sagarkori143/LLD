// for decorator design pattern we need an abstract class.. why? lest say we are creating decorator for different
// kind of pizzas. Now we need a base pizza and for that I will create a base abstract class.. Why class and why not
// interface? because in interface we can only have contracts or signature of the methods adn we cannot add any data
// member of any implementation. Thats why we will create an abstract class and creat an abstract function cost
// so that it can be overridden.

abstract class BasePizza{
    public abstract int cost();
}

// Now I will create many pizzaas using this base pizza and than add a decorator for the toppings
 
class Margherita extends BasePizza{
    @Override
    public int cost(){
        return 40;
    }
}

class VegDelight extends BasePizza{
  @Override
  public int cost(){
    return 50;
  }

}

// Now I want toppings on my pizzas. For that I will craete a decorator. A decorated works two ways. It extend or inherits a class and at the same time its childrens
// use the parent class as a data member that means it has both the is-a relation and the has-a a relation. 

 abstract class ToppingsDecorator extends BasePizza{
  BasePizza pizza;
  ToppingsDecorator(BasePizza pizza){
    this.pizza=pizza;
  }
}

class Cheese extends ToppingsDecorator{
  Cheese(BasePizza pizza){
    super(pizza);
  }
  @Override
  public int cost(){
    return pizza.cost()+20;
  }
}

class Mushroom extends ToppingsDecorator{
  Mushroom(BasePizza pizza){
    super(pizza);
  }
  @Override
  public int cost(){
    return pizza.cost()+30;
  }
}

// Now I can add these toppings to my pizza

public class Main{
  public static void main(String[] args){
    
    BasePizza myPizza= new Mushroom(new Cheese(new Margherita()));
    int cost=myPizza.cost();
    System.out.println(cost);
  }
}



