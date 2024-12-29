#include <wiringPi.h>
#include "presence_detector.hpp"

#define DETECTED_PERIOD_MS std::chrono::milliseconds(60000)
#define WAIT_PERIOD_MS std::chrono::milliseconds(5000)

void PresenceDetector::poll(void)
{
    switch (currentState)
    {
    case IDLE:
        if (isDetectPinAsserted()) {
            ledStrip_.setDutyCycle(100);
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
            ledStrip_.turnOff();
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
    ledStrip_.turnOff();
    currentState = IDLE;
}

bool PresenceDetector::isDetectPinAsserted(void)
{
    int value = digitalRead(detectPin_);

    return value == HIGH;
}