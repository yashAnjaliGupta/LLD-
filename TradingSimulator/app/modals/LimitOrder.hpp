#pragma once

#include "../interface/IOrder.hpp"

class LimitOrder: public IOrder {
    public:
    double Price;
    LimitOrder(int id, string symbol, int quantity, bool isBuyOrder, double price) {
        this->id = id;
        this->Symbol = symbol;
        this->Quantity = quantity;
        this->isBuyOrder = isBuyOrder;
        this->Price = price;
    }
    string getOrderType() override {
        return "Limit";
    }

};