#include <bits/stdc++.h>
using namespace std;

class DiscountStrategy{
public:
    virtual ~DiscountStrategy() = default;

    virtual double calculate(double baseAmount)=0;
};

class FlatStrategy:public DiscountStrategy{
    double flat;
public:
    FlatStrategy(double flat){
        this->flat=flat;
    }

    double calculate(double baseAmount) override{
        return min(flat,baseAmount);
    }
};

class PercentStrategy:public DiscountStrategy{
    double percent;

public:
    PercentStrategy(double percent){
        this->percent=percent;
    }

    double calculate(double baseAmount) override{
        return baseAmount*percent/100;
    }
};

class PercentCapStrategy:public DiscountStrategy{
    double percent;
    double cap;

public:
    PercentCapStrategy(double percent, double cap){
        this->percent=percent;
        this->cap=cap;
    }

    double calculate(double baseAmount) override{
        return min(cap,baseAmount*percent/100);
    }
};

enum class StrategyType{
    FLAT,
    PERCENT,
    PERCENTCAP
};

class StrategyManager{
private:
    static StrategyManager* instance;

    StrategyManager(){}

    StrategyManager(const StrategyManager&)= delete;
    StrategyManager& operator=(const StrategyManager&)= delete;

public:
    static StrategyManager* getInstance(){
        if(!instance){
            instance= new StrategyManager();
        }

        return instance;
    }

    DiscountStrategy* getStrategy(
        StrategyType type,
        double param1,
        double param2=0
    ){
        if(type==StrategyType::FLAT){
            return new FlatStrategy(param1);

        }else if(type==StrategyType::PERCENT){
            return new PercentStrategy(param1);

        }else{
            return new PercentCapStrategy(param1,param2);
        }

        return nullptr;
    }
};

StrategyManager* StrategyManager::instance=nullptr;

class Product{
private:
    string name;
    string category;
    double price;

public:
    Product(string name,string category, double price){
        this->name=name;
        this->category=category;
        this->price=price;
    }

    string getName() const{
        return name;
    }

    string getCategory() const{
        return category;
    }

    double getPrice() const{
        return price;
    }
};

class CartItem{
private:
    Product* product;
    int quantity;

public:
    CartItem(Product* prod,int qty){
        product=prod;
        quantity=qty;
    }

    double itemTotal() const{
        return product->getPrice()*quantity;
    }

    Product* getProduct() const{
        return product;
    }
};

class Cart{
private:
    vector<CartItem*> items;

    double originalTotal;
    double currentTotal;

    bool loyaltyMember;
    string paymentBank;

public:
    Cart(){
        originalTotal=0.0;
        currentTotal=0.0;
        loyaltyMember=false;
        paymentBank="";
    }

    ~Cart(){
        for(auto item: items){
            delete item;
        }
    }

    void addProduct(Product* prod,int qty=1){
        CartItem* item= new CartItem(prod,qty);

        items.push_back(item);

        originalTotal+=item->itemTotal();
        currentTotal+= item->itemTotal();
    }

    double getOriginalTotal() const{
        return originalTotal;
    }

    double getCurrentTotal() const{
        return currentTotal;
    }

    void applyDiscount(double d){
        currentTotal-=d;

        if(currentTotal<0){
            currentTotal=0;
        }
    }

    void setLoyaltyMember(bool member){
        loyaltyMember=member;
    }

    bool isLoyaltyMember() const{
        return loyaltyMember;
    }

    void setPaymentBank(string bank){
        paymentBank=bank;
    }
    string getPaymentBank() const{
        return paymentBank;
    }
    const vector<CartItem*>& getItems() const{
        return items;
    }
};

class Coupon{
private:
    Coupon*next;

protected:
    DiscountStrategy* strat;

public:
    Coupon(){
        next=nullptr;
        strat=nullptr;
    }

    virtual ~Coupon(){

        delete strat;

        if(next){
            delete next;
        }
    }

    void setNext(Coupon* nxt){
        next=nxt;
    }

    Coupon* getNext() const{
        return next;
    }

    void applyDiscount(Cart* cart){

        if(isApplicable(cart)){

            double discount= getDiscount(cart);

            cart->applyDiscount(discount);

            cout<< name()<<" applied: "<<discount<<endl;

            if(!isCombinable()){
                return;
            }
        }

        if(next){
            next->applyDiscount(cart);
        }
    }

    virtual bool isApplicable(Cart* cart) const =0;

    virtual double getDiscount(Cart* cart)=0;

    virtual bool isCombinable() const{
        return true;
    }

    virtual string name() const =0;
};

//------------------------------------------

class SeasonalOffer: public Coupon{
private:
    double percent;
    string category;

public:
    SeasonalOffer(double pct, string cat){
        percent=pct;
        category=cat;

        strat=StrategyManager::getInstance()
                ->getStrategy(StrategyType::PERCENT,percent);
    }

    bool isApplicable(Cart* cart) const override{

        for(CartItem* item: cart->getItems()){

            if(item->getProduct()->getCategory()== category){
                return true;
            }
        }

        return false;
    }

