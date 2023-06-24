import os
#this file will contain dict variable for secure data. fill it as necessary
#data in get_data(data_name)
#data can be from file in /data folder, make it lazy load to save ram

DATA_DIR = '/data'

try:
    os.listdir(DATA_DIR)
except OSError:
    os.mkdir(DATA_DIR)

def get_data_keys()->list:
    return os.listdir(DATA_DIR)

def get_data(name="")->bytes:
    if name:
        print(f"getting data {name}")
        try:
            f = open(f"{DATA_DIR}/{name}", 'rb')
            buff = f.read()
            return buff
        except OSError:
            print(f"file not found {name}")
        finally:
            f.close()
