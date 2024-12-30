#include <wiringPi.h>
#include <iostream>
#include <fstream>
#include "crow.h"
#include "sysfs_pwm.hpp"
#include "control_box.hpp"

#define OUTPUT_PIN 1  // Physical pin 12 (WiringPi pin 1)
#define INPUT_PIN 4   // Physical pin 16 (WiringPi pin 4)
#define PWM_CHIP 0
#define PWM_CHANNEL 0
#define PWM_FREQUENCY 1000

#define SECONDS_TO_USECOND (1000000)


crow::SimpleApp app;
SysfsPwm sysfsPwm(PWM_CHIP, PWM_CHANNEL, PWM_FREQUENCY);
ControlBox controlBox(sysfsPwm, INPUT_PIN);

void controlBoxThread(void) {
    while (true) {
        controlBox.doMode();
    }
}

// Function to load HTML file content
std::string loadHTML(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        return "<h1>Error: HTML file not found</h1>";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void signalHandler(int signal) {
    if (signal == SIGINT) {
        app.stop(); // Stop Crow server
    }
}

int main() {
    try {
        // Initialize wiringPi
        if (wiringPiSetup() == -1) {
            std::cerr << "Failed to initialize wiringPi" << std::endl;
            return 1;
        }

        // Register signal handler
        signal(SIGINT, signalHandler);

        // Set up GPIO pin
        pinMode(INPUT_PIN, INPUT);

        std::cout << "Starting small room LED strip controller!" << std::endl;

        // Start a thread for LED control
        std::thread controlBoxThreadRunner(controlBoxThread);

        // Serve the HTML page from file
        CROW_ROUTE(app, "/")([]() {
            return crow::response(loadHTML("../src/templates/index.html"));
        });

        // Handle the form submission
        CROW_ROUTE(app, "/update").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
            auto body = crow::json::load(req.body);
            if (!body) {
                return crow::response(400);
            }

            int newBrightness = body["brightness"].i();
            std::string newMode = body["mode"].s();
            std::cout << "New brightness: " << newBrightness << ", new mode: " << newMode << std::endl;
            controlBox.setBrightness(newBrightness);
            controlBox.setMode(newMode);

            return crow::response(200);
        });

        // Start the webserver
        app.port(5000).multithreaded().run();

        // Clean up on exit
        controlBoxThreadRunner.join();
    }
    catch (const std::exception& ex) {
        fprintf(stderr, "Error: %s\n", ex.what());
        return 1;
    }

    return 0;
}
