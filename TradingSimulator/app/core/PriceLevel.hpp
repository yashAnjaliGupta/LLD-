#pragma once

#include<deque>
#include "../interface/IOrder.hpp"

class PriceLevel{
    public:
    double Price;
    deque<IOrder*> Orders;
    public:
    PriceLevel(double price) {
        this->Price = price;
    }
};