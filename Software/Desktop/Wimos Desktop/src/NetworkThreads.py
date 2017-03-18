from random import randint
from datetime import datetime
from PyQt4 import QtCore, QtGui
from NetworkList import QNetworkList
import threading, Queue
import serial
import sys
import os
import re
import threading
import time


class QNetworkThreads(QtGui.QWidget):

    signalInfo = QtCore.pyqtSignal(int, str, int,
                                        int, str, str,
                                        str, str, int,
                                        int, int, str)

    signalAlert = QtCore.pyqtSignal(int, str, int,
                                        int, str, str,
                                        int, int, int,
                                        int, int, str)

    signalCenter = QtCore.pyqtSignal(str, int, str, int, int )
    
    def __init__(self):
        super(QNetworkThreads, self).__init__()        
        #General Windows        
        self.NetworkLayout = QtGui.QVBoxLayout(self)
        self.NetworkListScroll = QNetworkList(self)
        
        self.NetworkLayout.addWidget(self.NetworkListScroll)
        self.setLayout(self.NetworkLayout)
        
        # Gui Signals and Slot
        self.signalInfo.connect(self.NetworkListScroll.addInfo)        
        
        self.signalAlert.connect(self.NetworkListScroll.addAlert)        
        
        self.signalCenter.connect(self.NetworkListScroll.addCenter)
        
        # Sharing structure
        self.SerialInput = Queue.Queue()
        self.ProcessDataInput = Queue.Queue()

        # Serial Thread.
        #t1.isAlive()
        self.SerialPort = 'COM6'
        self.SerialThreadRunning = True
        
        self.SerialThread = threading.Thread(target=self.runSerial, args=())
        self.SerialThread.daemon = True
        self.SerialThread.start()

        # Process Thread.
        self.ProcessThread = threading.Thread(target=self.runProcess, args=())
        self.ProcessThread.daemon = True
        self.ProcessThread.start()

        # Refresh Thread.
        self.RefreshThread = threading.Thread(target=self.runRefresh, args=())
        self.RefreshThread.daemon = True
        self.RefreshThread.start()
        
    def __exit__(self, exc_type, exc_value, traceback):
        self.SerialThreadRunning = False

    def runSerial(self):
        #Daemon for Serial Management.
        while True:
            # Open Serial
            SerialPort = serial.Serial(port= self.SerialPort,\
                                        baudrate=115200,\
                                        parity=serial.PARITY_NONE,\
                                        stopbits=serial.STOPBITS_ONE,\
                                        bytesize=serial.EIGHTBITS,\
                                        timeout=1)
            # Clear Serial Buffer
            SerialBuffer = ""
            print "["+datetime.now().strftime("%Y-%m-%d %H:%M:%S")+"] connected to: " + SerialPort.portstr
            
            # While Serial is Open and Connected.
            while SerialPort.isOpen() and self.SerialThreadRunning:

                # If buffer Serial is greater than 300    
                if(self.SerialInput.qsize() > 300):
                    # Print warning.
                    print "["+datetime.now().strftime("%Y-%m-%d %H:%M:%S")+"] Oversize Serial Thread." + str(self.SerialInput.qsize())

                # Bytes available to be read
                bytesToRead = SerialPort.inWaiting()
                # Bytes available
                if bytesToRead > 0:

                    # Concatenate the Bytes available with the Serial Buffer.
                    SerialBuffer += SerialPort.read(bytesToRead)
                    # Split each line.
                    SerialSplitted = SerialBuffer.split("\r\n")
                    # if there are more than 2 lines.
                    if len(SerialSplitted) >= 2:
                        # For each line
                        for line in SerialSplitted:
                            # Input into share structure.
                            self.SerialInput.put([datetime.now(),line])
                    # Get the remainer buffer.
                    SerialBuffer = SerialSplitted[-1]
            # Close Serial.
            SerialPort.close()
                
                
    def processHeader(self, datainput):
        frameID = int(re.search(r'\d+', datainput[0]).group())
        frameName = datainput[0]

        idCenter = int(datainput[4], 16)
        idHost = int(datainput[3], 16)

        return [frameID, frameName, idCenter, idHost]
                
                
    def processDateTime(self, datainput):
        Date = str(int(datainput[5], 16))+"-"+str(int(datainput[6], 16))+"-"+str(int(datainput[7], 16))
        Time = str(int(datainput[10], 16))+":"+str(int(datainput[9], 16))+":"+str(int(datainput[8], 16))

        return [Date, Time]
                
                
    def processAlertValues(self, datainput):
        Alert1 = int(datainput[15], 16)
        Alert2 = int(datainput[16], 16)
        Alert3 = int(datainput[17], 16)
        Alert4 = int(datainput[18], 16)
        Alert5 = int(datainput[19], 16)
        
        return Alert1, Alert2, Alert3, Alert4, Alert5
                
                
    def processGPSValues(self, datainput):
        # Calculate the latitude.
        LatitudeDegree = int(datainput[11], 16)
        LatitudeMinutes = int(datainput[12]+datainput[13]+datainput[14]+datainput[15], 16)

        if LatitudeMinutes > 2147483647:
            LatitudeMinutes -= 4294967294
            LatitudeDegree = LatitudeDegree * -1
        Latitude =  str(LatitudeDegree)+"."+str(LatitudeMinutes)

        # Calculate the longitude.
        LongitudeDegree = int(datainput[16], 16)
        LongitudeMinutes = int(datainput[17]+datainput[18]+datainput[19]+datainput[20], 16)
        
        if LongitudeMinutes > 2147483647:
            LongitudeMinutes -= 4294967294
            LongitudeDegree = LongitudeDegree * -1
        Longitude =  str(LongitudeDegree)+"."+str(LongitudeMinutes)

        
        return Latitude, Longitude

    def processStatus(self, datainput):
        
        memory =  int(datainput[21], 16)
        battery =  int(datainput[22], 16)
        status =  datainput[24]+datainput[25]+datainput[26]
        
        
        return memory, battery, status
    

    def processCenterInfo(self, datainput):
    
        frameName = datainput[0]    
        #idCenter = datainput[1]        
        #messageRatio = int(datainput[2], 16)
        #messageTime = int(datainput[3], 16)
        
        idCenter = 0       
        messageRatio = int(datainput[1], 16)
        messageTime = int(datainput[2], 16)
        
        return frameName, idCenter, messageRatio, messageTime
    
    def runProcess(self):
        # Processor Data Daemon.
        while True:
            # If there is some data to be processed.
            if self.SerialInput.qsize() > 0:
                # Get the datetime and Data string.
                DateTime,Data = self.SerialInput.get()
                # If the data is not null
                if(len(Data) > 0):
                    # Split each space
                    DataSplitted = Data.split(' ')
                    # If the data is valid.
                    if(DataSplitted[-1] == 'ACK-OK' or DataSplitted[-1] == 'OK'):
                        # If the frame is a Alert message.
                        if(DataSplitted[0] == 'FRAME162:'):
                            # Process message header.
                            [_FrameID, _FrameName, _IdCenter, _IdHost] = self.processHeader(DataSplitted)
                            # Process TimeStamp.
                            [_SystemDate, _SystemTime] = self.processDateTime(DataSplitted)
                            # Process the Alert.
                            [_AlertA1, _AlertA2, _AlertA3, _AlertA4, _AlertA5] = self.processAlertValues(DataSplitted)

                            # Input the new Data for updating.
                            self.ProcessDataInput.put([_FrameName,[ _FrameID, _FrameName,
                                                                    _IdCenter, _IdHost,
                                                                    _SystemDate, _SystemTime,
                                                                    _AlertA1, _AlertA2, _AlertA3,
                                                                    _AlertA4, _AlertA5,
                                                                    DateTime.strftime("%Y-%m-%d %H:%M:%S")]])
                        # If the frame is Info message.
                        elif(DataSplitted[0] == 'FRAME161:'):
                            # Process message header.
                            [_FrameID, _FrameName, _IdCenter, _IdHost] = self.processHeader(DataSplitted)
                            # Process TimeStamp.
                            [_SystemDate, _SystemTime] = self.processDateTime(DataSplitted)
                            # Process the GPS data.
                            [_GpsLatitude, _GpsLongitude] = self.processGPSValues(DataSplitted)
                            # Process the Status.
                            [_Memory, _Battery, _Status] = self.processStatus(DataSplitted)

                            # Input the new Data for updating.
                            self.ProcessDataInput.put([_FrameName,[  _FrameID, _FrameName,
                                                                     _IdCenter, _IdHost,
                                                                     _SystemDate, _SystemTime,
                                                                     _GpsLatitude, _GpsLongitude,
                                                                     _Memory, _Battery, _Status,
                                                                     DateTime.strftime("%Y-%m-%d %H:%M:%S")]])
                        # If the frame is a Center information. 
                        elif(DataSplitted[0] == '[INFO001]:'):
                            # Process Center Info.                            
                            [_FrameName, _CenterID, _MessageRatio, _MessageTime] = self.processCenterInfo(DataSplitted)

                            # Input the new Data for updating.
                            self.ProcessDataInput.put([_FrameName,[  _FrameName, _CenterID,
                                                                     _MessageRatio, _MessageTime,
                                                                     DateTime.strftime("%Y-%m-%d %H:%M:%S")]])
                            
                    else:
                        print "["+datetime.now().strftime("%Y-%m-%d %H:%M:%S")+"] ACK Error found"
                        
    def runRefresh(self):
        """ Method that runs forever """
        while True:
            if self.ProcessDataInput.qsize() > 0:
                Data = self.ProcessDataInput.get()
                # If it is a Alert message.
                if (Data[0] == 'FRAME162:'):
                    # Emit the signal Alert.
                    self.signalAlert.emit(Data[-1][0],Data[-1][1],
                                          Data[-1][2],Data[-1][3],
                                          Data[-1][4],Data[-1][5],
                                          Data[-1][6],Data[-1][7],
                                          Data[-1][8],Data[-1][9],
                                          Data[-1][10],Data[-1][11])
                
                # If it is a Info message.
                elif (Data[0] == 'FRAME161:'):
                    # Emit the signal Info.
                    self.signalInfo.emit(Data[-1][0],Data[-1][1],
                                                    Data[-1][2],Data[-1][3],
                                                    Data[-1][4],Data[-1][5],
                                                    Data[-1][6],Data[-1][7],
                                                    Data[-1][8],Data[-1][9],
                                                    Data[-1][10],Data[-1][11])
                
                # If it is a Center Info.
                elif (Data[0] == '[INFO001]:'):
                    self.signalCenter.emit(Data[-1][0],Data[-1][1],
                                           Data[-1][4],Data[-1][2],
                                           Data[-1][3])
