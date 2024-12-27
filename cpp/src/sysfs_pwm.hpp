#include <iostream>
#include <string>

class SysfsPwm {
    private:
        int chip;
        int channel;
        int period;
        void writeToFile(const std::string& fileName, const std::string& value);

    public:
        SysfsPwm(int chip, int channel) : chip(chip), channel(channel) {};
        int initialize(int frequency); // Export and set period
        int setDutyCycle(int dutyCycle); // Set duty-cycle and enable
        void turnOff(void); // Disable
};
