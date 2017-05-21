import sys
from PyQt4 import QtCore, QtGui
from random import randint
from datetime import datetime
from AnalogValueSetting import QAnalogValueSetting
import urllib2
import os
import glob


class QAnalogSettingTab(QtGui.QWidget):
    def __init__(self, analogValue, sizeValue = 1):
        QtGui.QWidget.__init__(self)

        # Section Mode
        # Create the Layout structure.

        self.AnalogSettingTabLayout = QtGui.QVBoxLayout(self)
        self.size = sizeValue

        # Create the Selection Mode Section
        if sizeValue > 0:
            self.AnalogSettingList = []
            self.AnalogSetting = QtGui.QTabWidget()
            for index in range(0,sizeValue):
                self.AnalogSettingList.append([QAnalogValueSetting(),analogValue,str(index+1)])
                self.AnalogSetting.addTab(self.AnalogSettingList[-1][0],self.AnalogSettingList[-1][-1])
            self.AnalogSettingTabLayout.addWidget(self.AnalogSetting)

        # Save all Layout.              
        self.setLayout(self.AnalogSettingTabLayout)
        
    def getValues(self):
        listReturn = []
        
        for item in self.AnalogSettingList:
            listReturn.append(item[0].getValues())
            
        return listReturn

    def disable(self):
        for item in self.AnalogSettingList:
            item[0].disable()

    def enable(self):
        for item in self.AnalogSettingList:
            item[0].enable()

    def buildSetting(self):
        stringBuild = ""
        if self.size > 1:
            for index in self.AnalogSettingList:
                stringBuild += str(index[0].buildSetting(str(index[1][::-1]+index[2])))
        else:
            stringBuild += str(self.AnalogSettingList[0][0].buildSetting(str(self.AnalogSettingList[0][1][::-1])))
        return stringBuild

