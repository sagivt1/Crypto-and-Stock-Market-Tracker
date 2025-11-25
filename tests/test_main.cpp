#include <gtest/gtest.h>
#include "market_client.hpp"
#include "logic.hpp"

TEST(SetupTest, VersionCheck) {
    EXPECT_EQ(MarketConfig::get_app_version(), "MarketTracker v1.0");
}

// Test valid JSON Parsing
TEST(MarketClientTest, ParsesValidJsonCorrectly) {
    std::string test_json = R"({"bitcoin": {"usd": 50000.0}})";
    auto result = MarketClient::parse_coin_price(test_json, "bitcoin");
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->id, "bitcoin");
    EXPECT_EQ(result->current_price, 50000.0);
}

// Test Garbage Json
TEST(MarketClientTest, HandlesInvalidJson) {
    std::string broken_json = "{ broken : ";
    auto result = MarketClient::parse_coin_price(broken_json, "bitcoin");
    EXPECT_FALSE(result.has_value());
}

// Test Wrong coin Data
TEST(MarketClientTest, HandlesMissingData) {
    std::string wrong_json = R"({"ethereum": {"usd": 3000.0}})";
    auto result = MarketClient::parse_coin_price(wrong_json, "bitcoin");
    EXPECT_FALSE(result.has_value());
}