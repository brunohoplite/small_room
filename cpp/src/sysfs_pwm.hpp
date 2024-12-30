#include <iostream>
#include <string>

#pragma once

class SysfsPwm {
    private:
        int chip;
        int channel;
        int period;
        void writeToFile(const std::string& filePath, const std::string& value);
        void initialize(int frequency); // Export and set period
        void unexport(void);
        std::string getPwmChannelDir(void);
        std::string getPwmChipDir(void);

    public:
        SysfsPwm(int chip, int channel, int frequency) : chip(chip), channel(channel) { initialize(frequency); };
        void setDutyCycle(int dutyCycle); // Set duty-cycle and enable
        void turnOff(void); // Disable
        ~SysfsPwm();
};
