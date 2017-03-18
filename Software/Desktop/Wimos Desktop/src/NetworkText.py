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
        
        
