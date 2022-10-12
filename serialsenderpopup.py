#!/opt/miniconda/miniconda3/bin/python3
#sending serial with input text gui
import time
import sys
from multiprocessing import Process

import PySide2.QtWidgets

from serialsender import kirim
app = PySide2.QtWidgets.QApplication(sys.argv)

le = PySide2.QtWidgets.QLineEdit()
def proses():
    p = Process(target=kirim, args=(le.text().split(" "),0.4,False,))
    p.daemon = True
    p.start()
    le.hide()
    time.sleep(2)
    exit(0)
le.returnPressed.connect(proses)
le.show()

app.exec_()
