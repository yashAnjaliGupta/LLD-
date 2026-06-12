#pragma once

#include <algorithm>
#include <vector>
#include "IMatchingStrategy.hpp"
#include "../modals/Trade.hpp"
#include "../modals/LimitOrder.hpp"
#include "../core/OrderBook.hpp"

class FIFOMatchingStrategy: public IMatchingStrategy {
public:
    std::vector<Trade*> matchOrders(IOrder* order, OrderBook* orderBook) override {
        std::vector<Trade*> trades;
        if(order == nullptr || orderBook == nullptr) {
            return trades;
        }

        bool isBuy = order->isBuyOrder;
        bool isLimitOrder = order->getOrderType() == "Limit";
        double orderPrice = 0.0;
        if(isLimitOrder) {
            LimitOrder* limitOrder = dynamic_cast<LimitOrder*>(order);
            if(limitOrder != nullptr) {
                orderPrice = limitOrder->Price;
            }
        }

        auto& oppositeBook = isBuy ? orderBook->getSellBook() : orderBook->getBuyBook();
        std::vector<double> priceLevels;
        for(auto& entry : oppositeBook) {
            priceLevels.push_back(entry.first);
        }
        if(!isBuy) {
            std::reverse(priceLevels.begin(), priceLevels.end());
        }

        for(double priceLevel : priceLevels) {
            if(order->Quantity <= 0) {
                break;
            }

            if(isLimitOrder) {
                if(isBuy && orderPrice < priceLevel) {
                    break;
                }
                if(!isBuy && orderPrice > priceLevel) {
                    break;
                }
            }

            auto levelIt = oppositeBook.find(priceLevel);
            if(levelIt == oppositeBook.end()) {
                continue;
            }
            PriceLevel* level = levelIt->second;
            if(level == nullptr) {
                continue;
            }

            std::vector<IOrder*> ordersAtLevel(level->Orders.begin(), level->Orders.end());
            for(IOrder* oppositeOrder : ordersAtLevel) {
                if(order->Quantity <= 0) {
                    break;
                }
                if(oppositeOrder == nullptr || oppositeOrder->Quantity <= 0) {
                    orderBook->removeOrder(oppositeOrder);
                    continue;
                }

                int tradeQty = std::min(order->Quantity, oppositeOrder->Quantity);
                double tradePrice = orderPrice;
                if(oppositeOrder->getOrderType() == "Limit") {
                    LimitOrder* oppositeLimit = dynamic_cast<LimitOrder*>(oppositeOrder);
                    if(oppositeLimit != nullptr) {
                        tradePrice = oppositeLimit->Price;
                    }
                }

                int buyerID = isBuy ? order->id : oppositeOrder->id;
                int sellerID = isBuy ? oppositeOrder->id : order->id;
                Trade* trade = new Trade(buyerID, sellerID, order->Symbol, tradeQty, tradePrice);
                trades.push_back(trade);

                order->Quantity -= tradeQty;
                oppositeOrder->Quantity -= tradeQty;
                if(oppositeOrder->Quantity <= 0) {
                    orderBook->removeOrder(oppositeOrder);
                }
            }
        }

        if(order->Quantity > 0 && isLimitOrder) {
            orderBook->addOrder(order);
        }

        return trades;
    }
};