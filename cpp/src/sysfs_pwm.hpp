#include <iostream>
#include <string>

class SysfsPwm {
    private:
        int chip;
        int channel;
        int period;
        void writeToFile(const std::string& filePath, const std::string& value);
        void unexport(void);
        std::string getPwmChannelDir(void);
        std::string getPwmChipDir(void);

    public:
        SysfsPwm(int chip, int channel) : chip(chip), channel(channel) {};
        void initialize(int frequency); // Export and set period
        void setDutyCycle(int dutyCycle); // Set duty-cycle and enable
        void turnOff(void); // Disable
        ~SysfsPwm();
};
