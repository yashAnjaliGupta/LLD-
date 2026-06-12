#include <iostream>

#include "core/MatchingEngine.hpp"
#include "Strategy/FIFOMatchingStrategy.hpp"
#include "managers/OrderManager.hpp"
#include "managers/MarketDataPublisher.hpp"
#include "enums/OrderType.hpp"

int main() {
    std::cout << "=== Trading Simulator FIFO Matching Test ===" << std::endl;

    MatchingEngine* engine = MatchingEngine::getInstance();
    FIFOMatchingStrategy strategy;
    engine->setMatchingStrategy(&strategy);

    MarketDataPublisher publisher;
    engine->setPublisher(&publisher);

    OrderManager orderManager(engine);

    orderManager.createOrder(OrderType::LIMIT, "AAPL", 100, true, 100.0);
    orderManager.createOrder(OrderType::LIMIT, "AAPL", 50, true, 99.0);
    orderManager.createOrder(OrderType::LIMIT, "AAPL", 120, false, 100.0);
    orderManager.createOrder(OrderType::LIMIT, "AAPL", 30, false, 99.0);
    orderManager.createOrder(OrderType::MARKET, "AAPL", 40, true);

    std::cout << "=== Test complete ===" << std::endl;
    return 0;
}
