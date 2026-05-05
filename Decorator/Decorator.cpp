#include <bits/stdc++.h>
using namespace std;

class Coffee{
    public:
    virtual int cost()=0;
    virtual string make()=0;
};
class SimpleCoffee: public Coffee{
    public:
    int cost(){
        return 100;
    }
    string make(){
        return "Make Simple Coffee";
    }
};
class ColdCoffee: public Coffee{
    public:
    int cost(){
        return 150;
    }
    string make(){
        return "Make Cold Coffee";
    }
};

class CoffeeDecorator:public Coffee{
    public:
    Coffee *coffee;
};

class MilkDecorator: public CoffeeDecorator{
    public:
    MilkDecorator(Coffee * co){
        this->coffee=co;
    }
    int cost(){
        return coffee->cost()+30;
    }
    string make(){
        return coffee->make()+" add Milk";
    }
};
class SugarDecorator: public CoffeeDecorator{
    public:
    SugarDecorator(Coffee * co){
        this->coffee=co;
    }
    int cost(){
        return coffee->cost()+10;
    }
    string make(){
        return coffee->make()+" add Sugar";
    }
};

class ChocolateDecorator: public CoffeeDecorator{
    public:
    ChocolateDecorator(Coffee * co){
        this->coffee=co;
    }
    int cost(){
        return coffee->cost()+25;
    }
    string make(){
        return coffee->make()+" add Chocolate";
    }
};

int main()
{
    Coffee* co= new SugarDecorator(new MilkDecorator(new SimpleCoffee()));
    cout<<co->make()<<" costs "<<co->cost()<<endl;
    co= new ChocolateDecorator(new MilkDecorator(new ColdCoffee()));
    cout<<co->make()<<" costs "<<co->cost()<<endl;
    return 0;
}