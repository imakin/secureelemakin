#this file will contain dict variable data. fill it as necessary
#data
data = {}
with open('data.bin','rb') as f:
    buff = f.read()
    buff = bytearray(buff)
    data['anote'] = buff

def get_data()->dict:
    return data
