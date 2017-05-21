from PyQt4 import QtCore
from PyQt4 import QtGui
from NetworkGPS import QNetworkGPS
from NetworkText import QNetworkText
from random import randint
from datetime import datetime
import os
import time
import sys

class QNetworkGraph(QtGui.QWidget):
    def __init__(self):
        QtGui.QWidget.__init__(self)

        self.NetworkGraph = QtGui.QTabWidget()
        self.NetworkGraph.setTabPosition(QtGui.QTabWidget.South)

        self.NetworkGPS = QNetworkGPS()
        self.NetworkGPS.setStatusTip("Text display")

        self.NetworkText = QNetworkText()
        self.NetworkText.setStatusTip("Map display")
        
        self.NetworkGraph.addTab(self.NetworkGPS,"Graph Mode")
        self.NetworkGraph.addTab(self.NetworkText,"Text Mode")

        self.layout = QtGui.QVBoxLayout(self)
        self.layout.addWidget(self.NetworkGraph)

        self.timeoutConnection = 5000
        self.timeoutList = []
        
        self.setLayout(self.layout)

    def addCenter(self,
                  _FrameName, _CenterID,
                  _CurrentDateTime,
                  _messageRatio, _messageTime):
        
        self.NetworkText.addCenter(_FrameName, _CenterID,
                                   _CurrentDateTime,
                                   _messageRatio, _messageTime)
        

    def addInfo(self,
                _FrameID, _FrameName,
                _IdCenter, _IdHost,
                _SystemDate, _SystemTime,
                _GpsLatitude, _GpsLongitude,
                _Memory, _Battery, _Status,
                _CurrentDateTime):
        
        found = False;
        self.NetworkText.addInfo(_FrameID, _FrameName,
                                _IdCenter, _IdHost,
                                _SystemDate, _SystemTime,
                                _GpsLatitude, _GpsLongitude,
                                _Memory, _Battery, _Status,
                                _CurrentDateTime)

        self.NetworkGPS.addInfo(_IdHost, _GpsLatitude,
                                _GpsLongitude, "Hello Word")
        
        for idDev, lastTime in self.timeoutList:
            if(idDev is _IdHost):
                lastTime = int(round(time.time() * 1000))

        if not found:
            self.timeoutList.append((_IdHost, int(round(time.time() * 1000))))



    def addAlert(self,
                _FrameID, _FrameName,
                _IdCenter, _IdHost,
                _SystemDate, _SystemTime,
                _AlertA1, _AlertA2, _AlertA3,
                _AlertA4, _AlertA5, _AlertLevel,
                _CurrentDateTime):
        
        found = False
        
        self.NetworkText.addAlert(_FrameID, _FrameName,
                                _IdCenter, _IdHost,
                                _SystemDate, _SystemTime,
                                _AlertA1, _AlertA2, _AlertA3,
                                _AlertA4, _AlertA5,
                                _CurrentDateTime)

        self.NetworkGPS.addAlert(_IdHost, _AlertLevel)

    def disconnect(self, idDevice):
        self.NetworkGPS.disconnect(idDevice)
        self.NetworkText.disconnect(idDevice)

    def eraseAlert(self):
        self.NetworkGPS.eraseAlert(idDevice)



    def TestUTs(self):

##        resultn8UT01 = self.n8UT01()
##        if ( resultn8UT01 == 100):
##            print("[TEST] n8UT01\t[OK]\tn8.UT01 = 100%")
##        else:
##            print("[TEST] n8UT01\t[ERROR]\tn8.UT01 = "+(str(resultn8UT01))+"%")
        print("In progress..")
        self.NetworkGPS.TestUTs()
        self.NetworkText.TestUTs()
        

    def TestVT(self):
        print("In progress..")        
        self.NetworkGPS.TestVT()
        self.NetworkText.TestVT()
        
        #self.NetworkThread.TestVT()        
        
