#!/opt/miniconda/miniconda3/bin/python3
import sys
import serial
import traceback
import os
ser = serial.Serial(port='/dev/ttyACM0', baudrate=115200)
import time
DEBUG=True
try:
    os.environ['SILENT']
    DEBUG = False
except:pass

def kirim(inputs, delay=0, debug=True):
    global DEBUG
    time.sleep(delay)
    if not(debug):
        DEBUG = False
    if DEBUG:print(inputs)
    data = []
    for i in range(0,len(inputs)):
        try:
            #treat current argument as one byte 
            data.append(int(inputs[i]))
        except ValueError:
            #treat current argument as string (multiple byte array)
            word = inputs[i].encode('utf8')
            for c in word:
                data.append(c)
    # ~ data.append(ord('\n')) //you must append manuali in input with 10
    bdata = bytes(data)
    ser.write(bdata)
    if not(DEBUG):
        return

    if DEBUG:print("sending: ",bdata)
    if DEBUG:print("sending: ",[int(c) for c in bdata])

    ser.timeout = 1
    if DEBUG:print("trying to read:")
    masukan = []
    try:
        while True:
            c = ser.read()[0]
            masukan.append(c)
    except IndexError:
        pass
    except Exception as e:
        traceback.print_exception()
    finally:
        if DEBUG:print(masukan)
        if DEBUG:print("".join(chr(c) for c in masukan))
        ser.close()


if __name__=="__main__":
    inputs = sys.argv[1:]
    if (len(inputs)<1):
        if DEBUG:print("send all arguments through serial, valid byte number will be send as is, anything else will be send each charracter as byte ")
        if DEBUG:print("example `python serialsender.py 254 aaaaa 10`")
        if DEBUG:print("         will send: [254, 97, 97, 97, 97, 97, 10]")
        if DEBUG:print("example `python serialsender.py 254 254a`")
        if DEBUG:print("         will send: [254, 50, 53, 52, 97, 10]")
        if DEBUG:print("and then will print the serial read result (timeout 3s)")
        if DEBUG:print("Note: no newline automatically added, add number 10 for \\n ")
        if DEBUG:print("if no argument inputed, read standard input in one line")
        if DEBUG:print("please input data bellow...")
        masukan = input()
        inputs = masukan.split(" ")
    kirim(inputs)
