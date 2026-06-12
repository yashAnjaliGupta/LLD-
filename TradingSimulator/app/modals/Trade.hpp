#include <string>
using namespace std;
#pragma once

#include <string>
using namespace std;

class Trade{
    int buyerID;
    int sellerID;
    string symbol;
    int quantity;
    double price;
    public: 
    Trade(int buyerID, int sellerID, string symbol, int quantity, double price) {
        this->buyerID = buyerID;
        this->sellerID = sellerID;
        this->symbol = symbol;
        this->quantity = quantity;
        this->price = price;
    }

    int getBuyerID() const { return buyerID; }
    int getSellerID() const { return sellerID; }
    const string& getSymbol() const { return symbol; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
};