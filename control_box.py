from presence_detector import Pir
from led_strip import LedStrip
import paho.mqtt.client as mqtt
from enum import IntEnum


topic1 = "MODE"
topic2 = "BRIGHTNESS"


class Mode(IntEnum):
    DETECTOR = 1
    DIMMER = 2


class ControlBox:
    def __init__(self, detectPin, brokerIp):
        self.led = LedStrip()
        self.pir = Pir(detectPin, self.led)
        self.mode = Mode.DETECTOR
        self.brightness = 20
        self.mqttClient = mqtt.client()
        self.mqttClient.connect(brokerIp, userData=self)
        self.mqttClient.on_connect = self.on_connect
        self.mqttClient.on_message = self.on_message

    def on_connect(client, userdata, flags, rc):
        print("Connected with result code "+str(rc))
        client.subscribe(topic1)
        client.subscribe(topic2)

    def processDimmerCommand(self, newBrightness):
        if newBrightness < 0:
            newBrightness = 0
        elif newBrightness > 100:
            newBrightness = 100

        self.brightness = newBrightness
        self.led.ledPwm(newBrightness)

    def processModeMsg(self, newMode):
        self.mode = newMode
        if self.mode == Mode.DIMMER:
            self.led.ledPwm(self.brightness)

    def on_message(client, userdata, msg):
        print(msg.topic + " " + str(msg.payload))
        if msg.topic == topic1:
            newCommand = msg.payload
            if newCommand == Mode.DETECTOR.name or newCommand == Mode.BRIGHTNESS.name:
                userdata.processModeCommand(newCommand)
            else:
                print("Bad topic")

        elif msg.topic == topic2:
            if userdata.mode == Mode.DIMMER:
                newBrightness = msg.payload
                userdata.processDimmerCommand(newBrightness)
            else:
                print("Cannot change brightness. Switch to dimmer mode.")

    def run(self):
        if self.mode == Mode.DETECTOR:
            self.pir.poll()

        self.mqttClient.loop(timeout=0.2)
