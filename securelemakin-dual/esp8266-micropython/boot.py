import os

import wificonnect
import time

expected_lib = ['base64.mpy', 'binascii.mpy', 'hmac.mpy', 'struct.mpy']

if os.listdir('/lib')!=expected_lib:
    import libinstall


#main app
import atmega32u4
