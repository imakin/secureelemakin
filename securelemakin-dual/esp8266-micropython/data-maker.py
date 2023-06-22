from enc import *

print("set password: ")
pw = input()
with open('data.raw') as f:
    whole = f.read()
    with open('data.bin', 'wb') as chip:
        buff = encrypt(whole,pw)
        chip.write(buff)
