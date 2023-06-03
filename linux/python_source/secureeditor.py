"""
to make securedata
data format is:
    [item_name in utf8][space][chiper in binary][NULL NULL NULL (tripple null)]
    [next_item_name in utf8][space][next_chiper in binary][NULL NULL NULL (tripple null)]
    [next_item_name in utf8][space][next_chiper in binary][NULL NULL NULL (tripple null)]
    [next_item_name in utf8][space][next_chiper in binary][NULL NULL NULL (tripple null)]
    [next_item_name in utf8][space][next_chiper in binary][NULL NULL NULL (tripple null)]
"""
import libsecurelemakin
from timeoutmanager import (setTimeout, clearTimeout)
import sys
import uuid

from PySide2.QtWidgets import (
    QApplication
    ,QMainWindow
    ,QWidget
    ,QPushButton
    ,QLineEdit
    ,QLabel
    ,QFileDialog
    ,QFrame
    ,QHBoxLayout
)
from PySide2.QtUiTools import QUiLoader

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Bare Application")
        loader = QUiLoader()
        self.ui = loader.load("mainwindow.ui")
        self.setCentralWidget(self.ui)
        self.setWindowTitle("Securelemakin Secure Storage Editor")
        
        self.sc = self.findChild(QWidget, "sc")
        self.iv_sc = self.sc.layout()
        self.sc.setStyleSheet("""
        QLineEdit{
            padding: 10px;
        }
        """)
        
        self.bt_open = self.findChild(QPushButton, "bt_open")
        self.bt_open.clicked.connect(self.open_file_dialog)
        self.le_file = self.findChild(QLineEdit, "le_file")
        
        self.bt_save = self.findChild(QPushButton, "bt_save")
        self.bt_save.clicked.connect(self.save)
        
        self.bt_add = self.findChild(QPushButton, "bt_add")
        self.bt_add.clicked.connect(self.add_item)
        
        self.bt_refresh = self.findChild(QPushButton, "bt_refresh")
        self.bt_refresh.clicked.connect(self.load)
        self.le_key = self.findChild(QLineEdit, "le_key")
        
        self.bt_showhide = self.findChild(QPushButton, "bt_showhide")
        self.bt_showhide.clicked.connect(self.show_hide_data)
        
        self.data = {}
        
    
    def get_key(self):
        return self.le_key.text()

    def open_file_dialog(self):
        file_dialog = QFileDialog()
        file_path = file_dialog.getSaveFileName(self, "Open File or Save as")
        if file_path[0]:
            self.le_file.setText(file_path[0])
            try:
                with open(file_path[0],"rb") as f:
                    pass
            except FileNotFoundError:
                #new file
                pass

    def show_hide_data(self):
        les = [le for le in self.sc.findChildren(QLineEdit) if le.placeholderText()=="raw value"]
        if len(les)==0:return
        v = QLineEdit.Normal
        if les[0].echoMode()==QLineEdit.Normal:
            v = QLineEdit.Password
        for le in les:
            le.setEchoMode(v)
                

    def add_item(self, name="",raw=""):
        fr = QFrame(self.sc)
        fr.setObjectName(uuid.uuid4().hex)#random
        hl = QHBoxLayout(fr)
        le_name = QLineEdit(fr)
        le_name.setPlaceholderText("name")
        le_name.setMaximumWidth(300)
        le_name.setText(name)
        le_raw = QLineEdit(fr)
        le_raw.setPlaceholderText("raw value")
        le_raw.setText(raw)
        hl.addWidget(le_name)
        hl.addWidget(le_raw)
        self.iv_sc.addWidget(fr)
        
        self.tmr_edited = None
        def execute_on_item_edited():
            self.on_item_edited(fr)
        def maybe_execute_on_item_edited():
            clearTimeout(self.tmr_edited)
            self.tmr_edited = setTimeout(execute_on_item_edited,1)

        le_name.textEdited.connect(maybe_execute_on_item_edited)
        le_raw.textEdited.connect(maybe_execute_on_item_edited)
        return fr.objectName() #uuid
        
    
    def on_item_edited(self,item_frame):
        uuid = item_frame.objectName()
        children = item_frame.findChildren(QLineEdit)
        le_raw = [c for c in children if c.placeholderText()=="raw value"][0]
        le_name = [c for c in children if c.placeholderText()=="name"][0]
        # ~ self.data[uuid] = libsecurelemakin.encrypt(self.get_key(), le_raw.text())
        
    
    def save(self):
        frs = self.sc.findChildren(QFrame)
        with open(self.le_file.text(),"wb") as f:
            for item_frame in frs:
                uuid = item_frame.objectName()
                children = item_frame.findChildren(QLineEdit)
                le_name = [c for c in children if c.placeholderText()=="name"][0]
                le_raw = [c for c in children if c.placeholderText()=="raw value"][0]
                
                name = le_name.text().replace(" ","_") #no space allowed
                name += " " #space used for separator of name from data
                print(f"saving {name}")
                f.write(name.encode('utf8'))
                f.write(libsecurelemakin.encrypt(self.get_key(), le_raw.text()) + b'\0\0\0') #\0\0\0 is used as separator between item
    
    def load(self):
        #clear data
        self.data = {}
        frs = self.sc.findChildren(QFrame)
        for item_frame in frs:
            layout = item_frame.layout()
            while layout.count():
                c = layout.takeAt(0)
                widget = c.widget()
                if widget:
                    widget.deleteLater()
            item_frame.deleteLater()
        #read file
        with open(self.le_file.text(),"rb") as f:
            data = f.read()
            i = 0
            while i<len(data):
                name = ""
                while data[i]!=ord(' '):
                    name += chr(data[i])
                    i += 1
                i += 1 #skip the ord(' ')
                chiper = b""
                while (i+2)<len(data) and not(
                    data[i]==0 and
                    data[i+1]==0 and
                    data[i+2]==0
                ):
                    chiper += bytes([data[i]])
                    i += 1
                i += 3 #skip the \0\0\0
                raw_data = libsecurelemakin.decrypt(self.get_key(), chiper).decode('utf8')
                uuid = self.add_item(name,raw_data)
                # ~ self.data[uuid] = chiper
                


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
