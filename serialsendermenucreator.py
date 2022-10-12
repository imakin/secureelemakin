#!/opt/miniconda/miniconda3/bin/python3
import subprocess
import os
import sys

home = os.environ['HOME']

def makelauncher(name):
    content = f"""[Desktop Entry]
Type=Application
Exec={home}/.local/bin/serialsender {name} 10
Name=secure {name}
"""
    targetfile = f"{home}/.local/share/applications/secure_{name}.desktop"
    try:
        with open(targetfile,"w") as f:
            f.write(content)
    except:print(f"not making {targetfile}")
    os.chmod(targetfile, 0b111111111)

BASE_DIR = os.path.dirname( os.path.realpath(sys.argv[0]) )
os.chdir(BASE_DIR)

hasil = subprocess.check_output("python serialsender.py list 10", shell=True).decode('utf8').replace('\r\n','\n')
mark = "text is: list"
position = hasil.find(mark)
if position<0:
    print('error')
hasil = hasil[hasil.find(mark):]
print(hasil)
hasils = hasil.split('\n')

if (hasils[0]!=mark):
    print(hasils[0],'vs',mark)
    print('error')

for name in hasils[1:]:#first line is {mark}, last line is "done listing"
    name = name.strip()
    if not(name) or name=="done listing" or name.startswith("otp_"):continue
    print(f"making launcher [{name}]")
    makelauncher(name)
