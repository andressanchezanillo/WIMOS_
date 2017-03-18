import sys
from PyQt4 import QtCore, QtGui
from NetworkItem import QNetworkItem
from NetworkGraph import QNetworkGraph
from random import randint
import time, threading

class QNetworkList (QtGui.QWidget):

    def __init__ (self, parent = None):
        super(QNetworkList, self).__init__(parent)
        QtGui.QWidget.__init__(self)

        # List object.
        self.mygroupbox = QtGui.QGroupBox(self)
        self.myform = QtGui.QFormLayout(self)  
        self.mygroupbox.setLayout(self.myform)
        self.scroll = QtGui.QScrollArea(self)
        self.scroll.setWidget(self.mygroupbox)
        self.scroll.setWidgetResizable(True)
        self.scroll.setFixedWidth(355)
        self.scroll.setMinimumHeight(440)

        # Add a network Graph.
        self.NetworkGraph = QNetworkGraph()        
        
        # Layout Scroll.
        self.layout = QtGui.QHBoxLayout(self)
        self.layout.addWidget(self.scroll)
        self.layout.addWidget(self.NetworkGraph)

        # Network item set.
        self.NetworkItemArray = []
        self.timeoutDataIn = 300
        self.timeoutConnection = 5000

        # Settings for threshold.
        self.startThreshold = 0
        self.noThreshold = 0
        self.lowThreshold = 4
        self.mediumThreshold = 8
        self.highThreshold = 10
        self.endThreshold = 10

        # Timer refresh thread
        self.TimerRefresh = QtCore.QTimer(self)
        self.TimerRefresh.setInterval(250)
        self.TimerRefresh.setSingleShot(False)
        self.TimerRefresh.timeout.connect(self.refresh)
        self.TimerRefresh.start(250)

    def addCenter(self, _FrameName, _CenterID,
                          _CurrentDateTime,
                          _messageRatio = None,
                          _messageTime = None):

        varFound = False
        # Update the item list.
        for item in self.NetworkItemArray:
            if(item.getID() == int(_CenterID)):
                item.addCenter(   _CenterID,
                                  _CurrentDateTime,
                                  _messageRatio,
                                  _messageTime)
                varFound = True

        #Add the new item.
        if not varFound:

            # Add new Line.
            line = QtGui.QFrame();
            line.setFrameShape(QtGui.QFrame.HLine)
            line.setFrameShadow(QtGui.QFrame.Sunken)

            # Enter the new Item.
            self.NetworkItemArray.append(QNetworkItem())            
            self.NetworkItemArray[-1].addCenter(  _CenterID,
                                                  _CurrentDateTime,
                                                  _messageRatio,
                                                  _messageTime)
            
            # Add the Item and the Separator line.
            self.myform.addRow(self.NetworkItemArray[-1])
            self.myform.addRow(line)
        
        #Send the information to Network Graph.
        if _messageRatio is not None and _messageTime is not None:
            self.NetworkGraph.addCenter(_FrameName, _CenterID,
                                        _CurrentDateTime,
                                        _messageRatio, _messageTime )

        
        
    def addInfo(self,
                _FrameID, _FrameName,
                _IdCenter, _IdHost,
                _SystemDate, _SystemTime,
                _GpsLatitude, _GpsLongitude,
                _Memory, _Battery, _Status,
                _CurrentDateTime):
        
        self.addCenter(_FrameName, _IdCenter, _CurrentDateTime)

        varFound = False
        # Update the item list.
        for item in self.NetworkItemArray:
            if(item.getID() == int(_IdHost)):
                item.addInfo(_IdHost,'Host',_Battery)
                varFound = True

        #Add the new item.
        if not varFound:

            # Add new Line.
            line = QtGui.QFrame();
            line.setFrameShape(QtGui.QFrame.HLine)
            line.setFrameShadow(QtGui.QFrame.Sunken)

            # Enter the new Item.
            self.NetworkItemArray.append(QNetworkItem())            
            self.NetworkItemArray[-1].addInfo(_IdHost,'Host',_Battery)
            
            # Add the Item and the Separator line.
            self.myform.addRow(self.NetworkItemArray[-1])
            self.myform.addRow(line)

        #Send the information to Network Graph.
        self.NetworkGraph.addInfo(_FrameID, _FrameName,
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
        
        self.addCenter(_FrameName, _IdCenter, _CurrentDateTime)

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
        
        # Add the alert to the Device list if it exists.
        for item in self.NetworkItemArray:
            if(item.getID() == int(_IdHost)):
                item.addAlert(_IdHost,AlertLevel)

        # Send the alert to Network Graph.
        self.NetworkGraph.addAlert(_FrameID, _FrameName,
                                    _IdCenter, _IdHost,
                                    _SystemDate, _SystemTime,
                                    _AlertA1, _AlertA2, _AlertA3,
                                    _AlertA4, _AlertA5, AlertLevel,
                                    _CurrentDateTime)
            
            
    def refresh(self):
        # Check each item.
        for item in self.NetworkItemArray:
            
            # If the timeout of datain is expired.
            if (int(round(time.time() * 1000)) - item.getLastUpdateTime()) > self.timeoutDataIn:
                item.addAlert(item.getID(), 'none', True)
            
            # If the timeout of connection is expired.    
            if (int(round(time.time() * 1000)) - item.getLastUpdateTime()) > self.timeoutConnection:
                if item.getStatus():
                    item.enable = False
                    item.disconnect()
                    self.NetworkGraph.disconnect(item.getID())


