#include <unistd.h>
#include <cmath>
#include "control_box.hpp"


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
        usleep(100000);
        break;

    case Mode::DIM:
        usleep(250000);
        break;

    case Mode::BREATH:
        doBreath();
        usleep(10000);
        break;

    case Mode::BLINK:
        doBlink();
        usleep(1000000);
        break;

    default:
        break;
    }
}

void ControlBox::setMode(const Mode newMode)
{
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
    int newDt = toggle_ ? increasingDt_[stepIndex_] : decreasingDt_[stepIndex_];
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