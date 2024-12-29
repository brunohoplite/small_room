# Small room light controller

This is a service meant to run on a raspberry pi that controls an LED light strip by PWM in response to presence detection. This is useful because I don't want to have to turn on and off the light every time I enter the room.

The device is made out of an off the shelf box containing:
- A raspberry pi 0 W
- A PIR sensor
- A perf board with an AC to DC converter supplying 12 V to the LED strip and a BJT transistor to switch the LED strip current
- A 12V to 5V DC/DC converter with a USB output to supply power to the raspberry pi

The service also hosts a small web server to allow the user to choose the operational mode of the control box:
- Detector (default): turn on LED strip on presence detection and turn it off 1 min following no detection
- Dimmer: choose the LED strip brightness and light becomes solid
- Blink: blink the LED strip with the desired brightness
- Breath: breathing animation with the desired brightness
