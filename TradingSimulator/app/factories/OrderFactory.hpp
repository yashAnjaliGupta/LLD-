#pragma once

#include "../enums/OrderType.hpp"
#include "../interface/IOrder.hpp"
#include "../modals/MarketOrder.hpp"
#include "../modals/LimitOrder.hpp"


class OrderFactory {
    private:
        static int orderIdCounter;
        static OrderFactory *instance;
        OrderFactory() {}
    public:
        static OrderFactory* getInstance() {
            if(instance == nullptr) {
                instance = new OrderFactory();
            }
            return instance;
        }
        IOrder* createOrder(OrderType orderType, string symbol, int quantity, bool isBuyOrder, double price = 0) {
            orderIdCounter++;
            if(orderType == OrderType::MARKET) {
                return new MarketOrder(orderIdCounter, symbol, quantity, isBuyOrder);
            } else if(orderType == OrderType::LIMIT) {
                return new LimitOrder(orderIdCounter, symbol, quantity, isBuyOrder, price);
            }
            return nullptr;
        }

};

int OrderFactory::orderIdCounter = 0;
OrderFactory* OrderFactory::instance = nullptr;

