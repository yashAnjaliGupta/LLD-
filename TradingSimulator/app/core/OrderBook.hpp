#pragma once

#include <string>
#include <map>
#include <algorithm>
#include "PriceLevel.hpp"
#include "../modals/LimitOrder.hpp"
using namespace std;

class OrderBook{
    string symbol;
    map<double, PriceLevel*> buyBook;
    map<double, PriceLevel*> sellBook;
    public:
    map<double, PriceLevel*>& getBuyBook() { return buyBook; }
    map<double, PriceLevel*>& getSellBook() { return sellBook; }

    OrderBook(string symbol) {
        this->symbol = symbol;
    }
    string getSymbol() const {
        return symbol;
    }
    double getBestBid() {
        if(buyBook.empty()) return 0;
        return buyBook.rbegin()->first;
    }
    double getBestAsk() {
        if(sellBook.empty()) return 0;
        return sellBook.begin()->first;
    }
    double getSpread() {
        double bestBid = getBestBid();
        double bestAsk = getBestAsk();
        if(bestBid == 0 || bestAsk == 0) return 0;
        return bestAsk - bestBid;
    }
    void addOrder(IOrder* order) {
        if(order == nullptr || order->getOrderType() != "Limit") {
            return;
        }
        LimitOrder* limitOrder = dynamic_cast<LimitOrder*>(order);
        if(limitOrder == nullptr) {
            return;
        }

        double price = limitOrder->Price;
        if(order->isBuyOrder) {
            if(buyBook.find(price) == buyBook.end()) {
                buyBook[price] = new PriceLevel(price);
            }
            buyBook[price]->Orders.push_back(order);
        } else {
            if(sellBook.find(price) == sellBook.end()) {
                sellBook[price] = new PriceLevel(price);
            }
            sellBook[price]->Orders.push_back(order);
        }
    }
    void removeOrder(IOrder* order) {
        if(order == nullptr || order->getOrderType() != "Limit") {
            return;
        }
        LimitOrder* limitOrder = dynamic_cast<LimitOrder*>(order);
        if(limitOrder == nullptr) {
            return;
        }

        double price = limitOrder->Price;
        if(order->isBuyOrder) {
            if(buyBook.find(price) != buyBook.end()) {
                auto& orders = buyBook[price]->Orders;
                orders.erase(remove(orders.begin(), orders.end(), order), orders.end());
                if(orders.empty()) {
                    buyBook.erase(price);
                }
            }
        } else {
            if(sellBook.find(price) != sellBook.end()) {
                auto& orders = sellBook[price]->Orders;
                orders.erase(remove(orders.begin(), orders.end(), order), orders.end());
                if(orders.empty()) {
                    sellBook.erase(price);
                }
            }
        }
    }

};