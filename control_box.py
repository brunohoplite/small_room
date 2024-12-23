import time

from presence_detector import Pir
from led_strip import LedStrip
import paho.mqtt.client as mqtt
from enum import Enum


topic1 = "MODE"
topic2 = "BRIGHTNESS"


class Mode(str, Enum):
    DETECTOR  = 'DETECTOR'
    DIMMER    = 'DIMMER'
    BREATHING = 'BREATHING'
    BLINK     = 'BLINK'

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(topic1)
    client.subscribe(topic2)

def on_message(client, userdata, msg):
    print("Topic:  " + msg.topic)
    if msg.topic == topic1:
        newCommand = str(msg.payload.decode())
        print(newCommand)
        if newCommand == 'DETECTOR' or newCommand == 'DIMMER':
            userdata.processModeCommand(newCommand)
        else:
            print("Bad topic")

    elif msg.topic == topic2:
        if userdata.mode == Mode.DIMMER:
            newBrightness = int(msg.payload.decode())
            userdata.processDimmerCommand(newBrightness)
        else:
            print("Cannot change brightness. Switch to dimmer mode.")

class ControlBox:
    def __init__(self, detectPin, brokerIp):
        self.led = LedStrip()
        self.pir = Pir(detectPin, self.led)
        self.mode = Mode.DETECTOR
        self.brightness = 0
        self.toggle = False
        self.mqttClient = mqtt.Client()
        self.mqttClient.user_data_set(self)
        self.mqttClient.on_connect = on_connect
        self.mqttClient.on_message = on_message
        try:
            self.mqttClient.connect(brokerIp)
        except:
            time.sleep(5)
            self.mqttClient.connect(brokerIp)

    def processDimmerCommand(self, newBrightness):
        if newBrightness < 0:
            newBrightness = 0
        elif newBrightness > 100:
            newBrightness = 100

        self.brightness = newBrightness

        if self.mode is Mode.DIMMER:
            self.led.set_pwm(newBrightness)

    def processModeCommand(self, newMode):
        if newMode == 'DIMMER':
            self.mode = Mode.DIMMER
            self.led.set_pwm(self.brightness)
        elif newMode == 'DETECTOR':
            self.mode = Mode.DETECTOR
            self.led.turn_off()
            self.pir.reset()

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

    def run(self):
        if self.mode == Mode.DETECTOR:
            self.pir.poll()
        elif self.mode == Mode.BLINK:
            if self.toggle:
                self.led.turn_off()
            else:
                self.led.set_pwm(self.brightness)
            self.toggle = not self.toggle

        self.mqttClient.loop(timeout=0.2)
