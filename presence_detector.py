from time import monotonic
from  enum import Enum
import RPi.GPIO as gpio
from led_strip import LedStrip


class State(Enum):
    IDLE = 0
    DETECTED = 1
    WAIT = 2


class Pir:
    DETECTED_PERIOD_S = 60
    WAIT_PERIOD_S = 5
    def __init__(self, pin: int, led_strip: LedStrip):
        self.detect_pin = pin  # BCM pin numbering
        self.start_time = 0
        self.state = State.IDLE
        self.led_strip = led_strip
        self.__init_gpio()

    def reset(self):
        self.state = State.IDLE
        self.led_strip.turn_off()

    def poll(self):
        if self.state == State.IDLE:
            if gpio.input(self.detect_pin) == gpio.HIGH:
                self.led_strip.turn_on()
                self.start_time = monotonic()
                self.state = State.DETECTED
            return
        if self.state == State.DETECTED:
            if gpio.input(self.detect_pin) == gpio.HIGH:
                self.start_time = monotonic()
                return
            if (monotonic() - self.start_time) >= self.DETECTED_PERIOD_S:
                self.led_strip.turn_off()
                self.start_time = monotonic()
                self.state = State.WAIT
            return
        if self.state == State.WAIT:
            if (monotonic() - self.start_time) >= self.WAIT_PERIOD_S:
                self.start_time = monotonic()
                self.state = State.IDLE

    def __init_gpio(self):
        gpio.setmode(gpio.BCM)
        gpio.setup(self.detect_pin, gpio.IN, pull_up_down=gpio.PUD_DOWN)
