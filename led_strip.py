from rpi_hardware_pwm import HardwarePWM

class LedStrip:
    def __init__(self):
        self.pwm = HardwarePWM(0, hz=1000)  # 0 for GPIO18

    def turn_on(self):
        self.pwm.start(100)

    def turn_off(self):
        self.pwm.stop()

    def set_pwm(self, duty_cycle: int):
        self.pwm.start(duty_cycle)
