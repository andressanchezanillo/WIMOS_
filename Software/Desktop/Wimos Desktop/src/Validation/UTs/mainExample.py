import sys
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtWebKit import *
from NetworkWindows import QNetworkWindows
from random import randint, uniform
from datetime import datetime
import os


class exampleQMainWindow (QWidget):
    def __init__ (self):
        super(exampleQMainWindow, self).__init__()

        # Create QListWidget
                
        self.InfoLayout = QVBoxLayout(self)  
        self.setLayout(self.InfoLayout)
        
        self.list = QNetworkWindows()
        
        self.InfoLayout.addWidget(self.list)
        
        self.button = QPushButton()
        self.InfoLayout.addWidget(self.button)
        self.button.clicked.connect(self.sendInfo)

    def sendInfo(self):
        ranValue = str(randint(1, 10))
        self.list.addInfo('123','TestFrame',
                          '1','1',
                          '2017-03-11',
                          '12:03:54.234',
                          str(uniform(34.5, 36.8)),
                          '-6.45312',
                          '45%', '76%',
                          '120',
                          datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
        
        self.list.addAlert('01','TestFrame',
                          '0x01',str(randint(0,10)),
                          '2017-03-11',
                          '12:03:54.234',
                          '1','2','3','4','5',
                          datetime.now().strftime('%Y-%m-%d %H:%M:%S'))

app = QApplication([])
window = exampleQMainWindow()
window.show()
sys.exit(app.exec_())
