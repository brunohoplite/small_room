import math
from time import sleep
from enum import Enum
from presence_detector import Pir
from led_strip import LedStrip


STEPS = 100
TIME_STEP = 0.010
MAX_DT = 100


class Mode(str, Enum):
    DETECTOR  = 'DETECTOR'
    DIMMER    = 'DIMMER'
    BREATHING = 'BREATH'
    BLINK     = 'BLINK'


class ControlBox:
    def __init__(self, detectPin):
        self.led = LedStrip()
        self.pir = Pir(detectPin, self.led)
        self.mode = Mode.DETECTOR
        self.brightness = 0
        self.toggle = False
        self.breathing_increasing = []
        self.breathing_decreasing = []
        self.breathing_index = 0
        self.__prepare_breathing()

    def __prepare_breathing(self):
        for i in range(STEPS):
            brightness = MAX_DT * (1 - math.exp(-4 * i / STEPS))
            self.breathing_increasing.append(brightness)

        for i in range(STEPS):
            self.breathing_decreasing.append(self.breathing_increasing[STEPS - 1 - i])

    def processDimmerCommand(self, newBrightness):
        if newBrightness < 0:
            newBrightness = 0
        elif newBrightness > 100:
            newBrightness = 100

        self.brightness = newBrightness

        if self.mode is Mode.DIMMER:
            self.led.set_pwm(newBrightness)

    def set_mode(self, new_mode: str) -> None:
        if new_mode.lower() == Mode.DETECTOR.lower():
            self.mode = Mode.DETECTOR
            self.pir.reset()
            return

        if new_mode.lower() == Mode.DIMMER.lower():
            self.mode = Mode.DIMMER
            self.led.set_pwm(self.brightness)
            return

        if new_mode.lower() == Mode.BLINK.lower():
            self.mode = Mode.BLINK
            self.led.set_pwm(self.brightness)
            return

        if new_mode.lower() == Mode.BREATHING.lower():
            self.mode = Mode.BREATHING
            self.led.set_pwm(0)
            self.toggle = False
            self.breathing_index = 0
            return

    def run(self):
        if self.mode == Mode.DETECTOR:
            self.pir.poll()
            sleep(0.1)
        elif self.mode == Mode.BLINK:
            if self.toggle:
                self.led.turn_off()
            else:
                self.led.set_pwm(self.brightness)
            self.toggle = not self.toggle
            sleep(1)
        elif self.mode == Mode.BREATHING:
            if self.toggle:
                self.led.set_pwm(self.breathing_decreasing[self.breathing_index])
            else:
                self.led.set_pwm(self.breathing_increasing[self.breathing_index])
            self.breathing_index += 1
            if self.breathing_index >= STEPS:
                self.breathing_index = 0
                self.toggle = not self.toggle
            sleep(TIME_STEP)
        else:
            sleep(0.25)
