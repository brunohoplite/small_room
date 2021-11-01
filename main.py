from control_box import ControlBox

if __name__ == '__main__':
    box = ControlBox(detectPin=23, brokerIp="192.168.2.1")
    while(True):
        box.run()
