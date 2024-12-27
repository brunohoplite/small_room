#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
#include "sysfs_pwm.hpp"

#define OUTPUT_PIN 1  // Physical pin 12 (WiringPi pin 1)
#define INPUT_PIN 4   // Physical pin 16 (WiringPi pin 4)
#define PWM_CHIP 0
#define PWM_CHANNEL 0

// Read input pin state
// int buttonState = digitalRead(INPUT_PIN);

int main() {
    try {
        // Initialize wiringPi
        if (wiringPiSetup() == -1) {
            std::cerr << "Failed to initialize wiringPi" << std::endl;
            return 1;
        }

        // Set up GPIO pins
        //pinMode(OUTPUT_PIN, OUTPUT);
        pinMode(INPUT_PIN, INPUT);

        std::cout << "Blinking LED strip!!!" << std::endl;

        SysfsPwm sysfsPwm(PWM_CHIP, PWM_CHANNEL);
        sysfsPwm.initialize(1000);
        bool ledState = false;

        while (true) {
            ledState = !ledState;
            int dutyCycle = ledState ? 20 : 0;
            sysfsPwm.setDutyCycle(dutyCycle);
            sleep(1); // Debounce delay
        }
    }
    catch (const std::exception& ex) {
        fprintf(stderr, "Error: %s\n", ex.what());
        return 1;
    }

    return 0;
}
