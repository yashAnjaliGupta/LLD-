#pragma once

#include "../interface/IOrder.hpp"

class MarketOrder: public IOrder {
    public:
    MarketOrder(int id, string symbol, int quantity, bool isBuyOrder) {
        this->id = id;
        this->Symbol = symbol;
        this->Quantity = quantity;
        this->isBuyOrder = isBuyOrder;
    }
    string getOrderType() override {
        return "Market";
    }
};