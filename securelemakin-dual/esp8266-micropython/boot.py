import os

import wificonnect

expected_lib = ['base64.mpy', 'binascii.mpy', 'hmac.mpy', 'struct.mpy']

if os.listdir('/lib')!=expected_lib:
    import libinstall

from machine import UART
import sys

# ~ uart = UART(0,baudrate=115200)
# ~ b = sys.stdin.read(10)
with open('bacaan.bin','wb') as f:
    f.write(bytearray(input().encode('utf8')))
