#from gpiozero import LED
from time import sleep
from signal import pause
import RPi.GPIO as GPIO
import signal

continue_reading = True
gpioMode=GPIO.getmode()
GPIO.setmode(GPIO.BCM)
pin_led=26
pin_cs=2
pin_clk=3
pin_miso=4
pin_mosi=17
time_gap=0.005
GPIO.setup(pin_led,GPIO.OUT)
GPIO.setup(pin_cs,GPIO.OUT)
GPIO.setup(pin_clk,GPIO.OUT)
GPIO.setup(pin_mosi,GPIO.OUT)
GPIO.setup(pin_miso,GPIO.IN)

GPIO.output(pin_led,0)
GPIO.output(pin_cs,1)
GPIO.output(pin_clk,0)

reg=0
def end_read(signal,frame):
    global continue_reading
    print("Ctrl+C captured, ending read.")
    continue_reading = False
    GPIO.cleanup()

# Hook the SIGINT
signal.signal(signal.SIGINT, end_read)

def spi_write(spi_data):
    GPIO.output(pin_clk,0)
    GPIO.output(pin_mosi,0)
    for x in range(8):
        if((spi_data & 0x80)==0x80):
            GPIO.output(pin_mosi,1)
            #print("1")
        else:
            GPIO.output(pin_mosi,0)
            #print("0")
        sleep(time_gap)
        GPIO.output(pin_clk,1)
        spi_data <<=1
        sleep(time_gap)
        GPIO.output(pin_clk,0)
    sleep(time_gap)

def spi_read():
    GPIO.output(pin_clk,0)
    GPIO.output(pin_mosi,0)
    spi_data=0
    for x in range(8):
        GPIO.output(pin_clk,1)
        sleep(time_gap)
        if GPIO.input(pin_miso):
            spi_data |=(1<<(7-x))
            print("1")
        else:
            print("0")
        GPIO.output(pin_clk,0)
        sleep(time_gap)
    sleep(time_gap)
    return spi_data

while continue_reading:
    sleep(1)
    GPIO.output(pin_led,1)
    GPIO.output(pin_cs,0)
    print("send:"+str(reg))
    spi_write(reg)    
    print("receive:0x%x " % (spi_read()))
    GPIO.output(pin_cs,1)
    GPIO.output(pin_led,0)
    sleep(1)
    
    reg +=1
    if(reg==6):
        reg=0
    