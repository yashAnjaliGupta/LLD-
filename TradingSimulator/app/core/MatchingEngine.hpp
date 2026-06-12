#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "OrderBook.hpp"
#include "../interface/IOrder.hpp"
#include "../modals/Trade.hpp"
#include "../Strategy/IMatchingStrategy.hpp"
#include "../managers/MarketDataPublisher.hpp"
using namespace std;

class MatchingEngine {
    static MatchingEngine* instance;
    MatchingEngine() : matchingStrategy(nullptr), publisher(nullptr) {}
    std::unordered_map<std::string, OrderBook*> orderBooks;
    IMatchingStrategy* matchingStrategy;
    std::vector<Trade*> tradeHistory;
    MarketDataPublisher* publisher;

public:
    static MatchingEngine* getInstance(){
        if(instance == nullptr) {
            instance = new MatchingEngine();
        }
        return instance;
    }
    void setMatchingStrategy(IMatchingStrategy* strategy) {
        this->matchingStrategy = strategy;
    }
    void setPublisher(MarketDataPublisher* publisher) {
        this->publisher = publisher;
    }
    void submitOrder(IOrder* order) {
        if(orderBooks.find(order->Symbol) == orderBooks.end()) {
            orderBooks[order->Symbol] = new OrderBook(order->Symbol);
        }
        std::vector<Trade*> trades = matchingStrategy->matchOrders(order, orderBooks[order->Symbol]);
        for(Trade* trade : trades) {
            tradeHistory.push_back(trade);
            if(publisher) {
                publisher->publishTrade(trade);
            }
        }
        publisher->publishOrderBook(orderBooks[order->Symbol]);
    }
};

MatchingEngine* MatchingEngine::instance = nullptr;