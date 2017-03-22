import sys
from PyQt4 import QtCore
from PyQt4 import QtGui
from random import randint
from datetime import datetime
import os
import glob


class QNetworkGraph(QtGui.QWidget):
    def __init__(self):
        QtGui.QWidget.__init__(self)

        # Section Mode
        # Create the Layout structure.
        self.ModeLayout = QtGui.QHBoxLayout(self)
        self.ModeLayoutRight = QtGui.QVBoxLayout(self)
        self.ModeLayoutLeft = QtGui.QVBoxLayout(self)

        # Create the Selection Mode Section
        self.ModeSectionLabel = QtGui.QLabel("Select a Mode:")
        self.SelectMode = QtGui.QComboBox()
        self.SelectMode.addItem("--- Select Mode ---")
        self.SelectMode.addItem("Normal Mode")
        self.SelectMode.addItem("Validation Mode")
        # Save the Release widget.
        self.ModeLayoutRight.addWidget(self.ModeSectionLabel) 
        self.ModeLayoutRight.addWidget(self.SelectMode) 

        # Create the Selection of Submode.
        self.SubmodeSectionLabel = QtGui.QLabel("Select a Normal Mode:")
        self.SelectSubmode = QtGui.QComboBox()
        self.SelectSubmode.addItem("--- Select Release Version ---")
        self.SelectSubmode.addItem("Normal Mode")
        self.SelectSubmode.addItem("Debug Mode")
        self.SelectSubmode.addItem("Debug Color Mode")
        self.SelectSubmode.addItem("Debug Communication Frames")
        self.SelectSubmode.addItem("Debug Communication Status")
        self.SelectSubmode.addItem("Debug Analog A5")
        # Save the Device Widget.
        self.ModeLayoutLeft.addWidget(self.SubmodeSectionLabel)
        self.ModeLayoutLeft.addWidget(self.SelectSubmode)
    
        # Save all Layout.    
        self.ModeLayout.addLayout(self.ModeLayoutRight)
        self.ModeLayout.addLayout(self.ModeLayoutLeft)
        
        self.setLayout(self.ModeLayout)
        


class exampleQMainWindow (QtGui.QWidget):
    def __init__ (self):
        super(exampleQMainWindow, self).__init__()

        # Create QListWidget
                
        self.InfoLayout = QtGui.QVBoxLayout(self)  
        self.setLayout(self.InfoLayout)
        
        self.list = QNetworkGraph()
        
        self.InfoLayout.addWidget(self.list)
        
        self.button = QtGui.QPushButton()
        self.InfoLayout.addWidget(self.button)
        #self.button.clicked.connect(self.sendInfo)

    def sendInfo(self):
        ranValue = str(randint(1, 10))
        self.list.addInfo('01','TestFrame',
                          '0x01','0x02',
                          '2017-03-11',
                          '12:03:54.234',
                          '36.56234',
                          '-6.45312',
                          '45%', '76%',
                          '120',
                          datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
        
        self.list.addAlert('01','TestFrame',
                          '0x01','0x02',
                          '2017-03-11',
                          '12:03:54.234',
                          '1','2','3','4','5',
                          datetime.now().strftime('%Y-%m-%d %H:%M:%S'))

        print ranValue

app = QtGui.QApplication([])
window = exampleQMainWindow()
window.show()
sys.exit(app.exec_())
