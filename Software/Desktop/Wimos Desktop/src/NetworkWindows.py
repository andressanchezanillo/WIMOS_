import sys
from PyQt4 import QtCore, QtGui
from NetworkList import QNetworkList
from random import randint
from datetime import datetime
import serial


class QNetworkWindows(QtGui.QWidget):
    def __init__ (self):
        super(QNetworkWindows, self).__init__()
        
        self.NetworkLayout = QtGui.QHBoxLayout(self)
        self.NetworkListScroll = QNetworkList(self)
        
        self.NetworkLayout.addWidget(self.NetworkListScroll)
        self.setLayout(self.NetworkLayout)

        self.Serial = {}

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

        self.NetworkListScroll.addInfo(_FrameID, _FrameName,
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
        

        self.NetworkListScroll.addAlert(_FrameID, _FrameName,
                                        _IdCenter, _IdHost,
                                        _SystemDate, _SystemTime,
                                        _AlertA1, _AlertA2, _AlertA3,
                                        _AlertA4, _AlertA5,
                                        _CurrentDateTime)
        
        
