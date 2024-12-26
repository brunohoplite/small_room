#include <iostream>
#include <string>

class SysfsPwm {
    private:
        std::string pwmChipPath;
    
    public:
        SysfsPwm(const std::string& chipPath) : pwmChipPath(chipPath) {};
        int initialize(unsigned int frequency); // Export and set period
        int setDutyCycle(unsigned int dutyCycle); // Set duty-cycle and enable
        void turnOff(void); // Disable
};
