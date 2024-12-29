#include <wiringPi.h>
#include <iostream>
#include "sysfs_pwm.hpp"
#include "control_box.hpp"

#define OUTPUT_PIN 1  // Physical pin 12 (WiringPi pin 1)
#define INPUT_PIN 4   // Physical pin 16 (WiringPi pin 4)
#define PWM_CHIP 0
#define PWM_CHANNEL 0

#define SECONDS_TO_USECOND (1000000)


int main() {
    try {
        // Initialize wiringPi
        if (wiringPiSetup() == -1) {
            std::cerr << "Failed to initialize wiringPi" << std::endl;
            return 1;
        }

        // Set up GPIO pin
        pinMode(INPUT_PIN, INPUT);

        std::cout << "Starting small room LED strip controller!" << std::endl;

        SysfsPwm sysfsPwm(PWM_CHIP, PWM_CHANNEL);
        sysfsPwm.initialize(1000);
        ControlBox controlBox(sysfsPwm, INPUT_PIN);
        #if DEBUG // TODO: remove eventually
        const ControlBox::Mode modes[] = {ControlBox::Mode::BLINK, ControlBox::Mode::BREATH, ControlBox::Mode::DIM, ControlBox::Mode::DETECT};
        int index = 0;
        Timer myTimer(std::chrono::milliseconds(5000));
        controlBox.setBrightness(75);
        #endif

        while (true) {
            controlBox.doMode();
            #if DEBUG
            if (myTimer.hasElapsed()) {
                myTimer.reset();
                ControlBox::Mode newMode = modes[index];
                index++;
                if (index >= 4) index = 0;
                controlBox.setMode(newMode);
            }
            #endif
        }
    }
    catch (const std::exception& ex) {
        fprintf(stderr, "Error: %s\n", ex.what());
        return 1;
    }

    return 0;
}
