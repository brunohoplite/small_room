import time

from presence_detector import Pir
from led_strip import LedStrip
import paho.mqtt.client as mqtt
from enum import IntEnum


topic1 = "MODE"
topic2 = "BRIGHTNESS"


class Mode(IntEnum):
    DETECTOR = 1
    DIMMER = 2

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
        self.led.ledPwm(newBrightness)

    def processModeCommand(self, newMode):
        if newMode == 'DIMMER':
            self.mode = Mode.DIMMER
            self.led.ledPwm(self.brightness)
        elif newMode == 'DETECTOR':
            self.mode = Mode.DETECTOR
            self.led.ledOff()
            self.pir.resetState()

    def run(self):
        if self.mode == Mode.DETECTOR:
            self.pir.poll()

        self.mqttClient.loop(timeout=0.2)
