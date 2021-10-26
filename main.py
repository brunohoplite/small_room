from presence_detector import Pir

if __name__ == '__main__':
    pir = Pir(18)
    while(True):
        pir.poll()
