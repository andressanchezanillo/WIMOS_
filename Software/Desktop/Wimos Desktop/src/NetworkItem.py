import sys
from PyQt4 import QtGui
from datetime import datetime
from PyQt4.QtCore import pyqtSignal
import time

class QNetworkItem (QtGui.QWidget):
    def __init__ (self, parent = None):
        super(QNetworkItem, self).__init__(parent)
        
        self.MainLayout = QtGui.QHBoxLayout()
        self.InfoLayout = QtGui.QVBoxLayout()   
        
        #Device Type Image
        self.DType = ''
        self.DeviceImage = QtGui.QLabel()
        self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Center_device.png"))
        self.MainLayout.addWidget(self.DeviceImage)

        #Device Battery State.
        self.DeviceBattery = QtGui.QLabel()
        self.DeviceBattery.setPixmap(QtGui.QPixmap('../images/battery25.ico').scaledToWidth(32))
        self.MainLayout.addWidget(self.DeviceBattery)
        self.MainLayout.addLayout(self.InfoLayout)
             
        #Device ID
        self.myID = 0
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
        self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_no.png').scaledToWidth(16))
        self.StatusLayout.addWidget(self.DeviceDatain)

        #Build the main widget
        self.setLayout(self.MainLayout)

        #Time variables and display utilities.
        self.LastUpdate = int(round(time.time() * 1000))
        self.LastUpdateAlert = int(round(time.time() * 1000))
        self.enable = True
        self.data = False

        self.setStatusTip(self.DType+": "+ str(self.myID))


    def addCenter(self, _CenterID,
                        _CurrentDateTime,
                        _messageRatio = None,
                        _messageTime = None,
                        _timeout = False):
        #set the internal ID
        self.myID = int(_CenterID)
        
        # ID device
        self.DeviceID.setText("<b>ID:</b> "+str(_CenterID))

        # Type device
        if _timeout:
            self.enable = False
            self.DType = 'Center'
            self.DeviceType.setText("<b>Type:</b> "+self.DType+" (Disconnected)")
            self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Center_device.png"))
            #self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/coverage_level.png').scaledToWidth(16))
        else:
            self.enable = True
            self.LastUpdate = int(round(time.time() * 1000))
            self.DType = 'Center'
            self.DeviceType.setText("<b>Type:</b> "+self.DType)
            self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Center_device.png"))
            #if _messageRatio is not None and _messageTime is not None:
                #self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/coverage_level.png').scaledToWidth(16))
        

        self.DeviceBattery.setPixmap(QtGui.QPixmap('../images/battery100.ico').scaledToWidth(32))
        self.LastPackageTimestap.setText("<b>Timestamp:</b> "+ _CurrentDateTime+"   ")
        
        self.setStatusTip(self.DType+": "+ str(self.myID))

            

        
    def addInfo(self, idDevice, typeDevice, batteryDevice, timeout = False):
        #set the internal ID
        self.myID = int(idDevice)
        
        # ID device
        self.DeviceID.setText("<b>ID:</b> "+str(idDevice))

        # Type device
        if(typeDevice is 'Center'):
            self.DType = 'Center'
            self.DeviceType.setText("<b>Type:</b> "+self.DType)
            self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Center_device.png"))
        elif(typeDevice is 'Host'):            
            self.DType = 'Host'
            self.DeviceType.setText("<b>Type:</b> "+self.DType)
            self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Host_device.png"))
        else:
            self.DType ='Uknown'
            self.DeviceType.setText("<b>Type:</b> "+self.DType)
            self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Uknown_device.png"))
                


        if(batteryDevice is '100'):
            self.DeviceBattery.setPixmap(QtGui.QPixmap('../images/battery100.ico').scaledToWidth(32))
        elif(batteryDevice is '75'):
            self.DeviceBattery.setPixmap(QtGui.QPixmap('../images/battery75.ico').scaledToWidth(32))
        elif(batteryDevice is '50'):
            self.DeviceBattery.setPixmap(QtGui.QPixmap('../images/battery50.ico').scaledToWidth(32))
        elif(batteryDevice is '25'):
            self.DeviceBattery.setPixmap(QtGui.QPixmap('../images/battery25.ico').scaledToWidth(32))

        self.LastPackageTimestap.setText("<b>Timestamp:</b> "+ datetime.now().strftime('%Y-%m-%d %H:%M:%S')+"   ")

        if not timeout:
            if not self.enable:
                self.enable = True
                if(typeDevice is 'Center'):
                    self.DeviceType.setText("<b>Type:</b> "+self.DType)
                    self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Center_device.png"))
            
                elif(typeDevice is 'Host'):
                    self.DeviceType.setText("<b>Type:</b> "+self.DType)
                    self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Host_device.png"))
                else:
                    self.DeviceType.setText("<b>Type:</b> "+self.DType)
                    self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Uknown_device.png"))
                
            self.LastUpdate = int(round(time.time() * 1000))

        
    def addAlert(self, idDevice, alertDevice, timeout = False):
        if(alertDevice is 'none'):
            self.data = False
            self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_no.png').scaledToWidth(16))
        elif(alertDevice is 'low'):
            self.data = True
            self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_green.png').scaledToWidth(16))
        elif(alertDevice is 'medium'):
            self.data = True
            self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_orange.png').scaledToWidth(16))
        elif(alertDevice is 'high'):
            self.data = True
            self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_red.png').scaledToWidth(16))
        else:
            self.data = False
            self.DeviceDatain.setPixmap(QtGui.QPixmap('../images/led_no.png').scaledToWidth(16))

        if not timeout:
            if not self.enable:
                self.enable = True
                if(self.DType is 'Center'): 
                    self.DeviceType.setText("<b>Type:</b> "+self.DType)
                    self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Center_device.png"))
            
                elif(self.DType is 'Host'):
                    self.DeviceType.setText("<b>Type:</b> "+self.DType)
                    self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Host_device.png"))
                else:
                    self.DeviceType.setText("<b>Type:</b> "+self.DType)
                    self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Uknown_device.png"))

            
            self.LastUpdate = int(round(time.time() * 1000))
            self.LastUpdateAlert = int(round(time.time() * 1000))
            self.setStatusTip(self.DType+": "+ str(self.myID))

    
    def disconnect(self):
        if(self.DType is 'Center'):
            self.DeviceType.setText("<b>Type:</b> "+self.DType+" <b>(Disconnected)</b>")
            self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Center_device_noconnection.png"))
        elif(self.DType is 'Host'):
            self.DeviceType.setText("<b>Type:</b> "+self.DType+" <b>(Disconnected)</b>")
            self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Host_device_noconnection.png"))
        else:
            self.DeviceType.setText("<b>Type:</b> "+self.DType+" <b>(Disconnected)</b>")
            self.DeviceImage.setPixmap(QtGui.QPixmap("../images/Uknown_device.png"))
        
        self.setStatusTip("[Disconnected]"+self.DType+": "+ str(self.myID))


    def getID(self):
        return self.myID


    def getLastUpdateTime(self):
        return self.LastUpdate


    def getStatus(self):
        return self.enable
        

    def n8UT12(self):        
        testCount = 0
        testSuccess = 0
        
        self.addCenter(1, "2017-05-22 21:00:34")
        
        testCount += 1
        if self.myID == 1:
            testSuccess += 1
            
        testCount += 1
        if self.DeviceID.text() == "<b>ID:</b> 1":
            testSuccess += 1
            
        testCount += 1
        if self.enable == True:
            testSuccess += 1
            
        testCount += 1
        if self.DType == "Center":
            testSuccess += 1
            
        testCount += 1
        if self.DeviceType.text() == "<b>Type:</b> Center":
            testSuccess += 1
            
        testCount += 1
        if "<b>Timestamp:</b> 2017-05-22 21:00:34" in self.LastPackageTimestap.text():
            testSuccess += 1    
                
        return (testSuccess/testCount)*100

        

    def n8UT13(self):        
        testCount = 0
        testSuccess = 0
        
        self.addCenter(1, "2017-05-22 21:00:34", 0, 0, True)
        
        testCount += 1
        if self.myID == 1:
            testSuccess += 1
            
        testCount += 1
        if self.DeviceID.text() == "<b>ID:</b> 1":
            testSuccess += 1
            
        testCount += 1
        if self.enable == False:
            testSuccess += 1
            
        testCount += 1
        if self.DType == "Center":
            testSuccess += 1
            
        testCount += 1
        if self.DeviceType.text() == "<b>Type:</b> Center (Disconnected)":
            testSuccess += 1
            
        testCount += 1
        if "<b>Timestamp:</b> 2017-05-22 21:00:34" in self.LastPackageTimestap.text():
            testSuccess += 1    
                
        return (testSuccess/testCount)*100
        

    def n8UT14(self):        
        testCount = 0
        testSuccess = 0
        
        self.addInfo(5, "Host", 0)
        
        testCount += 1
        if self.myID == 5:
            testSuccess += 1
            
        testCount += 1
        if self.DeviceID.text() == "<b>ID:</b> 5":
            testSuccess += 1
            
        testCount += 1
        if self.enable == True:
            testSuccess += 1
            
        testCount += 1
        if self.DType == "Host":
            testSuccess += 1
            
        testCount += 1
        if self.DeviceType.text() == "<b>Type:</b> Host":
            testSuccess += 1  
                
        return (testSuccess/testCount)*100

        

    def n8UT15(self):        
        testCount = 0
        testSuccess = 0
        
        self.DType is 'Center'
        self.disconnect()
        
        testCount += 1
        if "<b>Type:</b> "+self.DType+" <b>(Disconnected)</b>" in self.DeviceType.text():
            testSuccess += 1
                
        return (testSuccess/testCount)*100

        

    def n8UT16(self):        
        testCount = 0
        testSuccess = 0
        
        self.DType is 'Host'
        self.disconnect()
        
        testCount += 1
        if "<b>Type:</b> "+self.DType+" <b>(Disconnected)</b>" in self.DeviceType.text():
            testSuccess += 1
                
        return (testSuccess/testCount)*100
    

    

    

    def TestUTs(self):
        
        resultn8UT12 = self.n8UT12()
        if ( resultn8UT12 == 100):
            print("[TEST] n8UT12\t[OK]\tn8.UT12 = 100%")
        else:
            print("[TEST] n8UT12\t[ERROR]\tn8.UT12 = "+(str(resultn8UT12))+"%")

        resultn8UT13 = self.n8UT13()
        if ( resultn8UT13 == 100):
            print("[TEST] n8UT13\t[OK]\tn8.UT13 = 100%")
        else:
            print("[TEST] n8UT13\t[ERROR]\tn8.UT13 = "+(str(resultn8UT13))+"%")
        
        resultn8UT14 = self.n8UT14()
        if ( resultn8UT14 == 100):
            print("[TEST] n8UT14\t[OK]\tn8.UT14 = 100%")
        else:
            print("[TEST] n8UT14\t[ERROR]\tn8.UT14 = "+(str(resultn8UT14))+"%")

        resultn8UT15 = self.n8UT15()
        if ( resultn8UT15 == 100):
            print("[TEST] n8UT15\t[OK]\tn8.UT15 = 100%")
        else:
            print("[TEST] n8UT15\t[ERROR]\tn8.UT15 = "+(str(resultn8UT15))+"%")
            
        resultn8UT16 = self.n8UT16()
        if ( resultn8UT16 == 100):
            print("[TEST] n8UT16\t[OK]\tn8.UT16 = 100%")
        else:
            print("[TEST] n8UT16\t[ERROR]\tn8.UT16 = "+(str(resultn8UT16))+"%")

        

    def TestVT(self):
        print("In progress..")
    
