#!/usr/bin/env python
import os
import subprocess
import sys
sys.path.append('..')
import enc

files = os.listdir('.')

device = '/dev/ttyUSB*'
ESP_DIR = '/data'
try:
	subprocess.check_output(f"ls {device}", shell=True)
except subprocess.CalledProcessError as e:
	device = '/dev/tty.usbserial*'
password = None
for f in files:
	if f.endswith('.py') or f.endswith('.dontupload'):
		continue
	if f.endswith('.raw'):
		print(f'encrypting {f}')
		with open(f) as fobj:
			string = fobj.read()
			if string[-1]=='\n':
				string = string[:-1] #remove last empty line
			if password is None:
				print("password used for encryption:")
				password = input()
			m = enc.encrypt(string,password)
			newfilename = f.replace(".raw","")
			with open(newfilename,"wb") as fwrite:
				fwrite.write(m)
				print(f"done making {newfilename}, deleting {f}")
				os.unlink(f)
				f = newfilename
	cmd = f"pyboard.py --no-soft-reset -d {device} -f cp {f} :{ESP_DIR}/{f}"
	print(cmd)
	subprocess.check_output(cmd,shell=True)
	
