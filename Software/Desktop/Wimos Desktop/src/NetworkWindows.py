import sys
from PyQt4 import QtCore, QtGui
from NetworkList import QNetworkList
from NetworkGraph import QNetworkGraph
from random import randint
from datetime import datetime
import serial


class QNetworkWindows(QtGui.QWidget):
    def __init__ (self):
        super(QNetworkWindows, self).__init__()
        self.NetworkLayout = QtGui.QHBoxLayout(self)
        self.NetworkListLayout = QtGui.QVBoxLayout(self)
        self.NetworkListScroll = QNetworkList(self)
        self.NetworkInfo = QNetworkGraph()
        self.NetworkButton = QtGui.QPushButton()
        self.NetworkButton.setText("Connect")
        self.NetworkButton.clicked.connect(self.startSerial)      
        
        self.NetworkListLayout.addWidget(self.NetworkListScroll)
        self.NetworkListLayout.addWidget(self.NetworkButton)
        self.NetworkLayout.addLayout(self.NetworkListLayout)
        self.NetworkLayout.addWidget(self.NetworkInfo)
        self.setLayout(self.NetworkLayout)

        self.TimerRefresh = QtCore.QTimer(self)
        self.TimerRefresh.setInterval(250)
        self.TimerRefresh.setSingleShot(False)
        self.TimerRefresh.timeout.connect(self.refresh)
        self.TimerRefresh.start(250)

        self.TimerSerial = QtCore.QTimer(self)
        self.TimerSerial.setInterval(10)
        self.TimerSerial.setSingleShot(False)
        self.TimerSerial.timeout.connect(self.processSerial)

        self.Serial = {}
                

        self.startThreshold = 0
        self.noThreshold = 0
        self.lowThreshold = 1
        self.mediumThreshold = 6
        self.highThreshold = 10
        self.endThreshold = 10


    def startSerial(self):
        self.NetworkButton.setText("Disconnect");
        self.NetworkButton.clicked.connect(self.stopSerial)
        self.TimerSerial.start(10)
        self.Serial = serial.Serial(port='COM6',\
                                    baudrate=115200,\
                                    parity=serial.PARITY_NONE,\
                                    stopbits=serial.STOPBITS_ONE,\
                                    bytesize=serial.EIGHTBITS,\
                                    timeout=1)

    def stopSerial(self):
        self.TimerSerial.stop()
        self.NetworkButton.setText("Connect");
        self.NetworkButton.clicked.connect(self.startSerial)
        self.Serial.close()


        

    def processSerial(self):
        line = self.Serial.readline()
        if len(line) > 0:
            print line
            #values = line.split(' ')
            #if len(values) > 0 :
                #if values[-1] is "ACK-OK":
                    #if values[0] is "Frame-125":
                        
                    #elif values[0] is "Frame-125":
                        
                    #elif values[0] is "Frame-125":
            
        print "Processing..."
    
    def addInfo(self,
                _FrameID, _FrameName,
                _IdCenter, _IdHost,
                _SystemDate, _SystemTime,
                _GpsLatitude, _GpsLongitude,
                _Memory, _Battery, _Status,
                _CurrentDateTime):

        self.NetworkListScroll.addInfo(_IdHost, 'Host', _Battery )
        
        self.NetworkInfo.addInfo(_FrameID, _FrameName,
                                _IdCenter, _IdHost,
                                _SystemDate, _SystemTime,
                                _GpsLatitude, _GpsLongitude,
                                _Memory, _Battery, _Status,
                                _CurrentDateTime)

    def addAlert(self,
                _FrameID, _FrameName,
                _IdCenter, _IdHost,
                _SystemDate, _SystemTime,
                _AlertA1, _AlertA2, _AlertA3,
                _AlertA4, _AlertA5,
                _CurrentDateTime):

        Alert = max(int(_AlertA1), int(_AlertA2),
                    int(_AlertA3), int(_AlertA4),
                    int(_AlertA5))
        AlertLevel = 'none'

        if(Alert < self.startThreshold or Alert > self.endThreshold):
            AlertLevel = 'none'
        elif(Alert >= self.startThreshold and Alert <= self.noThreshold):
            AlertLevel = 'none'
        elif(Alert > self.noThreshold and Alert <= self.lowThreshold):
            AlertLevel = 'low'
        elif(Alert > self.lowThreshold and Alert <= self.mediumThreshold):
            AlertLevel = 'medium'
        elif(Alert >= self.mediumThreshold and Alert <= self.highThreshold):
            AlertLevel = 'high'
        else:
            AlertLevel = 'none'

        self.NetworkListScroll.addAlert(_IdHost, AlertLevel)
        
        self.NetworkInfo.addAlert(_FrameID, _FrameName,
                                    _IdCenter, _IdHost,
                                    _SystemDate, _SystemTime,
                                    _AlertA1, _AlertA2, _AlertA3,
                                    _AlertA4, _AlertA5, AlertLevel,
                                    _CurrentDateTime)
    def refresh(self):
        self.NetworkListScroll.refresh()
        #self.NetworkInfo.refresh()
        
        
