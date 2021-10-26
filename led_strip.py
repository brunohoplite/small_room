from rpi_hardware_pwm import HardwarePWM

class LedStrip:
    def __init__(self):
        self.pwm = HardwarePWM(0, hz=10000)  # 0 for GPIO18

    def ledOn(self):
        self.pwm.start(100)

    def ledOff(self):
        self.pwm.stop()

    def ledPwm(self, dt):
        self.pwm.start(dt)
