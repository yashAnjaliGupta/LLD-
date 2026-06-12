#pragma once

#include <string>
using namespace std;


class IOrder {
    public:
    int id;
    string Symbol;
    int Quantity;
    bool isBuyOrder;
    virtual string getOrderType() = 0;
};
