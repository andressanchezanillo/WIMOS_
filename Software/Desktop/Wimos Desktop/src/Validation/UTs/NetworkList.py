import sys
from PyQt4 import QtCore, QtGui
from NetworkItem import QNetworkItem
from NetworkGraph import QNetworkGraph
from random import randint
from datetime import datetime
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

    def n8UT06(self):        
        testCount = 0
        testSuccess = 0
        
        for index in reversed(range(self.myform.count())): 
            self.myform.itemAt(index).widget().setParent(None)
        
        del self.NetworkItemArray[:]
        self.addCenter("Frame161:", 0, datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        
        testCount += 1
        if len(self.NetworkItemArray) == 1:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 2:
            testSuccess += 1
            
        self.addCenter("Frame161:", 1, datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        
        testCount += 1
        if len(self.NetworkItemArray) == 2:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 4:
            testSuccess += 1
            
        self.addCenter("Frame161:", 0, datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        
        testCount += 1
        if len(self.NetworkItemArray) == 2:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 4:
            testSuccess += 1
        
                
        return (testSuccess/testCount)*100

    def n8UT07(self):        
        testCount = 0
        testSuccess = 0
        
        for index in reversed(range(self.myform.count())): 
            self.myform.itemAt(index).widget().setParent(None)
        
        del self.NetworkItemArray[:]
        self.addCenter("INFO001:", 0, datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        
        testCount += 1
        if len(self.NetworkItemArray) == 1:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 2:
            testSuccess += 1
            
        self.addCenter("INFO001:", 1, datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        
        testCount += 1
        if len(self.NetworkItemArray) == 2:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 4:
            testSuccess += 1
            
        self.addCenter("INFO001:", 1, datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        
        testCount += 1
        if len(self.NetworkItemArray) == 2:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 4:
            testSuccess += 1
        
                
        return (testSuccess/testCount)*100
        

    def n8UT08(self):        
        testCount = 0
        testSuccess = 0
        
        for index in reversed(range(self.myform.count())): 
            self.myform.itemAt(index).widget().setParent(None)
        
        del self.NetworkItemArray[:]
        self.addInfo(161,"Frame161:",0,16,"21-5-17","20:45:6","90.1346383873","-74.2140119036",50,0,"236b0a",datetime.now().strftime("%Y-%m-%d %H:%M:%S"))       

        testCount += 1
        if len(self.NetworkItemArray) == 2:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 4:
            testSuccess += 1
            
        self.addInfo(161,"Frame161:",0,18,"21-5-17","20:45:6","90.1346383873","-74.2140119036",50,0,"236b0a",datetime.now().strftime("%Y-%m-%d %H:%M:%S"))       

        testCount += 1
        if len(self.NetworkItemArray) == 3:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 6:
            testSuccess += 1
            
        self.addInfo(161,"Frame161:",0,18,"21-5-17","20:45:6","90.1346383873","-74.2140119036",50,0,"236b0a",datetime.now().strftime("%Y-%m-%d %H:%M:%S"))       

        testCount += 1
        if len(self.NetworkItemArray) == 3:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 6:
            testSuccess += 1
        
                
        return (testSuccess/testCount)*100
        

    def n8UT09(self):        
        testCount = 0
        testSuccess = 0
        
        for index in reversed(range(self.myform.count())): 
            self.myform.itemAt(index).widget().setParent(None)
        
        del self.NetworkItemArray[:]
        self.addInfo(161,"Frame161:",0,18,"21-5-17","20:45:6","90.1346383873","-74.2140119036",50,0,"236b0a",datetime.now().strftime("%Y-%m-%d %H:%M:%S"))       

        testCount += 1
        if len(self.NetworkItemArray) == 2:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 4:
            testSuccess += 1
            
        self.addAlert(162,"Frame162:",0,18,"21-5-17","20:45:6",0,1,2,3,4,datetime.now().strftime("%Y-%m-%d %H:%M:%S"))       

        testCount += 1
        if len(self.NetworkItemArray) == 2:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 4:
            testSuccess += 1
            
        self.addAlert(162,"Frame162:",0,16,"21-5-17","20:45:6",0,1,2,3,4,datetime.now().strftime("%Y-%m-%d %H:%M:%S"))    

        testCount += 1
        if len(self.NetworkItemArray) == 2:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 4:
            testSuccess += 1
        
                
        return (testSuccess/testCount)*100
        

    def n8UT10(self):        
        testCount = 0
        testSuccess = 0
        
        for index in reversed(range(self.myform.count())): 
            self.myform.itemAt(index).widget().setParent(None)
        
        del self.NetworkItemArray[:]
        self.addCenter("INFO001:", 1, datetime.now().strftime("%Y-%m-%d %H:%M:%S"))      
        self.refresh()
        
        testCount += 1
        if len(self.NetworkItemArray) == 1:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 2:
            testSuccess += 1
            
        testCount += 1
        if self.NetworkItemArray[-1].getStatus() == True:
            testSuccess += 1

        time.sleep(6)
        
        self.refresh()
        
        testCount += 1
        if len(self.NetworkItemArray) == 1:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 2:
            testSuccess += 1
            
        testCount += 1
        if self.NetworkItemArray[-1].getStatus() == False:
            testSuccess += 1

                
        return (testSuccess/testCount)*100
        

    def n8UT11(self):        
        testCount = 0
        testSuccess = 0
        
        for index in reversed(range(self.myform.count())): 
            self.myform.itemAt(index).widget().setParent(None)
        
        del self.NetworkItemArray[:]
        self.addInfo(161,"Frame161:",0,18,"21-5-17","20:45:6","90.1346383873","-74.2140119036",50,0,"236b0a",datetime.now().strftime("%Y-%m-%d %H:%M:%S"))       
        self.addAlert(162,"Frame162:",0,18,"21-5-17","20:45:6",0,1,2,3,4,datetime.now().strftime("%Y-%m-%d %H:%M:%S"))       

        self.refresh()
        
        testCount += 1
        if len(self.NetworkItemArray) == 2:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 4:
            testSuccess += 1
            
        testCount += 1
        if self.NetworkItemArray[-1].data == True:
            testSuccess += 1

        time.sleep(1)
        
        self.refresh()
        
        testCount += 1
        if len(self.NetworkItemArray) == 2:
            testSuccess += 1
            
        testCount += 1
        if self.myform.count() == 4:
            testSuccess += 1
            
        testCount += 1
        if self.NetworkItemArray[-1].data == False:
            testSuccess += 1

                
        return (testSuccess/testCount)*100

    

    def TestUTs(self):

        resultn8UT06 = self.n8UT06()
        if ( resultn8UT06 == 100):
            print("[TEST] n8UT06\t[OK]\tn8.UT06 = 100%")
        else:
            print("[TEST] n8UT06\t[ERROR]\tn8.UT06 = "+(str(resultn8UT06))+"%")
        
        resultn8UT07 = self.n8UT07()
        if ( resultn8UT07 == 100):
            print("[TEST] n8UT07\t[OK]\tn8.UT07 = 100%")
        else:
            print("[TEST] n8UT07\t[ERROR]\tn8.UT07 = "+(str(resultn8UT07))+"%")
        
        
        resultn8UT08 = self.n8UT08()
        if ( resultn8UT08 == 100):
            print("[TEST] n8UT08\t[OK]\tn8.UT08 = 100%")
        else:
            print("[TEST] n8UT08\t[ERROR]\tn8.UT08 = "+(str(resultn8UT08))+"%")
        
        
        resultn8UT09 = self.n8UT09()
        if ( resultn8UT09 == 100):
            print("[TEST] n8UT09\t[OK]\tn8.UT09 = 100%")
        else:
            print("[TEST] n8UT09\t[ERROR]\tn8.UT09 = "+(str(resultn8UT09))+"%")
        
        
        resultn8UT10 = self.n8UT10()
        if ( resultn8UT10 == 100):
            print("[TEST] n8UT10\t[OK]\tn8.UT10 = 100%")
        else:
            print("[TEST] n8UT10\t[ERROR]\tn8.UT10 = "+(str(resultn8UT10))+"%")
        
        resultn8UT11 = self.n8UT11()
        if ( resultn8UT11 == 100):
            print("[TEST] n8UT11\t[OK]\tn8.UT11 = 100%")
        else:
            print("[TEST] n8UT11\t[ERROR]\tn8.UT11 = "+(str(resultn8UT11))+"%")

        for index in reversed(range(self.myform.count())): 
            self.myform.itemAt(index).widget().setParent(None)
        
        del self.NetworkItemArray[:]
        self.addCenter("INFO001:", 1, datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

        self.NetworkItemArray[-1].TestUTs()
        
        
        self.NetworkGraph.TestUTs()
        

    def TestVT(self):
        print("In progress..")
        NewtorkItem = QNetworkItem()
        
        NewtorkItem.TestVT()
        self.NetworkGraph.TestVT()


