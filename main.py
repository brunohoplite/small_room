#!/usr/bin/env python3
from control_box import ControlBox

if __name__ == '__main__':
    box = ControlBox(detectPin=23, brokerIp="127.0.0.1")
    while(True):
        box.run()
