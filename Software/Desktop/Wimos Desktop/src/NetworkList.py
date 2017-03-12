import sys
from PyQt4 import QtCore, QtGui
from NetworkItem import QNetworkItem
from random import randint
import time, threading

class QNetworkList (QtGui.QWidget):

    def __init__ (self, parent = None):
        super(QNetworkList, self).__init__(parent)
        #self.NetworkList = QtGui.QListWidget(self)
        QtGui.QWidget.__init__(self)
        self.mygroupbox = QtGui.QGroupBox()
        self.myform = QtGui.QFormLayout()
        self.NetworkItemArray = []

        self.mygroupbox.setLayout(self.myform)
        self.scroll = QtGui.QScrollArea()
        self.scroll.setWidget(self.mygroupbox)
        self.scroll.setWidgetResizable(True)
        self.scroll.setFixedWidth(355)
        self.scroll.setMinimumHeight(440)
        self.layout = QtGui.QVBoxLayout(self)
        self.layout.addWidget(self.scroll)

        self.timeoutDataIn = 250
        self.timeoutConnection = 5000

        #self.displayControl()
        
    def addInfo(self, idDevice, typeD, batteryD):
        varFound = False

        for item in self.NetworkItemArray:
            if(item.getID() == int(idDevice)):
                item.addInfo(idDevice,typeD,batteryD)
                varFound = True

        if not varFound:
            line = QtGui.QFrame();
            line.setFrameShape(QtGui.QFrame.HLine)
            line.setFrameShadow(QtGui.QFrame.Sunken)
            self.NetworkItemArray.append(QNetworkItem())            
            self.NetworkItemArray[-1].addInfo(idDevice,typeD,batteryD)
            self.myform.addRow(self.NetworkItemArray[-1])
            self.myform.addRow(line)

        
    def addAlert(self, idDevice, alertDevice):
        for item in self.NetworkItemArray:
            if(item.getID() == int(idDevice)):
                item.addAlert(idDevice,alertDevice)
            
            
    def refresh(self):
        for item in self.NetworkItemArray:        

            if (int(round(time.time() * 1000)) - item.getLastUpdateTime()) > self.timeoutDataIn:
                item.addAlert(item.getID(), 'none', True)
                
            if (int(round(time.time() * 1000)) - item.getLastUpdateTime()) > self.timeoutConnection:
                if item.getStatus():
                    item.enable = False
                    item.disconnect()


