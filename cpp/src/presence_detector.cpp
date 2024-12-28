#include <wiringPi.h>
#include "presence_detector.hpp"

#define DETECTED_PERIOD_MS std::chrono::milliseconds(60000)
#define WAIT_PERIOD_MS std::chrono::milliseconds(5000)

PresenceDetector::PresenceDetector(int detectPin)
{
    detectPin_ = detectPin;
    currentState = IDLE;
}

void PresenceDetector::poll(void)
{
    switch (currentState)
    {
    case IDLE:
        if (isDetectPinAsserted()) {
            // TODO: Turn on LED
            timer_.setDelay(DETECTED_PERIOD_MS);
            currentState = DETECTED;
        }
        break;
    
    case DETECTED:
        if (isDetectPinAsserted()) {
            timer_.reset();
            return;
        }

        if (timer_.hasElapsed()) {
            // TODO: Turn off LED
            timer_.setDelay(WAIT_PERIOD_MS);
            currentState = WAIT;
        }
        break;
    
    case WAIT:
        if (timer_.hasElapsed()) {
            currentState = IDLE;
        }
        break;
    
    default:
        break;
    }
}

void PresenceDetector::reset(void)
{
    // TODO: turn off LED
    currentState = IDLE;
}

bool PresenceDetector::isDetectPinAsserted(void)
{
    int value = digitalRead(detectPin_);

    return value == HIGH;
}