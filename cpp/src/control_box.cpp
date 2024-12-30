#include <unistd.h>
#include <cmath>
#include <unordered_map>
#include "control_box.hpp"


#define DETECT_POLLING_PERIOD_US 100000
#define DIM_POLLING_PERIOD_US    250000
#define BREATH_POLLING_PERIOD_US 10000
#define BLINK_POLLING_PERIOD_US  1000000


ControlBox::ControlBox(SysfsPwm& ledStrip, int detectPin)
: PresenceDetector(ledStrip, detectPin)
{
    brightnessDutyCycle_ = 0;
    stepIndex_ = 0;
    toggle_ = false;
    currentMode_ = Mode::DETECT;
    prepareBreath();
}

void ControlBox::doMode(void)
{
    switch (currentMode_)
    {
    case Mode::DETECT:
        poll();
        usleep(DETECT_POLLING_PERIOD_US);
        break;

    case Mode::DIM:
        usleep(DIM_POLLING_PERIOD_US);
        break;

    case Mode::BREATH:
        doBreath();
        usleep(BREATH_POLLING_PERIOD_US);
        break;

    case Mode::BLINK:
        doBlink();
        usleep(BLINK_POLLING_PERIOD_US);
        break;

    default:
        break;
    }
}

void ControlBox::setMode(const std::string& modeStr)
{
    ControlBox::Mode newMode = stringToMode(modeStr);
    currentMode_ = newMode;
    switch (newMode)
    {
    case Mode::DETECT:
        reset();
        break;

    case Mode::DIM:
        ledStrip_.setDutyCycle(brightnessDutyCycle_);
        break;

    case Mode::BREATH:
        ledStrip_.turnOff();
        toggle_ = false;
        stepIndex_ = 0;
        break;

    case Mode::BLINK:
        ledStrip_.setDutyCycle(brightnessDutyCycle_);
        toggle_ = false;
        break;

    default:
        currentMode_ = Mode::DETECT;
        break;
    }
}

void ControlBox::setBrightness(int newBrightness)
{
    if (newBrightness > 100) {
        brightnessDutyCycle_ = 100;
        return;
    }

    if (newBrightness < 0) {
        brightnessDutyCycle_ = 0;
        return;
    }

    brightnessDutyCycle_ = newBrightness;
}

void ControlBox::doBlink(void)
{
    if (toggle_) {
        ledStrip_.setDutyCycle(brightnessDutyCycle_);
    }
    else {
        ledStrip_.turnOff();
    }
    toggle_ = !toggle_;
}

void ControlBox::doBreath(void)
{
    int newDt = toggle_ ? decreasingDt_[stepIndex_] : increasingDt_[stepIndex_];
    ledStrip_.setDutyCycle(newDt);
    stepIndex_++;
    if (stepIndex_ >= STEPS)
    {
        toggle_ = !toggle_;
        stepIndex_ = 0;
    }
}

void ControlBox::prepareBreath(void)
{
    for (int i = 0; i < STEPS; i++)
    {
        float brightness = (float)MAX_DT * (1.f - exp(-4.f * (float)i / (float)STEPS));
        increasingDt_[i] = (int)brightness;
        decreasingDt_[STEPS - i - 1] = (int)brightness;
    }
}

ControlBox::Mode ControlBox::stringToMode(const std::string& modeStr)
{
    static std::unordered_map<std::string, Mode> map = {
        {"Detector", Mode::DETECT},
        {"Dimmer", Mode::DIM},
        {"Blink", Mode::BLINK},
        {"Breath", Mode::BREATH}
   };

   auto it = map.find(modeStr);
   if (it == map.end()) {
        throw std::invalid_argument("Invalid mode string: " + modeStr);
   }

   return it->second;
}