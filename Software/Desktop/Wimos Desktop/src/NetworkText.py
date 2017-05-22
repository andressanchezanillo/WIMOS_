import sys
from PyQt4 import QtCore, QtGui
from random import randint
from datetime import datetime
import os

class QNetworkText(QtGui.QWidget):
    def __init__ (self, _logPath = "wimosLogs"):
        super(QNetworkText, self).__init__()
        self.NetworkLayout = QtGui.QVBoxLayout(self)
        self.NetworkOption = QtGui.QHBoxLayout(self)

        self.FrameNumber = 1000
        
        self.NetworkInfo = QtGui.QTextEdit()
        self.NetworkInfo.setMinimumWidth(400)
        self.NetworkInfo.setReadOnly(True)
        self.NetworkInfo.setStatusTip("Information text display")
        
        self.NetworkAutoScroll = QtGui.QCheckBox("Autoscroll")
        self.NetworkAutoScroll.setStatusTip("Enable/Disable the autoscroll option")
        
        self.NetworkLogger = QtGui.QCheckBox("Logger")
        self.NetworkLogger.setStatusTip("Enable/Disable the logger into File .html")
        
        
        self.NetworkLayout.addWidget(self.NetworkInfo)
        self.NetworkOption.addWidget(self.NetworkAutoScroll)
        self.NetworkOption.addWidget(self.NetworkLogger)
        self.NetworkLayout.addLayout(self.NetworkOption)
        self.setLayout(self.NetworkLayout)

        self.path = _logPath
        if not os.path.exists(self.path):
            os.makedirs(self.path)

        self.log = os.path.join(self.path, datetime.now().strftime('%Y-%m-%d')+"_wimosDesktop.html")
        
    def logger(self, _info):
        if(self.NetworkLogger.isChecked()):
            if os.path.exists(self.log):
                file = open(self.log, 'a+')
            else:
                file = open(self.log, 'w+')
            file.write(_info)
            file.close()

    def addCenter(self,
                  _FrameName, _CenterID,
                  _CurrentDateTime,
                  _messageRatio, _messageTime):
        
        self.protectTextEditSize(self.FrameNumber)
        
        bodyText = "<br><b>Frame:</b> "+str(_FrameName)+" - "+_CurrentDateTime
        bodyText += "<ul>"
        bodyText += "<li><b>[Center] Station ID:</b> "+str(_CenterID)+"</li>"
        bodyText += "<ul>"
        bodyText += "<li><b>[Center] Data Ratio:</b> "+str(_messageRatio)+"% </li>"
        bodyText += "<li><b>[Center] Data Time:</b> "+str(_messageTime)+" ms </li>"
        bodyText += "</ul>"		
        bodyText += "<li><b>[Center]<b> End </li>"
        bodyText += "</ul>"
        bodyText += "<b>Frame</b> End <br>"

        
        if(self.NetworkAutoScroll.isChecked()):
            self.NetworkInfo.append(bodyText)
            self.NetworkInfo.verticalScrollBar()
            self.NetworkInfo.moveCursor(QtGui.QTextCursor.End)
        else:
            self.NetworkInfo.setTabChangesFocus(True)
            self.NetworkInfo.append(bodyText)

        self.logger(bodyText)
        
    
    def addInfo(self,
                _FrameID, _FrameName,
                _IdCenter, _IdHost,
                _SystemDate, _SystemTime,
                _GpsLatitude, _GpsLongitude,
                _Memory, _Battery, _Status,
                _CurrentDateTime):
        
        self.protectTextEditSize(self.FrameNumber)
        
        bodyText = "<br><b>Frame:</b> "+str(_FrameName)+"("+str(_FrameID)+") - "+_CurrentDateTime
        bodyText += "<ul>"
        bodyText += "<li><b>[Center] Station ID:</b> "+str(_IdCenter)+"</li>"
        bodyText += "<li><b>[Host] Station ID:</b> "+str(_IdHost)+"</li>"
        bodyText += "<ul>"
        bodyText += "<li><b>[Host] Date:</b> "+str(_SystemDate)+"</li>"
        bodyText += "<li><b>[Host] Time:</b> "+str(_SystemTime)+"</li>"
        bodyText += "<li><b>[Host] Latitude:</b> "+str(_GpsLatitude)+"</li>"
        bodyText += "<li><b>[Host] Longitude:</b> "+str(_GpsLongitude)+"</li>"
        bodyText += "<li><b>[Host] Memory Status:</b> "+str(_Memory)+"</li>"
        bodyText += "<li><b>[Host] Battery Status:</b> "+str(_Battery)+"</li>"
        bodyText += "<li><b>[Host] Mode Status:</b> "+str(_Status)+"</li>"
        bodyText += "</ul>"		
        bodyText += "<li><b>[Host]<b> End </li>"
        bodyText += "</ul>"
        bodyText += "<b>Frame</b> End <br>"

        
        if(self.NetworkAutoScroll.isChecked()):
            self.NetworkInfo.append(bodyText)
            self.NetworkInfo.verticalScrollBar()
            self.NetworkInfo.moveCursor(QtGui.QTextCursor.End)
        else:
            self.NetworkInfo.setTabChangesFocus(True)
            self.NetworkInfo.append(bodyText)

        self.logger(bodyText)
                    
                    
    def addAlert(self,
                _FrameID, _FrameName,
                _IdCenter, _IdHost,
                _SystemDate, _SystemTime,
                _AlertA1, _AlertA2, _AlertA3,
                _AlertA4, _AlertA5,
                _CurrentDateTime):


        self.protectTextEditSize(self.FrameNumber)
        
        bodyText = "<br><b>[Frame] Frame ID:</b> "+str(_FrameName)+"("+str(_FrameID)+") - "+_CurrentDateTime+""
        bodyText += "<ul>"
        bodyText += "<li><b>[Center] Station ID:</b> "+str(_IdCenter)+"</li>"
        bodyText += "<li><b>[Host] Station ID:</b> "+str(_IdHost)+"</li>"
        bodyText += "<ul>"
        bodyText += "<li><b>[Host] Date:</b> "+str(_SystemDate)+"</li>"
        bodyText += "<li><b>[Host] Time:</b> "+str(_SystemTime)+"</li>"
        bodyText += "<ul>"
        bodyText += "<li><b>[Host-Alert] A1:</b> "+str(_AlertA1)+"</li>"
        bodyText += "<li><b>[Host-Alert] A2:</b> "+str(_AlertA2)+"</li>"
        bodyText += "<li><b>[Host-Alert] A3:</b> "+str(_AlertA3)+"</li>"
        bodyText += "<li><b>[Host-Alert] A4:</b> "+str(_AlertA4)+"</li>"
        bodyText += "<li><b>[Host-Alert] A5:</b> "+str(_AlertA5)+"</li>"
        bodyText += "</ul>"	
        bodyText += "<li><b>[Host-Alert]</b> End </li>"
        bodyText += "</ul>"		
        bodyText += "<li><b>[Host]</b> End </li>"
        bodyText += "</ul>"
        bodyText += "<b>Frame</b> End <br>"

        
        if(self.NetworkAutoScroll.isChecked()):
            self.NetworkInfo.append(bodyText)
            self.NetworkInfo.verticalScrollBar()
            self.NetworkInfo.moveCursor(QtGui.QTextCursor.End)
        else:
            self.NetworkInfo.setTabChangesFocus(True)
            self.NetworkInfo.append(bodyText)
            
        self.logger(bodyText)

    def disconnect(self, idDevice):
        
        self.protectTextEditSize(self.FrameNumber)

        bodyText = "<br><b>[Frame] Frame ERROR:</b> "
        bodyText += "<ul>"
        bodyText += "<li><b>[Host] Host ID: "+str(idDevice)+" Disconnection.</b> "
        bodyText += "</ul>"
        bodyText += "<b>Frame</b> End <br><br>"

        
        if(self.NetworkAutoScroll.isChecked()):
            self.NetworkInfo.append(bodyText)
            self.NetworkInfo.verticalScrollBar()
            self.NetworkInfo.moveCursor(QtGui.QTextCursor.End)
        else:
            self.NetworkInfo.setTabChangesFocus(True)
            self.NetworkInfo.append(bodyText)
        
        self.logger(bodyText)        

    def protectTextEditSize(self, maxSize):
        CurrentBuffer = self.NetworkInfo.toPlainText().split("Frame End")
        if len(CurrentBuffer) >= maxSize:
            self.NetworkInfo.clear()
        

    def n8UT18(self):        
        testCount = 0
        testSuccess = 0

        self.NetworkInfo.setHtml("")
        self.NetworkLogger.setCheckState(QtCore.Qt.Unchecked)
        self.addCenter("INFO001:", 4, "2017-05-22 21:00:34", 100, 200 )
        
        testCount += 1
        if "Frame: INFO001: - 2017-05-22 21:00:34" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Center] Station ID: 4" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Center] Data Ratio: 100% " in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Center] Data Time: 200 ms" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Center] End" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "Frame End" in self.NetworkInfo.toPlainText():
            testSuccess += 1
                
        return (testSuccess/testCount)*100
        

    def n8UT19(self):        
        testCount = 0
        testSuccess = 0
        
        self.NetworkInfo.setHtml("")
        self.NetworkLogger.setCheckState(QtCore.Qt.Unchecked)
        self.addInfo(161,"Frame161:",0,18,"21-5-17","20:45:6","90.1346383873","-74.2140119036",50,0,"236b0a","2017-05-22 21:00:34")       
        
        testCount += 1
        if "Frame: Frame161:(161) - 2017-05-22 21:00:34" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Center] Station ID: 0" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] Station ID: 18" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] Date: 21-5-17" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] Time: 20:45:6" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] Latitude: 90.1346383873" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] Longitude: -74.2140119036" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] Memory Status: 50" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] Battery Status: 0" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] Mode Status: 236b0a" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] End" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "Frame End" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
                
        return (testSuccess/testCount)*100
        

    def n8UT20(self):        
        testCount = 0
        testSuccess = 0
        
        self.NetworkInfo.setHtml("")
        self.NetworkLogger.setCheckState(QtCore.Qt.Unchecked)
        self.addAlert(162,"Frame162:",0,16,"21-5-17","20:45:6",0,1,2,3,4,"2017-05-22 21:00:34")    
        
        testCount += 1
        if "[Frame] Frame ID: Frame162:(162) - 2017-05-22 21:00:34" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Center] Station ID: 0" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] Station ID: 16" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] Date: 21-5-17" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] Time: 20:45:6" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host-Alert] A1: 0" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host-Alert] A2: 1" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host-Alert] A3: 2" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host-Alert] A4: 3" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host-Alert] A5: 4" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host-Alert] End" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "[Host] End" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
        testCount += 1
        if "Frame End" in self.NetworkInfo.toPlainText():
            testSuccess += 1
        
                
        return (testSuccess/testCount)*100


    def TestUTs(self):

        resultn8UT18 = self.n8UT18()
        if ( resultn8UT18 == 100):
            print("[TEST] n8UT18\t[OK]\tn8.UT18 = 100%")
        else:
            print("[TEST] n8UT18\t[ERROR]\tn8.UT18 = "+(str(resultn8UT18))+"%")


        resultn8UT19 = self.n8UT19()
        if ( resultn8UT19 == 100):
            print("[TEST] n8UT19\t[OK]\tn8.UT19 = 100%")
        else:
            print("[TEST] n8UT19\t[ERROR]\tn8.UT19 = "+(str(resultn8UT19))+"%")


        resultn8UT20 = self.n8UT20()
        if ( resultn8UT20 == 100):
            print("[TEST] n8UT20\t[OK]\tn8.UT20 = 100%")
        else:
            print("[TEST] n8UT20\t[ERROR]\tn8.UT20 = "+(str(resultn8UT20))+"%")

        

    def TestVT(self):
        print("In progress..")
        
