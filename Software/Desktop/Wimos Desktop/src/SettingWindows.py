import sys
from PyQt4 import QtCore, QtGui
from random import randint
from datetime import datetime
import urllib2
import os
import glob


class QSettingWindows(QtGui.QWidget):
    def __init__ (self):
        super(QSettingWindows, self).__init__()
        # Create the Global Layout.
        self.SettingLayout = QtGui.QVBoxLayout(self)

        #### Section Device Type ####
        # Create the Layout structure.
        self.DeviceLayout = QtGui.QHBoxLayout(self)
        self.DeviceLayoutLeft = QtGui.QVBoxLayout(self)
        self.DeviceLayoutCenter = QtGui.QVBoxLayout(self)
        self.DeviceLayoutRight = QtGui.QVBoxLayout(self)

        # Create the Selection Release Section
        
        self.ProyectNameLabel = QtGui.QLabel("Proyect Name:")
        self.ProyectNameTextEdit = QtGui.QTextEdit("Proyect Wimos")   
        self.ReleaseSectionLabel = QtGui.QLabel("Select a Release:")
        self.SelectRelease = QtGui.QComboBox()
        self.SelectRelease.addItem("--- Select Release Version ---")
        # Get File list.
        self.path = "workspace/releases/"
        fileList = glob.glob(self.path+"*.rar")              
        for file_ in fileList:
            self.SelectRelease.addItem(file_)
        # Save the Release widget.
        self.DeviceLayoutCenter.addWidget(self.ProyectNameLabel)
        self.DeviceLayoutCenter.addWidget(self.ProyectNameTextEdit) 
        self.DeviceLayoutCenter.addWidget(self.ReleaseSectionLabel) 
        self.DeviceLayoutCenter.addWidget(self.SelectRelease) 

        # Create the Selection of Device.        
        self.TypeDeviceLayout = QtGui.QHBoxLayout(self)
        self.DeviceSectionLabel = QtGui.QLabel("Select a Device:")

        self.CenterDevice = QtGui.QRadioButton("Center Device")
        self.CenterDevice.toggled.connect(self.selectCenter)
        self.CenterDevice.setChecked(True)
        
        self.HostDevice = QtGui.QRadioButton("Host Device")
        self.HostDevice.toggled.connect(self.selectHost)
        
        self.DeviceIDLabel = QtGui.QLabel("Device ID:")
        self.DeviceIDTextEdit = QtGui.QTextEdit("00")   

        # Save the Device Widget.
        self.TypeDeviceLayout.addWidget(self.CenterDevice)
        self.TypeDeviceLayout.addWidget(self.HostDevice)
        self.DeviceLayoutRight.addWidget(self.DeviceSectionLabel)
        self.DeviceLayoutRight.addLayout(self.TypeDeviceLayout)
        self.DeviceLayoutRight.addWidget(self.DeviceIDLabel) 
        self.DeviceLayoutRight.addWidget(self.DeviceIDTextEdit) 
    
        # Save all Layout.    

        self.DeviceLayout.addLayout(self.DeviceLayoutCenter)
        self.DeviceLayout.addLayout(self.DeviceLayoutLeft)
        self.DeviceLayout.addLayout(self.DeviceLayoutRight)
        self.DeviceLayout.setSpacing(10)
        #### END  Section Device Type ####
        
        #### Group Box Device ####
        # Create the Layout structure.
        self.SectionDevice = QtGui.QGroupBox()
        self.SectionDevice.setTitle("Select Device")
        self.SectionDevice.setLayout(self.DeviceLayout) 
        self.SectionDevice.setFixedHeight(135)
        self.SectionDevice.setFixedWidth(400)
        self.SettingLayout.addWidget(self.SectionDevice)  


        #### Section Host Type ####
        # Section HostMode
        # Create the Layout structure.
        self.HostModeLayout = QtGui.QHBoxLayout(self)
        self.HostModeLayoutRight = QtGui.QVBoxLayout(self)
        self.HostModeLayoutLeft = QtGui.QVBoxLayout(self)

        # Create the Selection HostMode Section
        self.HostModeSectionLabel = QtGui.QLabel("Select a HostMode:")
        self.SelectHostMode = QtGui.QComboBox()
        self.SelectHostMode.addItem("--- Select Mode ---")
        self.SelectHostMode.addItem("Normal Mode")
        self.SelectHostMode.addItem("Validation Mode")
        
        # Save the Release widget.
        self.HostModeLayoutLeft.addWidget(self.HostModeSectionLabel) 
        self.HostModeLayoutLeft.addWidget(self.SelectHostMode) 

        # Create the Selection of SubHostMode.
        self.SubHostModeSectionLabel = QtGui.QLabel("Select the SubMode:")        
        self.HostModel = QtGui.QStandardItemModel()

        #item = QtGui.QStandardItem("Debug")
        #item.setCheckable(True)
        #item.setEditable(False)
        #HostModel.appendRow(item)

        self.view = QtGui.QListView()
        self.view.setModel(self.HostModel)
        self.view.setFixedHeight(80)
        
        # Save the Device Widget.
        self.HostModeLayoutLeft.addWidget(self.SubHostModeSectionLabel)
        self.HostModeLayoutLeft.addWidget(self.view)

        
        # Create the Selection of SubHostMode.
        self.EnableLabel = QtGui.QLabel("Enable peripherals:")        
        self.HostModelEnable = QtGui.QStandardItemModel()
        self.peripheralList = ["Geolocation","Memory SD","Internal IMU",
                            "Battery Monitor","Port A1","Port A2",
                            "Port A3","Port A4","Port A5"]

        for peripheral in self.peripheralList:
            item = QtGui.QStandardItem(peripheral)
            item.setCheckable(True)
            item.setEditable(False)
            self.HostModelEnable.appendRow(item)

        self.ViewEnable = QtGui.QListView()
        self.ViewEnable.setModel(self.HostModelEnable)
        self.ViewEnable.setFixedHeight(80)
        
        # Save the Device Widget.
        self.HostModeLayoutLeft.addWidget(self.EnableLabel)
        self.HostModeLayoutLeft.addWidget(self.ViewEnable)
        
        # Save all Layout.    
        self.HostModeLayout.addLayout(self.HostModeLayoutLeft)
        self.HostModeLayout.addLayout(self.HostModeLayoutRight)
        
        
        #### Setting the Tabs Setting ####
        self.SettingPeripherals = QtGui.QTabWidget()
        self.SettingPeripherals.setTabPosition(QtGui.QTabWidget.South)
        
        self.SettingPeripherals.addTab(QtGui.QWidget(),"A1")
        self.SettingPeripherals.addTab(QtGui.QWidget(),"A2")
        self.SettingPeripherals.addTab(QtGui.QWidget(),"A3")
        self.SettingPeripherals.addTab(QtGui.QWidget(),"A4")
        self.SettingPeripherals.addTab(QtGui.QWidget(),"A5")
        self.SettingPeripherals.addTab(QtGui.QWidget(),"I2C")
        self.HostModeLayoutRight.addWidget(self.SettingPeripherals)

        
        #### Group Box Device ####
        self.SectionHostOptions = QtGui.QGroupBox()
        self.SectionHostOptions.setTitle("Setting Device")
        self.SectionHostOptions.setLayout(self.HostModeLayout)
        self.SettingLayout.addWidget(self.SectionHostOptions)
        
        self.SectionHostOptions.hide()
        
        #### [END] Section HostMode Type ####



        #### Section Center Type ####
        # Section CenterMode
        # Create the Layout structure.
        self.CenterModeLayout = QtGui.QHBoxLayout(self)
        self.CenterModeLayoutRight = QtGui.QVBoxLayout(self)
        self.CenterModeLayoutLeft = QtGui.QVBoxLayout(self)

        # Create the Selection CenterMode Section
        self.CenterModeSectionLabel = QtGui.QLabel("Select a Mode:")
        self.SelectCenterMode = QtGui.QComboBox()
        self.SelectCenterMode.addItem("--- Select Mode ---")
        self.SelectCenterMode.addItem("Normal Mode")
        self.SelectCenterMode.addItem("Validation Mode")
        # Save the Release widget.
        self.CenterModeLayoutLeft.addWidget(self.CenterModeSectionLabel) 
        self.CenterModeLayoutLeft.addWidget(self.SelectCenterMode) 

        # Create the Selection of SubCenterMode.
        self.SubCenterModeSectionLabel = QtGui.QLabel("Select the SubMode:")        
        self.CenterModel = QtGui.QStandardItemModel()

        #item = QtGui.QStandardItem("Debug")
        #item.setCheckable(True)
        #item.setEditable(False)
        #CenterModel.appendRow(item)

        self.view = QtGui.QListView()
        self.view.setModel(self.CenterModel)
        
        # Save the Device Widget.
        self.CenterModeLayoutLeft.addWidget(self.SubCenterModeSectionLabel)
        self.CenterModeLayoutLeft.addWidget(self.view)

        # Save all Layout.    
        self.CenterModeLayout.addLayout(self.CenterModeLayoutLeft)
        self.CenterModeLayout.addLayout(self.CenterModeLayoutRight)
        
        
        self.SubCenterMessageSectionLabel = QtGui.QLabel("Message List:")        
        self.MessageListModel = QtGui.QStandardItemModel()

        #item = QtGui.QStandardItem("Debug")
        #item.setCheckable(True)
        #item.setEditable(False)
        #CenterModel.appendRow(item)

        self.MessageView = QtGui.QListView()
        self.MessageView.setModel(self.MessageListModel)
        
        
        self.MessageLayout = QtGui.QHBoxLayout()

        self.addressLabel = QtGui.QLabel("Address Message:")
        self.addressMessage = QtGui.QTextEdit(str(00))
        self.addressMessage.setFixedHeight(23)
        self.addressMessage.setFixedWidth(50)
        
        self.commandLabel = QtGui.QLabel("Command Message:")
        self.commandMessage = QtGui.QTextEdit(str(00))
        self.commandMessage.setFixedHeight(23)
        self.commandMessage.setFixedWidth(50)

        self.addMessageButton = QtGui.QPushButton("Add Message")
        self.delMessageButton = QtGui.QPushButton("Delete Message")
        
        self.MessageLayout.addWidget(self.addressLabel)
        self.MessageLayout.addWidget(self.addressMessage)
        self.MessageLayout.addWidget(self.commandLabel)
        self.MessageLayout.addWidget(self.commandMessage)
        self.MessageLayout.addWidget(self.addMessageButton)
        self.MessageLayout.addWidget(self.delMessageButton)
        
        # Save the Device Widget.
        self.CenterModeLayoutRight.addWidget(self.SubCenterModeSectionLabel)
        self.CenterModeLayoutRight.addWidget(self.MessageView)
        self.CenterModeLayoutRight.addLayout(self.MessageLayout)

        
        #### Group Box Device ####
        self.SectionCenterOptions = QtGui.QGroupBox()
        self.SectionCenterOptions.setTitle("Setting Device")
        self.SectionCenterOptions.setLayout(self.CenterModeLayout)
        self.SettingLayout.addWidget(self.SectionCenterOptions)
        
        #self.SectionCenterOptions.hide()


        
        
        #### [END] Section CenterMode Type ####


        

        # Set the Global Layout as default layout.
        self.setLayout(self.SettingLayout)

    def selectCenter(self):
        self.SectionCenterOptions.show()
        self.SectionHostOptions.hide()

    def selectHost(self):
        self.SectionHostOptions.show()
        self.SectionCenterOptions.hide()

    def downloadRelease(self):
        print "HELLO"
       
        
