import sys
from PyQt4 import QtCore, QtGui
from NetworkThreads import QNetworkThreads
from random import randint
from datetime import datetime
import serial


class QNetworkWindows(QtGui.QWidget):
    def __init__ (self):
        super(QNetworkWindows, self).__init__()
        
        self.NetworkLayout = QtGui.QHBoxLayout(self)
        self.NetworkThread = QNetworkThreads()
        
        self.NetworkLayout.addWidget(self.NetworkThread)
        self.setLayout(self.NetworkLayout)



    def TestUTs(self):
        
        self.NetworkThread.TestUTs()

    def TestVT(self):
        
        self.NetworkThread.TestVT()