    double getDiscount(Cart* cart) override{

        double subTotal=0.0;

        for(CartItem* item: cart->getItems()){

            if(item->getProduct()->getCategory() == category){
                subTotal+=item->itemTotal();
            }
        }

        return strat->calculate(subTotal);
    }

    bool isCombinable() const override{
        return true;
    }

    string name() const override{
        return "Seasonal Offer "
               +to_string((int)percent)
               +" % off "
               + category;
    }
};

class LoyaltyDiscount: public Coupon{
private:
    double percent;

public:
    LoyaltyDiscount(double pct){
        percent=pct;

        strat=StrategyManager::getInstance()
                ->getStrategy(StrategyType::PERCENT,percent);
    }

    bool isApplicable(Cart* cart) const override{
        return cart->isLoyaltyMember();
    }

    double getDiscount(Cart* cart) override{
        return strat->calculate(cart->getCurrentTotal());
    }

    string name() const override{
        return "Loyalty Discount "
               +to_string((int)percent)
               +" % off";
    }
};

class BulkCoupon: public Coupon{
private:
    double threshold;
    double flatOff;

public:
    BulkCoupon(double thresh,double flat){
        threshold=thresh;
        flatOff=flat;

        strat=StrategyManager::getInstance()
                ->getStrategy(StrategyType::FLAT,flatOff);
    }

    bool isApplicable(Cart* cart) const override{
        return cart->getOriginalTotal()>=threshold;
    }

    double getDiscount(Cart* cart) override{
        return strat->calculate(cart->getCurrentTotal());
    }

    string name() const override{
        return "Bulk Coupon Rs "
               +to_string((int)flatOff)
               +" off";
    }
};

class BankingCoupon: public Coupon{
private:
    string bank;

    double minSpend;
    double percent;
    double offCap;

public:
    BankingCoupon(
        const string& b,
        double ms,
        double percent,
        double offCap
    ){
        bank=b;

        minSpend=ms;

        this->percent=percent;
        this->offCap=offCap;

        strat=StrategyManager::getInstance()
                ->getStrategy(
                    StrategyType::PERCENTCAP,
                    percent,
                    offCap
                );
    }

    bool isApplicable(Cart* cart) const override{

        return cart->getPaymentBank()==bank
               &&
               cart->getOriginalTotal()>=minSpend;
    }

    double getDiscount(Cart* cart) override{
        return strat->calculate(cart->getCurrentTotal());
    }

    string name() const override{
        return "Bank Coupon "
               +to_string((int)percent)
               +"% off upto "
               + to_string((int) offCap);
    }
};

class CouponManager {

private:
    static CouponManager* instance;

    Coupon* head;

    CouponManager() {
        head = nullptr;
    }

public:

    static CouponManager* getInstance() {

        if (!instance) {
            instance = new CouponManager();
        }

        return instance;
    }

    void registerCoupon(Coupon* coupon) {

        if (!head) {

            head = coupon;

        } else {

            Coupon* cur = head;

            while (cur->getNext()) {
                cur = cur->getNext();
            }

            cur->setNext(coupon);
        }
    }

    vector<string> getApplicable(Cart* cart){

        vector<string> res;

        Coupon* cur = head;

        while (cur) {

            if (cur->isApplicable(cart)) {
                res.push_back(cur->name());
            }

            cur = cur->getNext();
        }

        return res;
    }

    double applyAll(Cart* cart) {

        if (head) {
            head->applyDiscount(cart);
        }

        return cart->getCurrentTotal();
    }

    ~CouponManager(){
        delete head;
    }
};

// Initialize static instance pointer
CouponManager* CouponManager::instance = nullptr;

// ----------------------------
// Main
// ----------------------------

int main() {

    CouponManager* mgr = CouponManager::getInstance();

    mgr->registerCoupon(new SeasonalOffer(10, "Clothing"));
    mgr->registerCoupon(new LoyaltyDiscount(5));
    mgr->registerCoupon(new BulkCoupon(1000, 100));
    mgr->registerCoupon(new BankingCoupon("ABC", 2000, 15, 500));

    Product* p1 = new Product("Winter Jacket", "Clothing", 1000);
    Product* p2 = new Product("Smartphone", "Electronics", 20000);
    Product* p3 = new Product("Jeans", "Clothing", 1000);
    Product* p4 = new Product("Headphones", "Electronics", 2000);

    Cart* cart = new Cart();

    cart->addProduct(p1, 1);
    cart->addProduct(p2, 1);
    cart->addProduct(p3, 2);
    cart->addProduct(p4, 1);

    cart->setLoyaltyMember(true);
    cart->setPaymentBank("ABC");

    cout << "Original Cart Total: "
         << cart->getOriginalTotal()
         << " Rs"
         << endl;

    vector<string> applicable = mgr->getApplicable(cart);

    cout << "Applicable Coupons:" << endl;

    for (string name : applicable) {
        cout << " - " << name << endl;
    }

    double finalTotal = mgr->applyAll(cart);

    cout << "Final Cart Total after discounts: "
         << finalTotal
         << " Rs"
         << endl;

    // Cleanup code
    delete p1;
    delete p2;
    delete p3;
    delete p4;

    delete cart;
    delete mgr;

    return 0;
}