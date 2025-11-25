#pragma once
#include <string>
#include <optional>

struct CoinData
{
    std::string id;
    double current_price;
};


// A simple class to verify our build system and testing logic
class MarketClient {
public:
    // Parse string -> Object
    static std::optional<CoinData> parse_coin_price(const std::string& json_body, const std::string& coin_id);

    std::optional<CoinData> get_coin_price(const std::string& coin_id);
};
