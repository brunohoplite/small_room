from led_strip import LedStrip
import RPi.GPIO as gpio
import time
from enum import IntEnum

detectedPeriod = 60
waitPeriod = 10
detectedPwm = 100


class PirState(IntEnum):
    IDLE = 0
    DETECTED = 1
    WAIT = 2


class Pir:
    def __init__(self, pin, ledStrip):
        self.pin = pin  # BCM pin numbering
        gpio.setmode(gpio.BCM)
        gpio.setup(self.pin, gpio.IN, pull_up_down=gpio.PUD_DOWN)
        self.startTime = 0
        self.pirState = PirState.IDLE
        self.led = ledStrip

    def resetState(self):
        self.pirState = PirState.IDLE

    def poll(self):
        if self.pirState == PirState.IDLE:
            if gpio.input(self.pin) == gpio.HIGH:
                self.led.ledPwm(detectedPwm)
                self.startTime = time.monotonic()
                self.pirState = PirState.DETECTED
        elif self.pirState == PirState.DETECTED:
            if (time.monotonic() - self.startTime) >= detectedPeriod:
                self.led.ledOff()
                self.startTime = time.monotonic()
                self.pirState = PirState.WAIT
        elif self.pirState == PirState.WAIT:
            if (time.monotonic() - self.startTime) >= waitPeriod:
                self.startTime = time.monotonic()
                self.pirState = PirState.IDLE
