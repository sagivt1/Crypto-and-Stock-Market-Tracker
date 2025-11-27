#include "market_client.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <print>


int main() {

    // --- Initialization ---
    // Create the main application window with a specific size and title.
    sf::RenderWindow window(sf::VideoMode(600, 400), "Crypto Traker");
    // Limit the framerate to 60 FPS to prevent excessive CPU usage.
    window.setFramerateLimit(60);

    // Initialize the ImGui-SFML backend. This is a crucial step to connect ImGui to SFML window.
    if(!ImGui::SFML::Init(window)){
        std::println(stderr, "Failed to initialize ImGui");
        return 1;
    }

    // --- Application State ---
    MarketClient client;
    double btc_price = 0.0;
    std::string status = "Ready";
    sf::Clock deltaClock;

    // --- Main Application Loop ---
    // This loop runs continuously as long as the window is open. Each iteration is one frame.
    while(window.isOpen()) {
        // --- Event Handling  ---
        // Process all pending events, such as mouse clicks, key presses, or window close requests.
        sf::Event event;
        while(window.pollEvent(event)) {
            // Pass the event to ImGui to handle its own interactions (e.g., clicking on a button).
            ImGui::SFML::ProcessEvent(window, event);
            // Check if the user has requested to close the window.
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        // --- GUI Update & Drawing ---
        // Tell ImGui to start a new frame. The delta time is used for animations and timing-dependent operations.
        ImGui::SFML::Update(window, deltaClock.restart());

        // Begin defining a new ImGui window. All subsequent ImGui calls will add elements to this window.
        ImGui::Begin("Dashboard"); // Start a new window in the app
        // Add text and a separator to the ImGui window.
        ImGui::Text("Bitcoin Price Tracker");
        ImGui::Separator();

        // Conditionally display the price: show the value if it's been fetched, otherwise show a placeholder.
        if (btc_price > 0.0) {
            ImGui::Text("Current Price: $%.2f", btc_price);
        } else {
            ImGui::Text("Current Price: ---");
        }

        ImGui::Spacing();

        // Create a button. The code inside the if-statement runs only when the button is clicked.
        if(ImGui::Button("Fetch Price Now")) {
            status = "Fetching...";

            // This is a blocking network call. The entire application will freeze here
            // until the get_coin_price function returns a value.
            auto result = client.get_coin_price("bitcoin");

            // After the call completes, check if we received valid data.
            if(result) {
                // If successful, update the price and status message.
                btc_price = result->current_price;
                status = "Updated successfully";
            } else {
                status = "Error: Faild to fetch price";
            }
        }

        // Display the current status text with a gray color.
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Status: %s", status.c_str());
                
        // End the definition of the "Dashboard" window.
        ImGui::End(); 

        // --- Rendering ---
        // Clear the SFML window with a default color (black).
        window.clear();
        // Tell ImGui-SFML to render all the ImGui elements that have been defined.
        ImGui::SFML::Render(window);
        // Display the contents of the window on the screen.
        window.display();
    }
    // --- Shutdown ---
    // Clean up ImGui-SFML resources before the application exits.
    ImGui::SFML::Shutdown();
    return 0;
}