#pragma once

#include <iostream>

#include "../modals/Trade.hpp"
#include "../core/OrderBook.hpp"

using namespace std;

class MarketDataPublisher {
public:
    void publishTrade(Trade* trade) {
        // Implement logic to publish trade data to subscribers
        cout << "Trade executed: " << trade->getQuantity() << " shares of " << trade->getSymbol() << " at $" << trade->getPrice() << endl;
        cout<< "-----------------------------\n";
    }
    void publishOrderBook(OrderBook* orderBook) {
        // Implement logic to publish order book updates to subscribers
        cout<< "Order book for " << orderBook->getSymbol() <<"\n";
        cout<< "Buy Book:\n";
        for(auto& entry : orderBook->getBuyBook()) {
            cout << "Price: $" << entry.first << ", Quantity: ";
            int totalQuantity = 0;
            for(IOrder* order : entry.second->Orders) {
                totalQuantity += order->Quantity;
            }      
            cout << totalQuantity << endl;
        }
        cout<< "Sell Book:\n";
        for(auto& entry : orderBook->getSellBook()) {
            cout << "Price: $" << entry.first << ", Quantity: ";
            int totalQuantity = 0;
            for(IOrder* order : entry.second->Orders) {
                totalQuantity += order->Quantity;
            }
            cout << totalQuantity << endl;
        }
        cout<< "-----------------------------\n";


    }
};