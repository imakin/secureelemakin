import os
from machine import (
    Pin,
    unique_id,
    I2C,
    UART,
    # ~ ADC,
)
import time
import socket
import gc

gc.collect()
print(gc.mem_free())
import totp
gc.collect()
print(gc.mem_free())
import enc
import secret
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


class keyboard(object):
    keyboard_echo_pin = Pin(15,Pin.OUT)
    @staticmethod
    def on():
        keyboard.keyboard_echo_pin.value(1)
    @staticmethod
    def off():
        keyboard.keyboard_echo_pin.value(0)
    

# ~ adc = ADC(0)
# ~ adc_threshold = 20 #when adc pin untouched, usually value is <8



class Password(object):
    uid = unique_id() #machine.unique_id
    uid_hex = "".join([ ("0"+hex(c)[2:])[-2:] for c in uid])
    password_file = 'master_secret.bin'#contains password, but this password is also encrypted with master_key
    exist = False
    password_passkey = None
    cipher = None


    def __init__(self,enc_context, prime, salt):
        self.enc = enc_context
        uid_hash = self.enc.hashpassword(
            self.uid_hex
            ,prime=prime
            ,salt=salt
        )
        self.password_passkey = "".join([chr(32+(c%(128-32))) for c in uid_hash])
        self.exist = False

    def get(self):
        if self.exist:
            temp = bytearray(self.cipher)
            return self.enc.bytearray_strip(self.enc.decrypt(temp,self.password_passkey))
        else:
            try:
                f = open(self.password_file,'rb')
                self.cipher = bytearray(f.read())
                self.exist = True
                temp = bytearray(self.cipher)
                return self.enc.bytearray_strip(self.enc.decrypt(temp,self.password_passkey))
            except:
                self.exist = False
                raise Exception('password not set')
            finally:
                try:f.close()
                except:pass


    def set(self,password):
        b = self.enc.encrypt(password,self.password_passkey)
        with open(self.password_file,'wb') as f:
            f.write(b)
            self.cipher = bytearray(b)
            gc.collect()
            





command_manager = None
class CommandManager(object):
    def __init__(self):
        global command_manager
        if command_manager!=None:
            raise Exception("command_manager singleton already instantiated")
        self.password = Password(enc, secret.Password.prime, secret.Password.salt)
    
    def set_context(self,ctx):
        self.ctx = ctx
    
    def singleton(self):
        global command_manager
        if command_manager==None:
            command_manager = CommandManager()
        return command_manager
    
    def cmd_print(self,securedata_name):
        b = bytearray(data_manager.get_data(securedata_name))
        s = enc.bytearray_strip(
            enc.decrypt(b,self.password.get())
        )
        keyboard.on()
        self.ctx.i2c.writeto(self.ctx.address,s.encode('utf8'))
        keyboard.off()#32u4 only check pin on the begining of i2c data, so it's safe to off() while 32u4 is still receiving

    """
    do keyboard print for chr(char_byte)
    usefull for one key charracter or keyboard like 32 (space) 10 (linefeed) 9(tab)
    """
    def cmd_print_char(self,char_byte):
        keyboard.on()
        self.ctx.i2c.writeto(self.ctx.address,bytes([char_byte]))
        keyboard.off()#32u4 only check pin on the begining of i2c data, so it's safe to off() while 32u4 is still receiving

    def cmd_password(self,password):
        self.password.set(password)
    
    def cmd_otp(self,securedata_name):
        b = bytearray(data_manager.get_data(securedata_name))
        s = enc.bytearray_strip(
            enc.decrypt(b,self.password.get())
        )
        pin = totp.now(s)
        keyboard.on()
        self.ctx.i2c.writeto(self.ctx.address,pin.encode('utf8'))
        keyboard.off()#32u4 only check pin on the begining of i2c data, so it's safe to off() while 32u4 is still receiving

    """
    data_message examples:
        "cmd_print securedata_name_secret"
        "cmd_otp mygmail"
        "cmd_password mypassword"
    first word is method in this class
    """
    def process(self,data_message):
        if type(data_message) in [bytes,bytearray]:
            m = data_message.decode('utf8')
        elif type(data_message)==str:
            m = data_message
        else:
            raise ValueError("parameter data_message must be str,bytes,or bytearray")
        try:
            first_space = m.find(' ')
            cmd = m[:first_space]
            param = m[first_space+1:]
            print(cmd)
            print(param)
            if cmd.startswith('cmd_'):
                getattr(self,cmd)(param)
            else:
                raise ValueError('malformed...')
        except Exception as e:
            raise e
            # ~ print(e)
            # ~ raise ValueError(f"malformed data {m}")

class Routine(object):
    def __init__(self):
        html = """to request use GET method example: curl [esp ip address]/[command]/[parameter]
        """
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind(['',80])
        self.server.listen()
        self.server.settimeout(0.5)
        self.server_last_connection = time.time()
        
        self.uart = UART(0,baudrate=115200,timeout=500)

        self.address = 1 #atmega32u4 self.address
        self.i2c = I2C(sda=Pin(4),scl=Pin(5))
        sampling = 0
        
        self.command_manager = CommandManager()
        self.command_manager.set_context(self)
        
        keyboard.off()
        led(1)
        while True:
            #self.i2c routine
            try:
                expecting_length_b = self.i2c.readfrom(self.address, 1)
                if len(expecting_length_b):
                    l = int(expecting_length_b[0])
                    print(f"got data from 32u4 {l}")
                    if (l>0):
                        data = self.i2c.readfrom(self.address, l)
                        if len(data)>0:
                            print(f"received {type(data)}: {data}")
                            try:
                                self.command_manager.process(data)
                            except Exception as e:
                                print("wrong command")
                                print(e)
            except OSError:
                sampling += 1
                if sampling>=10:
                    print("no data")
                    sampling = 0
                
            
            #UART routine
            serialdata = self.uart.read()
            if serialdata:
                print("atmega32u4 routine exit because self.uart is coming")
                print(serialdata)
                with open("lastdata.bin","wb") as f:
                    f.write(bytearray(serialdata))
                break
            
            
            #ADC & web server routine
            # ~ if adc.read()>adc_threshold:
                # ~ print("adc touched, web server mode")
                # ~ led("toggle")
                # ~ self.server.settimeout(1)
                # ~ while adc.read()>adc_threshold:
            led("toggle")
            havent_tried = True
            while havent_tried or (time.time()-self.server_last_connection)<1:
                havent_tried = False
                try:
                    conn, requesteraddr = self.server.accept()
                except:
                    led("toggle")
                    continue
                self.server_last_connection = time.time()
                led("toggle")
                request_message = conn.recv(1024).decode('utf8')
                lines = request_message.split('\r\n') #to get the first line
                GET_path = lines[0].split(' ')[1] #to get the 2nd word from the first line
                commands = GET_path.split('/')
                command = " ".join([word for word in commands if len(word)>0])
                try:
                    self.command_manager.process(command)
                except Exception as e:
                    print(f"wrong command:\n\t{GET_path}\n\t{command}")
                    print(e)
                
                conn.send("HTTP/1.1 200 OK\r\n")
                conn.send("Content-Type: text/html\r\n")
                conn.send("Connection: close\r\n\r\n")
                conn.sendall(html)
                conn.close()
                
try:
    Routine()
except KeyboardInterrupt: #so that pyboard --no-soft-reset can do file upload
    led(0)
    print("atmega32u4 routine exits because of keyboard interrupt")
