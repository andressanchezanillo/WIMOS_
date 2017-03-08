import sys
from PyQt4 import QtGui
from datetime import datetime

class QNetworkItem (QtGui.QWidget):
    def __init__ (self, parent = None):
        super(QCustomQWidget, self).__init__(parent)
        
        self.MainLayout = QtGui.QHBoxLayout()
        self.InfoLayout = QtGui.QVBoxLayout()   
        
        #Device Type Image
        self.DeviceImage = QtGui.QLabel()
        self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Center_device.png"))
        self.MainLayout.addWidget(self.DeviceImage)

        #Device Battery State.
        self.DeviceBattery = QtGui.QLabel()
        self.DeviceBattery.setPixmap(QtGui.QPixmap('../images/battery100.ico').scaledToWidth(32))
        self.MainLayout.addWidget(self.DeviceBattery)
        self.MainLayout.addLayout(self.InfoLayout)
             
        #Device ID
        self.DeviceID = QtGui.QLabel()
        self.DeviceID.setText("<b>ID:</b> 0x01")
        self.InfoLayout.addWidget(self.DeviceID)

        self.DeviceType = QtGui.QLabel()
        self.DeviceType.setText("<b>Type:</b> Center")
        self.InfoLayout.addWidget(self.DeviceType)

        #Status Layout              
        self.StatusLayout = QtGui.QVBoxLayout()
        self.MainLayout.addLayout(self.StatusLayout)
        
        
        #Device Last Package timestamp.
        self.LastPackageTimestap = QtGui.QLabel()
        self.LastPackageTimestap.setText("<b>Timestamp:</b> 2017-03-08 22:13:12   ")
        self.InfoLayout.addWidget(self.LastPackageTimestap)
    
        #Device Image DataIn.
        self.DeviceDatain = QtGui.QLabel()
        self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_red.png').scaledToWidth(16))
        self.StatusLayout.addWidget(self.DeviceDatain)

        #Build the main widget
        self.setLayout(self.MainLayout)

    def addInfo(self, idDevice, typeDevice, batteryDevice, alertDevice):
        # ID device
        self.DeviceID.setText("<b>ID:</b> "+idDevice)

        # Type device
        if(typeDevice is 'Center'):
            self.DeviceType.setText("<b>Type:</b> "+typeDevice)
            self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Center_device.png"))
        elif(typeDevice is 'Host'):
            self.DeviceType.setText("<b>Type:</b> "+typeDevice)
            self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Host_device.png"))

        if(batteryDevice is '100'):
            self.DeviceBattery.setPixmap(QtGui.QPixmap('../images/battery100.ico').scaledToWidth(32))
        elif(batteryDevice is '75'):
            self.DeviceBattery.setPixmap(QtGui.QPixmap('../images/battery75.ico').scaledToWidth(32))
        elif(batteryDevice is '50'):
            self.DeviceBattery.setPixmap(QtGui.QPixmap('../images/battery50.ico').scaledToWidth(32))
        elif(batteryDevice is '25'):
            self.DeviceBattery.setPixmap(QtGui.QPixmap('../images/battery25.ico').scaledToWidth(32))

        if(alertDevice is 'none'):
            self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_no.png').scaledToWidth(16))
        elif(alertDevice is 'low'):
            self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_green.png').scaledToWidth(16))
        elif(alertDevice is 'medium'):
            self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_orange.png').scaledToWidth(16))
        elif(alertDevice is 'high'):
            self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_red.png').scaledToWidth(16))
        else:
            self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_no.png').scaledToWidth(16))
        

        self.LastPackageTimestap.setText("<b>Timestamp:</b> 07/03/2017 - 22:13:12.4   ")
        
        self.LastPackageTimestap.setText("<b>Timestamp:</b> "+ datetime.now().strftime('%Y-%m-%d %H:%M:%S')+"   ")
        

