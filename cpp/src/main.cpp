#include <wiringPi.h>
#include <iostream>

#define OUTPUT_PIN 1  // (WiringPi pin 1)
#define INPUT_PIN 4   // (WiringPi pin 4)

int main() {
    // Initialize wiringPi
    if (wiringPiSetup() == -1) {
        std::cerr << "Failed to initialize wiringPi" << std::endl;
        return 1;
    }

    // Set up GPIO pins
    pinMode(OUTPUT_PIN, OUTPUT);
    pinMode(INPUT_PIN, INPUT);

    std::cout << "Blinking LED strip!!!" << std::endl;

    bool ledState = false;

    while (true) {
        // Read input pin state
        // int buttonState = digitalRead(INPUT_PIN);

        ledState = !ledState;
        digitalWrite(OUTPUT_PIN, ledState ? HIGH : LOW);
        delay(250); // Debounce delay
    }

    return 0;
}
