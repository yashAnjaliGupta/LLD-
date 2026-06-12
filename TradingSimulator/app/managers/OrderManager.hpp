#pragma once

#include <iostream>
#include "../core/MatchingEngine.hpp"
#include "../factories/OrderFactory.hpp"
#include "../enums/OrderType.hpp"

using namespace std;

class OrderManager {
    MatchingEngine* matchingEngine;
public:
    OrderManager(MatchingEngine* engine) : matchingEngine(engine) {}
    void createOrder(OrderType orderType, string symbol, int quantity, bool isBuyOrder, double price = 0) {
        IOrder* order = OrderFactory::getInstance()->createOrder(orderType, symbol, quantity, isBuyOrder, price);
        processOrder(order);
    }
    void processOrder(IOrder* order) {
        matchingEngine->submitOrder(order);
    }
    void cancelOrder(int orderId) {
        // Implement order cancellation logic
    }   
};