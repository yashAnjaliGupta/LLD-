#pragma once

#include <vector>
#include "../core/OrderBook.hpp"
#include "../interface/IOrder.hpp"
#include "../modals/Trade.hpp"

class IMatchingStrategy {
public:
    virtual std::vector<Trade*> matchOrders(IOrder* order, OrderBook* orderBook) = 0;
    virtual ~IMatchingStrategy() = default;
};