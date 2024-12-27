#include <fcntl.h>   // For open()
#include <unistd.h>  // For close()
#include <cstring>
#include "sysfs_pwm.hpp"

#define SYSFS_PERIOD_S_TO_NS (1000000000U)
#define SYSFS_PWM_PATH "/sys/class/pwm/pwmchip"

void SysfsPwm::writeToFile(const std::string& fileName, const std::string& value)
{
    const std::string filePath = std::string(SYSFS_PWM_PATH) + std::to_string(chip) + fileName;
    int fd = open(filePath.c_str(), O_WRONLY);

    if (fd < 0) {
        throw std::runtime_error("Failed to open " + filePath + ": " + strerror(errno));
    }

    if (write(fd, value.c_str(), value.size()) < 0) {
        close(fd);
        throw std::runtime_error("Failed to write to  " + filePath + ": " + strerror(errno));
    }

    close(fd);
}

SysfsPwm::~SysfsPwm()
{
    unexport();
}

int SysfsPwm::initialize(int frequency)
{
    if (frequency <= 0) {
        throw std::runtime_error("Frequency should be greater than 0!");
    }

    // Export the PWM chip
    writeToFile("/export", std::to_string(channel));

    // Give the kernel a few milliseconds to create the pwm channel
    sleep(1);

    // Write the PWM period
    unsigned int periodNs = SYSFS_PERIOD_S_TO_NS / frequency;
    std::cout << "PWM with period " << periodNs << " ns" << std::endl;
    writeToFile("/pwm" + std::to_string(channel) + "/period", std::to_string(periodNs));
    period = periodNs;

    return 0;
}

void SysfsPwm::unexport(void)
{
    writeToFile("/unexport", std::to_string(channel));
}

int SysfsPwm::setDutyCycle(int dutyCycle)
{
    if ((dutyCycle < 0) || (dutyCycle > 100)) {
        throw std::runtime_error("Duty cycle should be between 0 and 100");
    }

    int onPeriod = (dutyCycle * period) / 100;

    // Set the duty cycle
    writeToFile("/pwm" + std::to_string(channel) + "/duty_cycle", std::to_string(onPeriod));

    // Enable the pwm
    writeToFile("/pwm" + std::to_string(channel) + "/enable", std::string("1"));

    std::cout << "PWM on period " << onPeriod << " ns" << std::endl;

    return 0;
}