#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include "sysfs_pwm.hpp"
#include "presence_detector.hpp"

#define OUTPUT_PIN 1  // Physical pin 12 (WiringPi pin 1)
#define INPUT_PIN 4   // Physical pin 16 (WiringPi pin 4)
#define PWM_CHIP 0
#define PWM_CHANNEL 0

#define SECONDS_TO_USECOND (1000000)

#define STEPS 100
#define TIME_STEP (0.010)
#define MAX_DT (100)

int increasingDt[STEPS];
int decreasingDt[STEPS];

void breathingAnimation(void)
{
    for (int i = 0; i < STEPS; i++)
    {
        float brightness = (float)MAX_DT * (1.f - exp(-4.f * (float)i / (float)STEPS));
        increasingDt[i] = (int)brightness;
        decreasingDt[STEPS - i - 1] = (int)brightness;
    }
}

void doBreathing(void)
{
    // static int stepIndex = 0;
    // static bool increasing = true;
    // int newDt = increasing ? increasingDt[stepIndex] : decreasingDt[stepIndex];
    // sysfsPwm.setDutyCycle(newDt);
    // stepIndex++;
    // if (stepIndex >= STEPS)
    // {
    //     increasing = !increasing;
    //     stepIndex = 0;
    // }
}

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
        PresenceDetector presenceDetector(sysfsPwm, INPUT_PIN);

        while (true) {
            presenceDetector.poll();
            usleep(0.1f * SECONDS_TO_USECOND);
        }
    }
    catch (const std::exception& ex) {
        fprintf(stderr, "Error: %s\n", ex.what());
        return 1;
    }

    return 0;
}
