import sys
from PyQt4 import QtCore
from PyQt4 import QtGui
from NetworkGPS import QNetworkGPS
from NetworkText import QNetworkText
from random import randint
from datetime import datetime
import os


class QNetworkGraph(QtGui.QWidget):
    def __init__(self):
        QtGui.QWidget.__init__(self)

        self.NetworkGraph = QtGui.QTabWidget()
        self.NetworkGraph.setTabPosition(QtGui.QTabWidget.South)

        self.NetworkGPS = QNetworkGPS()
        self.NetworkText = QNetworkText()
        
        self.NetworkGraph.addTab(self.NetworkGPS,"Graph Mode")
        self.NetworkGraph.addTab(self.NetworkText,"Text Mode")

        self.layout = QtGui.QVBoxLayout(self)
        self.layout.addWidget(self.NetworkGraph)
        
        self.setLayout(self.layout)

    def setMarkGPS(_latitude, _longitude, _info):
        print "Set Mark"
        #Send to Javascript Markset

    def setCenterGPS(_latitude, _longitude):
        print "Set Center"
        #Send to Javascript center
        


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
