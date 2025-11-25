#include "market_client.hpp"
#include <print>


int main() {
    
    MarketClient client;

    std::println("--- Crypto Tracker CLI ---");
    std::println("Fetching Bitcoin price...");

    auto result = client.get_coin_price("bitcoin");

    if(result) {
        std::println("Success! {} price is ${}", result->id, result->current_price);
    } else {
        std::println(stderr, "Failed to fetch crypto data.");
    }

    return 0;
}