#include "market_client.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <print>
#include <format>

using json = nlohmann::json;

std::optional<CoinData> MarketClient::parse_coin_price(const std::string& json_body, const std::string& coin_id) {
    try {
        auto parsed = json::parse(json_body);
        
        // Check if coin exists
        if(parsed.contains(coin_id) && parsed[coin_id].contains("usd")) {
            double price = parsed[coin_id]["usd"];
            return CoinData{coin_id, price};
        } 
    } catch (const std::exception& e) {
        std::println(stderr, "JSON Parsing Error: {}", e.what());
    }
    return std::nullopt;
}

std::optional<CoinData> MarketClient::get_coin_price(const std::string& coin_id) {
    // Construct API URL
    std::string url = std::format("https://api.coingecko.com/api/v3/simple/price?ids={}&vs_currencies=usd", coin_id);

    // Blocking network call
    cpr::Response r = cpr::Get(cpr::Url{url}, cpr::VerifySsl(false));

    if(r.status_code == 200) {
        return parse_coin_price(r.text, coin_id);
    } else {
        std::println(stderr, "Network Error: {} - {}", r.status_code, r.error.message);
        return std::nullopt;
    }
}