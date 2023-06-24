import os
from machine import *
import time
import socket

import enc
import data_manager

ledpin = Pin(2,Pin.OUT)
def led(on_or_off):
    if type(on_or_off)==int:
        ledpin.value(1^on_or_off) #bitwise not
    elif type(on_or_off)==bool:
        if on_or_off: ledpin.value(0)
        else: ledpin.value(1)
    elif type(on_or_off)==str and on_or_off=="toggle":
        ledpin.value(1^ledpin.value())

adc = ADC(0)
adc_threshold = 20 #when adc pin untouched, usually value is <8

command_manager = None
class CommandManager(object):
    def __init__(self):
        global command_manager
        if command_manager!=None:
            raise Exception("command_manager singleton already instantiated")
    
    def singleton(self):
        global command_manager
        if command_manager==None:
            command_manager = CommandManager()
        return command_manager
    
    def cmd_print(self,securedata_name):
        b = data_manager.get_data(securedata_name)
        #get pass 
        #decrypt
    
    """
    data_message examples:
        "cmd_print securedata_name_secret"
        "cmd_otp mygmail"
        "cmd_password mypassword"
    first word is method in this class
    """
    def process(data_message):
        m = str(data_message)
        try:
            first_space = m.find(' ')]
            cmd = m[:first_space]
            param = m[first_space:]
            if cmd.startswith('cmd_'):
                getattr(self,cmd)(param)
            else:
                raise ValueError('malformed...')
        except:
            raise ValueError(f"malformed data {m}")

command_manager = CommandManager()

def process_command(data_message):
    # ~ if data_message.
    
    elif data_message.startswith("print "):
        securedata_name = data_message.split(' ')[1]
        buff = data_manager.get_data(securedata_name)
    

def routine():
    html = """<label>securedataid: <input id="securedataid" placeholder="securedataid" /></label>
    """
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(['',80])
    s.listen()
    
    uart = UART(0,baudrate=115200,timeout=500)

    address = 1 #atmega32u4 address
    i2c = I2C(sda=Pin(4),scl=Pin(5))
    sampling = 0
    
    led(1)
    while True:
        #i2c routine
        try:
            expecting_length_b = i2c.readfrom(address, 1)
            if len(expecting_length_b):
                l = int(expecting_length_b[0])
                print(f"got data from 32u4 {l}")
                if (l>0):
                    data = i2c.readfrom(address, l)
                    if len(data)>0:
                        print(f"received {type(data)} {data}")
                        message = "ok bos"
                        i2c.writeto(address,message.encode('utf8'))
        except OSError:
            sampling += 1
            if sampling>=10:
                print("no data")
                sampling = 0
            
        
        #UART routine
        serialdata = uart.read()
        if serialdata:
            print("atmega32u4 routine exit because uart is coming")
            print(serialdata)
            with open("lastdata.bin","wb") as f:
                f.write(bytearray(serialdata))
            break
        
        
        #ADC & web server routine
        if adc.read()>adc_threshold:
            print("adc touched, web server mode")
            led("toggle")
            s.settimeout(1)
            while adc.read()>adc_threshold:
                led("toggle")
                try:
                    conn, requesteraddr = s.accept()
                except:
                    continue
                request_message = str(conn.recv(1024))
                conn.send("HTTP/1.1 200 OK\r\n")
                conn.send("Content-Type: text/html\r\n")
                conn.send("Connection: close\r\n\r\n")
                conn.sendall(html)
                conn.close()
            
try:
    routine()
except KeyboardInterrupt: #so that pyboard --no-soft-reset can do file upload
    led(0)
    print("atmega32u4 routine exits because of keyboard interrupt")
