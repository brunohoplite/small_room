#include <fcntl.h>   // For open()
#include <unistd.h>  // For close()
#include <cstring>
#include "sysfs_pwm.hpp"

#define SYSFS_PERIOD_S_TO_NS (1000000000U)
#define SYSFS_PWM_PATH "/sys/class/pwm/pwmchip"

SysfsPwm::~SysfsPwm()
{
    const std::string pwmChannelDir = getPwmChannelDir();
    // Verify if pwm channel exists unexporting
    if (access(pwmChannelDir.c_str(), F_OK) != 0) {
        return;
    }

    unexport();
}

void SysfsPwm::initialize(int frequency)
{
    if (frequency <= 0) {
        throw std::runtime_error("Frequency should be greater than 0!");
    }

    const std::string pwmChannelDir = getPwmChannelDir();

    // Verify if pwm channel exists before exporting
    if (access(pwmChannelDir.c_str(), F_OK) != 0) {
        // Export the PWM chip
        writeToFile(getPwmChipDir() + "/export", std::to_string(channel));

        // Give the kernel a few milliseconds to create the pwm channel
        // TODO: understand why this needs hacky wait!
        sleep(100000);
    }

    // Write the PWM period
    unsigned int periodNs = SYSFS_PERIOD_S_TO_NS / frequency;
    std::cout << "Initializing PWM with period " << periodNs << " ns" << std::endl;
    writeToFile(pwmChannelDir + "/period", std::to_string(periodNs));
    period = periodNs;
}

void SysfsPwm::unexport(void)
{
    writeToFile(getPwmChipDir() + "/unexport", std::to_string(channel));
}

void SysfsPwm::setDutyCycle(int dutyCycle)
{
    const std::string pwmChannelDir = getPwmChannelDir();
    // Verify if pwm channel exists before writing
    if (access(pwmChannelDir.c_str(), F_OK) != 0) {
        throw std::runtime_error("PWM channel is not exported");
    }

    if ((dutyCycle < 0) || (dutyCycle > 100)) {
        throw std::runtime_error("Duty cycle should be between 0 and 100");
    }

    int onPeriod = (dutyCycle * period) / 100;

    // Set the duty cycle
    writeToFile(pwmChannelDir + "/duty_cycle", std::to_string(onPeriod));

    // Enable the pwm
    writeToFile(pwmChannelDir + "/enable", std::string("1"));
}

void SysfsPwm::turnOff(void)
{
    const std::string pwmChannelDir = getPwmChannelDir();
    // Verify if pwm channel exists before writing
    if (access(pwmChannelDir.c_str(), F_OK) != 0) {
        throw std::runtime_error("PWM channel is not exported");
    }

    // Disable the pwm
    writeToFile(pwmChannelDir + "/enable", std::string("0"));
}

// Private
std::string SysfsPwm::getPwmChannelDir(void)
{
    return SYSFS_PWM_PATH + std::to_string(chip) + "/pwm" + std::to_string(channel);
}

std::string SysfsPwm::getPwmChipDir(void)
{
    return SYSFS_PWM_PATH + std::to_string(chip);
}

void SysfsPwm::writeToFile(const std::string& filePath, const std::string& value)
{
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