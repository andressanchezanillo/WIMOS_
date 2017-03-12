import sys
from PyQt4 import QtCore, QtGui
from NetworkList import QNetworkList
from random import randint
from datetime import datetime
import os

class QNetworkText(QtGui.QWidget):
    def __init__ (self, _logPath = "wimosLogs"):
        super(QNetworkText, self).__init__()
        self.NetworkLayout = QtGui.QVBoxLayout(self)
        self.NetworkOption = QtGui.QHBoxLayout(self)
        self.NetworkInfo = QtGui.QTextEdit()
        self.NetworkInfo.setFixedWidth(600)
        self.NetworkInfo.setReadOnly(True)
        self.NetworkAutoScroll = QtGui.QCheckBox("Autoscroll")
        self.NetworkLogger = QtGui.QCheckBox("Logger")
        
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
        
    def addInfo(self,
                _FrameID, _FrameName,
                _IdCenter, _IdHost,
                _SystemDate, _SystemTime,
                _GpsLatitude, _GpsLongitude,
                _Memory, _Battery, _Status,
                _CurrentDateTime):
        bodyText = "<b>Frame:</b> "+_FrameName+"("+_FrameID+") - "+_CurrentDateTime
        bodyText += "<ul>"
        bodyText += "<li><b>[Center] Station ID:</b> "+_IdCenter+"</li>"
        bodyText += "<li><b>[Host] Station ID:</b> "+_IdHost+"</li>"
        bodyText += "<ul>"
        bodyText += "<li><b>[Host] Date:</b> "+_SystemDate+"</li>"
        bodyText += "<li><b>[Host] Time:</b> "+_SystemTime+"</li>"
        bodyText += "<li><b>[Host] Latitude:</b> "+_GpsLatitude+"</li>"
        bodyText += "<li><b>[Host] Longitude:</b> "+_GpsLongitude+"</li>"
        bodyText += "<li><b>[Host] Memory Status:</b> "+_Memory+"</li>"
        bodyText += "<li><b>[Host] Battery Status:</b> "+_Battery+"</li>"
        bodyText += "<li><b>[Host] Mode Status:</b> "+_Status+"</li>"
        bodyText += "</ul>"		
        bodyText += "<li><b>[Host]<b> End </li>"
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
            
                    
    def addAlert(self,
                _FrameID, _FrameName,
                _IdCenter, _IdHost,
                _SystemDate, _SystemTime,
                _AlertA1, _AlertA2, _AlertA3,
                _AlertA4, _AlertA5,
                _CurrentDateTime):

        bodyText = "<b>[Frame] Frame ID:</b> "+_FrameName+"("+_FrameID+") - "+_CurrentDateTime+""
        bodyText += "<ul>"
        bodyText += "<li><b>[Center] Station ID:</b> "+_IdCenter+"</li>"
        bodyText += "<li><b>[Host] Station ID:</b> "+_IdHost+"</li>"
        bodyText += "<ul>"
        bodyText += "<li><b>[Host] Date:</b> "+_SystemDate+"</li>"
        bodyText += "<li><b>[Host] Time:</b> "+_SystemTime+"</li>"
        bodyText += "<ul>"
        bodyText += "<li><b>[Host-Alert] A1:</b> "+_AlertA1+"</li>"
        bodyText += "<li><b>[Host-Alert] A2:</b> "+_AlertA2+"</li>"
        bodyText += "<li><b>[Host-Alert] A3:</b> "+_AlertA3+"</li>"
        bodyText += "<li><b>[Host-Alert] A4:</b> "+_AlertA4+"</li>"
        bodyText += "<li><b>[Host-Alert] A5:</b> "+_AlertA5+"</li>"
        bodyText += "</ul>"	
        bodyText += "<li><b>[Host-Alert]</b> End </li>"
        bodyText += "</ul>"		
        bodyText += "<li><b>[Host]</b> End </li>"
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

    def disconnect(self, idDevice):
        bodyText = "<b>[Frame] Frame ERROR:</b> "
        bodyText += "<ul>"
        bodyText += "<li><b>[Host] Host ID: "+idDevice+" Disconnection.</b> "
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
        
