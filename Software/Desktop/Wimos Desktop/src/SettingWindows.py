import sys
from PyQt4 import QtCore, QtGui
from random import randint
from datetime import datetime
from AnalogSetting import QAnalogSetting
from CommandI2CList import QCommandI2CList
import urllib2
import os
import re
import glob
import zipfile
import shutil


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
        self.ProyectNameTextEdit.setFixedWidth(250)
        self.ProyectNameTextEdit.setFixedHeight(23)
        self.ReleaseSectionLabel = QtGui.QLabel("Select a Release:")
        self.SelectRelease = QtGui.QComboBox()
        self.SelectRelease.setFixedWidth(250)
        self.SelectRelease.addItem("--- Select Release Version ---")
        # Get File list.
        self.updateReleaseList()
        # Save the Release widget.
        self.DeviceLayoutLeft.addWidget(self.ProyectNameLabel)
        self.DeviceLayoutLeft.addWidget(self.ProyectNameTextEdit) 
        self.DeviceLayoutLeft.addWidget(self.ReleaseSectionLabel) 
        self.DeviceLayoutLeft.addWidget(self.SelectRelease) 

        # Create the Selection of Device.        
        self.TypeDeviceLayout = QtGui.QHBoxLayout(self)
        self.DeviceSectionLabel = QtGui.QLabel("Select a Device:")

        self.CenterDevice = QtGui.QRadioButton("Center Device")
        self.CenterDevice.setChecked(True)
        self.CenterDevice.toggled.connect(self.selectCenter)
        
        self.HostDevice = QtGui.QRadioButton("Host Device")
        self.HostDevice.toggled.connect(self.selectHost)
        
        self.DeviceIDLabel = QtGui.QLabel("Device ID:")
        self.DeviceIDTextEdit = QtGui.QTextEdit("0x00")
        self.DeviceIDTextEdit.setFixedWidth(250) 
        self.DeviceIDTextEdit.setFixedHeight(23)

        # Save the Device Widget.
        self.TypeDeviceLayout.addWidget(self.CenterDevice)
        self.TypeDeviceLayout.addWidget(self.HostDevice)
        self.DeviceLayoutCenter.addWidget(self.DeviceSectionLabel)
        self.DeviceLayoutCenter.addLayout(self.TypeDeviceLayout)
        self.DeviceLayoutCenter.addWidget(self.DeviceIDLabel) 
        self.DeviceLayoutCenter.addWidget(self.DeviceIDTextEdit) 
    

        # Create the Selection of Device.
        self.CheckLayout = QtGui.QHBoxLayout()
        self.CheckProyectLabel = QtGui.QLabel("Proyect Checked:")
        self.CheckProyectLabel.setFixedWidth(100)
        self.CheckStatusProyectLabel = QtGui.QLabel("<div style=\"color:#FF0000\"><b>NOT PASS<\b><\div>")
        self.CheckStatusProyectLabel.setFixedWidth(80)
        self.ButtonCheckProyect = QtGui.QPushButton("Check Proyect")
        self.ButtonCheckProyect.clicked.connect(self.checkProyect)
        self.ButtonCheckProyect.setFixedWidth(100)
        self.CheckLayout.addWidget(self.CheckProyectLabel)
        self.CheckLayout.addWidget(self.CheckStatusProyectLabel)
        self.CheckLayout.addWidget(self.ButtonCheckProyect)
        
        self.BuildLayout = QtGui.QHBoxLayout()
        self.BuildProyectLabel = QtGui.QLabel("Proyect Built:")
        self.BuildProyectLabel.setFixedWidth(100)
        self.BuildStatusProyectLabel = QtGui.QLabel("<div style=\"color:#FF0000\"><b>NOT PASS<\b><\div>")
        self.BuildStatusProyectLabel.setFixedWidth(80)
        self.ButtonBuildProyect = QtGui.QPushButton("Build Proyect")
        self.ButtonBuildProyect.clicked.connect(self.buildProyect)
        self.ButtonBuildProyect.setFixedWidth(100)
        self.BuildLayout.addWidget(self.BuildProyectLabel)
        self.BuildLayout.addWidget(self.BuildStatusProyectLabel)
        self.BuildLayout.addWidget(self.ButtonBuildProyect)

        # Save the Device Widget.
        self.DeviceLayoutRight.addLayout(self.CheckLayout)
        self.DeviceLayoutRight.addLayout(self.BuildLayout)
    
        # Save all Layout.
        spacer = QtGui.QLabel()
        spacer.setMinimumWidth(15)
        self.DeviceLayout.addLayout(self.DeviceLayoutLeft)
        self.DeviceLayout.addWidget(spacer)
        self.DeviceLayout.addLayout(self.DeviceLayoutCenter)
        self.DeviceLayout.addWidget(spacer)
        self.DeviceLayout.addLayout(self.DeviceLayoutRight)
        #### END  Section Device Type ####
        
        #### Group Box Device ####
        # Create the Layout structure.
        self.SectionDevice = QtGui.QGroupBox()
        self.SectionDevice.setTitle("Select Device")
        self.SectionDevice.setLayout(self.DeviceLayout) 
        self.SectionDevice.setFixedHeight(135)
        self.SettingLayout.addWidget(self.SectionDevice)  


        #### Section Host Type ####
        # Section HostMode
        # Create the Layout structure.
        self.HostModeLayout = QtGui.QHBoxLayout(self)
        self.HostModeLayoutRight = QtGui.QVBoxLayout(self)
        self.HostModeLayoutLeft = QtGui.QVBoxLayout(self)

        # Create the Selection HostMode Section
        self.HostModeSectionLabel = QtGui.QLabel("Select a Mode:")
        self.SelectHostMode = QtGui.QComboBox()
        self.SelectHostMode.addItem("--- Select Mode ---")
        self.SelectHostMode.addItem("Normal Mode")
        self.SelectHostMode.addItem("Validation Mode")
        self.SelectHostMode.currentIndexChanged.connect(self.hostModeManager)
        
        # Save the Release widget.
        self.HostModeLayoutLeft.addWidget(self.HostModeSectionLabel) 
        self.HostModeLayoutLeft.addWidget(self.SelectHostMode) 

        # Create the Selection of SubHostMode.
        self.SubHostModeSectionLabel = QtGui.QLabel("Select the SubMode:")        
        self.HostModeModel = QtGui.QStandardItemModel()

        self.viewHostMode = QtGui.QListView()
        self.viewHostMode.setModel(self.HostModeModel)
        self.viewHostMode.setFixedHeight(80)
        
        # Save the Device Widget.
        self.HostModeLayoutLeft.addWidget(self.SubHostModeSectionLabel)
        self.HostModeLayoutLeft.addWidget(self.viewHostMode)

        
        # Create the Selection of SubHostMode.
        self.EnableLabel = QtGui.QLabel("Enable peripherals:")        
        self.HostModelEnable = QtGui.QStandardItemModel()
        self.peripheralList = ["Geolocation","Memory SD","Internal IMU",
                            "Battery Monitor"]

        for peripheral in self.peripheralList:
            item = QtGui.QStandardItem(peripheral)
            item.setCheckable(True)
            item.setEditable(False)
            self.HostModelEnable.appendRow(item)

        self.ViewEnableHost = QtGui.QListView()
        self.ViewEnableHost.setModel(self.HostModelEnable)
        self.ViewEnableHost.setFixedHeight(80)
        
        # Save the Device Widget.
        self.HostModeLayoutLeft.addWidget(self.EnableLabel)
        self.HostModeLayoutLeft.addWidget(self.ViewEnableHost)
        
        # Save all Layout.    
        self.HostModeLayout.addLayout(self.HostModeLayoutLeft)
        self.HostModeLayout.addLayout(self.HostModeLayoutRight)
        
        
        #### Setting the Tabs Setting ####
        self.SettingPeripherals = QtGui.QTabWidget()
        self.SettingPeripherals.setTabPosition(QtGui.QTabWidget.South)
        self.SettingPeripherals.setFixedWidth(650)
        
        self.A1Setting = QAnalogSetting("A1",1)
        self.SettingPeripherals.addTab(self.A1Setting,"A1")
        self.A2Setting = QAnalogSetting("A2",1)
        self.SettingPeripherals.addTab(self.A2Setting,"A2")
        self.A3Setting = QAnalogSetting("A3",1)
        self.SettingPeripherals.addTab(self.A3Setting,"A3")
        self.A4Setting = QAnalogSetting("A4",2)
        self.SettingPeripherals.addTab(self.A4Setting,"A4")
        self.A5Setting = QAnalogSetting("A5",3)
        self.SettingPeripherals.addTab(self.A5Setting,"A5")
        self.I2CSetting = QCommandI2CList()
        self.SettingPeripherals.addTab(self.I2CSetting ,"I2C")
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
        self.SelectCenterMode.currentIndexChanged.connect(self.centerModeManager)
        # Save the Release widget.
        self.CenterModeLayoutLeft.addWidget(self.CenterModeSectionLabel) 
        self.CenterModeLayoutLeft.addWidget(self.SelectCenterMode) 

        # Create the Selection of SubCenterMode.
        self.SubCenterModeSectionLabel = QtGui.QLabel("Select the SubMode:")        
        self.CenterModeModel = QtGui.QStandardItemModel()
        
        self.ViewEnableCenter = QtGui.QListView()
        self.ViewEnableCenter.setModel(self.CenterModeModel)
        
        # Save the Device Widget.
        self.CenterModeLayoutLeft.addWidget(self.SubCenterModeSectionLabel)
        self.CenterModeLayoutLeft.addWidget(self.ViewEnableCenter)

        # Save all Layout.    
        self.CenterModeLayout.addLayout(self.CenterModeLayoutLeft)
        self.CenterModeLayout.addLayout(self.CenterModeLayoutRight)
        
        
        self.SubCenterMessageSectionLabel = QtGui.QLabel("Message List:")        
        self.MessageListModel = QtGui.QStandardItemModel()

        #item = QtGui.QStandardItem("Debug")
        #item.setCheckable(True)
        #item.setEditable(False)
        #MessageListModel.appendRow(item)

        self.MessageView = QtGui.QListView()
        self.MessageView.setModel(self.MessageListModel)
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
        self.addMessageButton.clicked.connect(self.addNewCommand)
        self.delMessageButton = QtGui.QPushButton("Delete Messages")
        self.delMessageButton.clicked.connect(self.delNewCommand)
        
        self.MessageLayout.addWidget(self.addressLabel)
        self.MessageLayout.addWidget(self.addressMessage)
        self.MessageLayout.addWidget(self.commandLabel)
        self.MessageLayout.addWidget(self.commandMessage)
        self.MessageLayout.addWidget(self.addMessageButton)
        self.MessageLayout.addWidget(self.delMessageButton)
        
        # Save the Device Widget.
        self.CenterModeLayoutRight.addWidget(self.SubCenterMessageSectionLabel)
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

    def updateReleaseList(self, index = 0):
        self.path = "workspace/releases/"
        self.pathProyects = "workspace/src/"
        fileList = glob.glob(self.path+"*.zip")
        self.SelectRelease.clear()        
        self.SelectRelease.addItem("--- Select Release Version ---")
        for file_ in fileList:
            self.SelectRelease.addItem(file_.split('\\')[-1])

    def selectCenter(self):
        self.SectionCenterOptions.show()
        self.SectionHostOptions.hide()

    def selectHost(self):
        self.SectionHostOptions.show()
        self.SectionCenterOptions.hide()

    def downloadRelease(self):
        print "HELLO"
        
    def hostModeManager(self):
        if self.SelectHostMode.currentText() == "Normal Mode":
            self.HostModeModel.clear()
            hostNormalModeList = ["Debug","Debug Color","Debug Comm. Packages",
                            "Debug Comm. Status","Debug Analog A1","Debug Analog A2",
                            "Debug Analog A3","Debug Analog A4","Debug Analog A5"]

            for mode in hostNormalModeList:
                item = QtGui.QStandardItem(mode)
                item.setCheckable(True)
                item.setEditable(False)
                self.HostModeModel.appendRow(item)

        elif self.SelectHostMode.currentText() == "Validation Mode":
            self.HostModeModel.clear()
            hostNormalModeList = ["Unitary Test","Validation"]

            for mode in hostNormalModeList:
                item = QtGui.QStandardItem(mode)
                item.setCheckable(True)
                item.setEditable(False)
                self.HostModeModel.appendRow(item)

        else:
            self.HostModeModel.clear()
            print "No option selected."
            
    
    def centerModeManager(self):
        if self.SelectCenterMode.currentText() == "Normal Mode":
            self.CenterModeModel.clear()
            centerNormalModeList = ["Debug","Debug Comm. Packages",
                            "Debug Comm. Status"]

            for mode in centerNormalModeList:
                item = QtGui.QStandardItem(mode)
                item.setCheckable(True)
                item.setEditable(False)
                self.CenterModeModel.appendRow(item)

        elif self.SelectCenterMode.currentText() == "Validation Mode":
            self.CenterModeModel.clear()
            centerNormalModeList = ["Unitary Test","Validation"]

            for mode in centerNormalModeList:
                item = QtGui.QStandardItem(mode)
                item.setCheckable(True)
                item.setEditable(False)
                self.CenterModeModel.appendRow(item)

        else:
            CenterModeModel.clear()
            print "No option selected."

    def addNewCommand(self):
        item = QtGui.QStandardItem("Address="+self.addressMessage.toPlainText()+" Command="+self.commandMessage.toPlainText())
        item.setCheckable(True)
        item.setEditable(False)
        self.MessageListModel.appendRow(item)


    def delNewCommand(self):
        eraseList = []
        eraseListSorted = []
        for row in range(self.MessageListModel.rowCount()):
            item = self.MessageListModel.item(row)
            if item.checkState() == QtCore.Qt.Checked:
                eraseList.append(row)
        
        eraseListSorted = sorted(eraseList,reverse=True)
        for row in eraseListSorted:
            self.MessageListModel.removeRow(row)

    def checkProyect(self):
        if self.checkProyectItems():
            if self.checkSettingItems():
                self.CheckStatusProyectLabel.setText("<div style=\"color:#90FFA1\"><b>PASS<\b><\div>")
            else:
                self.CheckStatusProyectLabel.setText("<div style=\"color:#FFDD90\"><b>WARNING<\b><\div>")
            return True
        else:
            self.CheckStatusProyectLabel.setText("<div style=\"color:#FF0000\"><b>NOT PASS<\b><\div>")
            return False
                

    def checkProyectItems(self):
        stringFail = ""
        itemFail = 0
        if not re.match("^[a-zA-Z0-9_ ]*$", self.ProyectNameTextEdit.toPlainText()):
            itemFail = itemFail + 1
            self.ProyectNameLabel.setText("<div style=\"color:#FF0000\">Proyect Name*:<\div>")
            stringFail += "\t- Proyect name with especial characters (use only a-Z, 0-9, _ and space ).\n"
        else:
            self.ProyectNameLabel.setText("Proyect Name:")

        if self.SelectRelease.currentText() == "--- Select Release Version ---":
            itemFail = itemFail + 1
            self.ReleaseSectionLabel.setText("<div style=\"color:#FF0000\">Select a Release*:<\div>")
            stringFail += "\t- Select a correct release version.\n"
        else:
            self.ReleaseSectionLabel.setText("Select a Release:")
            
        if not re.match("^[a-fA-F0-9_x]*$", self.DeviceIDTextEdit.toPlainText()):
            itemFail = itemFail + 1
            self.DeviceIDLabel.setText("<div style=\"color:#FF0000\">Device ID*:<\div>")
            stringFail += "\t- Hexadecimal value error.\n"
        elif int(str(self.DeviceIDTextEdit.toPlainText()), 16) > 255 or  int(str(self.DeviceIDTextEdit.toPlainText()), 16) < 0:
            itemFail = itemFail + 1
            self.DeviceIDLabel.setText("<div style=\"color:#FF0000\">Device ID*:<\div>")
            stringFail += "\t- Hexadecimal range error.\n"
        else:
            self.DeviceIDLabel.setText("Device ID:")

        if itemFail > 0:
            msg = QtGui.QMessageBox()
            msg.setIcon(QtGui.QMessageBox.Critical)
            msg.setText("Critical Error with proyect building\t\t\t\t\t\t\t\t\t")
            msg.setInformativeText("Click show details for more information")
            msg.setWindowTitle("Proyect Error ("+ str(itemFail) +")")
            msg.setDetailedText("Issue List ("+ str(itemFail) +"):\n"+stringFail)
            msg.setStandardButtons(QtGui.QMessageBox.Ok)
            msg.exec_()
            return False
        else:
            return True


    def checkSettingItems(self):
        stringFail = ""
        itemFail = 0
        
        if self.CenterDevice.isChecked():
            if self.SelectCenterMode.currentText() == "--- Select Mode ---":
                itemFail = itemFail + 1
                self.CenterModeSectionLabel.setText("<div style=\"color:#FF0000\">Select a Mode*:<\div>")
                stringFail += "\t- No mode selected.\n"
            else:
                self.CenterModeSectionLabel.setText("Select a Mode:")

            self.SubCenterMessageSectionLabel.setText("Message List:")
            if self.MessageListModel.rowCount() < 1:
                itemFail = itemFail + 1
                self.SubCenterMessageSectionLabel.setText("<div style=\"color:#FF0000\">Message List*:<\div>")
                stringFail += "\t- No message added.\n"
            else:
                for row in range(self.MessageListModel.rowCount()):
                    item = self.MessageListModel.item(row)
                    itemText = item.text().split(" ")
                    if len(itemText) != 2:
                        itemFail = itemFail + 1
                        item.setBackground(QtGui.QColor("#FFDD90"))
                        stringFail += "\t- Message ("+str(row)+"): Format wrong.\n"
                    else:
                        if not re.match("^[a-fA-F0-9_x]*$", itemText[0].split("=")[-1]) or not re.match("^[a-fA-F0-9_x]*$", itemText[-1].split("=")[-1]):
                            itemFail = itemFail + 1
                            item.setBackground(QtGui.QColor("#FFDD90"))
                            stringFail += "\t- Message ("+str(row)+"): Data wrong.\n"
                        elif int(str(itemText[0].split("=")[-1]), 16) > 255 or int(str(itemText[-1].split("=")[-1]), 16) > 255:
                            itemFail = itemFail + 1
                            item.setBackground(QtGui.QColor("#FFDD90"))
                            stringFail += "\t- Message ("+str(row)+"): Range wrong.\n"
                        else:
                            item.setBackground(QtGui.QColor(255,255,255))
                

            if itemFail > 0:
                msg = QtGui.QMessageBox()
                msg.setIcon(QtGui.QMessageBox.Warning)
                msg.setText("Warning Error with proyect setting\t\t\t\t\t\t\t\t\t")
                msg.setInformativeText("Click show details for more information")
                msg.setWindowTitle("Setting Warning ("+ str(itemFail) +")")
                msg.setDetailedText("Warning List ("+ str(itemFail) +"):\n"+stringFail)
                msg.setStandardButtons(QtGui.QMessageBox.Ok)
                msg.exec_()
                return False
            else:
                return True
                                    
                        
        elif self.HostDevice.isChecked():
            if self.SelectHostMode.currentText() == "--- Select Mode ---":
                itemFail = itemFail + 1
                self.HostModeSectionLabel.setText("<div style=\"color:#FF0000\">Select a Mode*:<\div>")
                stringFail += "\t- No mode selected.\n"
            else:
                self.HostModeSectionLabel.setText("Select a Mode:")

            if self.A1Setting.checkSettingItems():
                self.SettingPeripherals.tabBar().setTabTextColor(0,QtGui.QColor("#000000"))
            else:
                itemFail = itemFail + 1
                self.SettingPeripherals.tabBar().setTabTextColor(0,QtGui.QColor("#FF0000"))
                stringFail += "\t- Analog A1 Wrong.\n"

            if self.A2Setting.checkSettingItems():
                self.SettingPeripherals.tabBar().setTabTextColor(1,QtGui.QColor("#000000"))
            else:
                itemFail = itemFail + 1
                self.SettingPeripherals.tabBar().setTabTextColor(1,QtGui.QColor("#FF0000"))
                stringFail += "\t- Analog A2 Wrong.\n"

            if self.A3Setting.checkSettingItems():
                self.SettingPeripherals.tabBar().setTabTextColor(2,QtGui.QColor("#000000"))
            else:
                itemFail = itemFail + 1
                self.SettingPeripherals.tabBar().setTabTextColor(2,QtGui.QColor("#FF0000"))
                stringFail += "\t- Analog A3 Wrong.\n"

            if self.A4Setting.checkSettingItems():
                self.SettingPeripherals.tabBar().setTabTextColor(3,QtGui.QColor("#000000"))
            else:
                itemFail = itemFail + 1
                self.SettingPeripherals.tabBar().setTabTextColor(3,QtGui.QColor("#FF0000"))
                stringFail += "\t- Analog A4 Wrong.\n"

            if self.A5Setting.checkSettingItems():
                self.SettingPeripherals.tabBar().setTabTextColor(4,QtGui.QColor("#000000"))
            else:
                itemFail = itemFail + 1
                self.SettingPeripherals.tabBar().setTabTextColor(4,QtGui.QColor("#FF0000"))
                stringFail += "\t- Analog A5 Wrong.\n"
                
            if len(self.I2CSetting.checkSetting()) > 0:
                itemFail = itemFail + 1
                stringFail += "\t- I2C setting Wrong.\n"
                

            if itemFail > 0:
                msg = QtGui.QMessageBox()
                msg.setIcon(QtGui.QMessageBox.Warning)
                msg.setText("Warning Error with proyect setting\t\t\t\t\t\t\t\t\t")
                msg.setInformativeText("Click show details for more information")
                msg.setWindowTitle("Setting Warning ("+ str(itemFail) +")")
                msg.setDetailedText("Warning List ("+ str(itemFail) +"):\n"+stringFail)
                msg.setStandardButtons(QtGui.QMessageBox.Ok)
                msg.exec_()
                return False
            else:
                return True
            
        else:
            return False
            

    def buildProyect(self):
        if self.checkProyect():
            settingValues = self.buildSettingItems()
            
            if not os.path.exists(self.pathProyects):
                os.makedirs(self.pathProyects)
                
            proyectFolder = os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText()))
            if not os.path.exists(proyectFolder):
                os.makedirs(proyectFolder)

            settingPath = os.path.join(proyectFolder, "wimos_station_main/_setting.h")
            if os.path.exists(settingPath):
                file = open(settingPath, 'w+')
            else:
                releasePath = os.path.join(self.path, str(self.SelectRelease.currentText()))
                zip_ref = zipfile.ZipFile(releasePath, 'r')
                zip_ref.extractall(proyectFolder)
                zip_ref.close()
                
                file = open(settingPath, 'w+')
            file.write(settingValues)
            file.close()
            
            self.BuildStatusProyectLabel.setText("<div style=\"color:#90FFA1\"><b>PASS<><\div>")
        else:
            self.BuildStatusProyectLabel.setText("<div style=\"color:#FF0000\"><b>NOT PASS<\b><\div>")
            
            




    def buildSettingItems(self):
        stringBuild = str("")

        stringBuild += "#define WIMOS_ID "+str(int(str(self.DeviceIDTextEdit.toPlainText()), 16))+"\n"
        
        if self.CenterDevice.isChecked():
            stringBuild += "#ifndef __AVR_ATmega32U4__\n"
            stringBuild += "\t#error \"You should select the Center Board\"\n"
            stringBuild += "#endif\n"
            
            for row in range(self.CenterModeModel.rowCount()):
                item = self.CenterModeModel.item(row)
                if item.checkState() == QtCore.Qt.Checked:
                    if item.text() == "Debug":
                        stringBuild += "#define WIMOS_DEBUG\n"
                    elif item.text() == "Debug Comm. Packages":
                        stringBuild += "#define DEBUG_COMM_RXTX\n"
                    elif item.text() == "Debug Comm. Status":
                        stringBuild += "#define DEBUG_COMM_STATUS\n"
                    elif item.text() == "Unitary Test":
                        stringBuild += "#define WIMOS_UNIT_TEST\n"
                    elif item.text() == "Validation":
                        stringBuild += "#define WIMOS_VALIDATION_TEST\n"
                    else:
                        print "[Warning] Uknown Submode"
                        
            
            if self.SelectCenterMode.currentText() == "Normal Mode":                
                stringBuild += "#undef WIMOS_UNIT_TEST\n"               
                stringBuild += "#undef WIMOS_VALIDATION_TEST\n"
            elif self.SelectCenterMode.currentText() == "Validation Mode":                         
                stringBuild += "#undef DEBUG_COMM_RXTX\n"               
                stringBuild += "#undef DEBUG_COMM_STATUS\n"           
                stringBuild += "#undef DEBUG_ANALOG_A1\n"           
                stringBuild += "#undef DEBUG_ANALOG_A2\n"           
                stringBuild += "#undef DEBUG_ANALOG_A3\n"         
                stringBuild += "#undef DEBUG_ANALOG_A4\n"           
                stringBuild += "#undef DEBUG_ANALOG_A5\n"    
                stringBuild += "#undef WIMOS_DEBUG\n"
            else:
                print "[Warning] No select mode."


            stringBuild += "#define _EN_WIMOS_RF \n"   
            stringBuild += "#define TIME_COMMAND_RATE 60000 \n"         
            stringBuild += "#define _WIMOS_COMMAND_LIST_SIZE "+str(self.MessageListModel.rowCount())+" \n"         
            stringBuild += "#define _WIMOS_COMMAND_LIST {\\ \n"
            for row in range(self.MessageListModel.rowCount()):
                item = self.MessageListModel.item(row)
                stringBuild += "(stCommandMessage) { "
                stringBuild += ".ucBegin = COMMAND_BEGIN_BYTE_CONST, "
                stringBuild += ".ucFrameID = COMMAND_SIZE_BYTE_CONST, "
                stringBuild += ".ucMessageFrom = WIMOS_ID, "
                stringBuild += ".ucMessageTo = "+str(int(str(item.text().split(" ")[0].split("=")[-1]),16))+", "
                stringBuild += ".ucCommand = "+str(int(str(item.text().split(" ")[-1].split("=")[-1]),16))+", "
                stringBuild += ".ucChecksum = 0x00 }"
                if row == (self.MessageListModel.rowCount()-1):
                    stringBuild += " \\ \n"                
                else:
                    stringBuild += ",\\ \n"
            stringBuild += "}"
            return stringBuild
        elif self.HostDevice.isChecked():            
            stringBuild += "#ifndef __SAM3X8E__\n"
            stringBuild += "\t#error \"You should select the Host Board\"\n"
            stringBuild += "#endif\n"
            
            for row in range(self.HostModeModel.rowCount()):
                item = self.HostModeModel.item(row)
                if item.checkState() == QtCore.Qt.Checked:
                    if item.text() == "Debug":
                        stringBuild += "#define WIMOS_DEBUG\n"
                    elif item.text() == "Debug Color":
                        stringBuild += "#define DEBUG_COLOR\n"
                    elif item.text() == "Debug Comm. Packages":
                        stringBuild += "#define DEBUG_COMM_RXTX\n"
                    elif item.text() == "Debug Comm. Status":
                        stringBuild += "#define DEBUG_COMM_STATUS\n"
                    elif item.text() == "Debug Analog A1":
                        stringBuild += "#define DEBUG_ANALOG_A1\n"
                    elif item.text() == "Debug Analog A2":
                        stringBuild += "#define DEBUG_ANALOG_A2\n"
                    elif item.text() == "Debug Analog A3":
                        stringBuild += "#define DEBUG_ANALOG_A3\n"
                    elif item.text() == "Debug Analog A4":
                        stringBuild += "#define DEBUG_ANALOG_A4\n"
                    elif item.text() == "Debug Analog A5":
                        stringBuild += "#define DEBUG_ANALOG_A5\n"
                    elif item.text() == "Unitary Test":
                        stringBuild += "#define WIMOS_UNIT_TEST\n"
                    elif item.text() == "Validation":
                        stringBuild += "#define WIMOS_VALIDATION_TEST\n"
                    else:
                        print "[Warning] Uknown Submode"
                        
            
            if self.SelectHostMode.currentText() == "Normal Mode":                
                stringBuild += "#undef WIMOS_UNIT_TEST\n"               
                stringBuild += "#undef WIMOS_VALIDATION_TEST\n"
            elif self.SelectHostMode.currentText() == "Validation Mode":                         
                stringBuild += "#undef DEBUG_COMM_RXTX\n"               
                stringBuild += "#undef DEBUG_COMM_STATUS\n"           
                stringBuild += "#undef DEBUG_ANALOG_A1\n"           
                stringBuild += "#undef DEBUG_ANALOG_A2\n"           
                stringBuild += "#undef DEBUG_ANALOG_A3\n"         
                stringBuild += "#undef DEBUG_ANALOG_A4\n"           
                stringBuild += "#undef DEBUG_ANALOG_A5\n"    
                stringBuild += "#undef WIMOS_DEBUG\n"
            else:
                print "[Warning] No select mode."
            
            for row in range(self.HostModelEnable.rowCount()):
                item = self.HostModelEnable.item(row)
                if item.checkState() == QtCore.Qt.Checked:
                    if item.text() == "Geolocation":
                        stringBuild += "#define _EN_WIMOS_GPS\n"
                    elif item.text() == "Memory SD":
                        stringBuild += "#define _EN_WIMOS_SD\n"
                    elif item.text() == "Internal IMU":
                        stringBuild += "#define _EN_WIMOS_IMU\n"
                    elif item.text() == "Battery Monitor":
                        stringBuild += "#define _EN_WIMOS_BAT\n"
                    elif item.text() == "RF Communication":
                        stringBuild += "#define _EN_WIMOS_RF\n"
                    else:
                        print "[Warning] Uknown Submode"
            
            
            stringBuild += str(self.A1Setting.buildSetting())
            stringBuild += str(self.A2Setting.buildSetting())
            stringBuild += str(self.A3Setting.buildSetting())
            stringBuild += str(self.A4Setting.buildSetting())
            stringBuild += str(self.A5Setting.buildSetting())   
            stringBuild += str(self.I2CSetting.buildSetting())          
            
            return stringBuild
        else:
            return stringBuild
        
    def n7UT03(self):        
        testCount = 0
        testSuccess = 0

        self.selectCenter()
        testCount += 1
        if(self.SectionCenterOptions.isHidden() == False and self.SectionHostOptions.isHidden() == True):
            testSuccess += 1
        
        
        return (testSuccess/testCount)*100
    
    def n7UT04(self):        
        testCount = 0
        testSuccess = 0

        self.selectHost()
        testCount += 1
        if(self.SectionCenterOptions.isHidden() == True and self.SectionHostOptions.isHidden() == False):
            testSuccess += 1
        
        
        return (testSuccess/testCount)*100

    
    
    def n7UT05(self):        
        testCount = 0
        testSuccess = 0

        testCount += 1
        testCounterLabel = 0
        self.SelectHostMode.setCurrentIndex(1)
        self.hostModeManager()
        for row in range(self.HostModeModel.rowCount()):
            item = self.HostModeModel.item(row)
            if item.text() == "Debug":
                testCounterLabel+=1
            elif item.text() == "Debug Color":
                testCounterLabel+=1
            elif item.text() == "Debug Comm. Packages":
                testCounterLabel+=1
            elif item.text() == "Debug Comm. Status":
                testCounterLabel+=1
            elif item.text() == "Debug Analog A1":
                testCounterLabel+=1
            elif item.text() == "Debug Analog A2":
                testCounterLabel+=1
            elif item.text() == "Debug Analog A3":
                testCounterLabel+=1
            elif item.text() == "Debug Analog A4":
                testCounterLabel+=1
            elif item.text() == "Debug Analog A5":
                testCounterLabel+=1
            else:                    
                testCounterLabel+=10
        if testCounterLabel == 9:
            testSuccess+=1
            
        return (testSuccess/testCount)*100

            
    def n7UT06(self):        
        testCount = 0
        testSuccess = 0

        testCount += 1
        testCounterLabel = 0
        self.SelectHostMode.setCurrentIndex(2)
        self.hostModeManager()
        for row in range(self.HostModeModel.rowCount()):
            item = self.HostModeModel.item(row)
            if item.text() == "Unitary Test":                    
                testCounterLabel+=1
            elif item.text() == "Validation":                    
                testCounterLabel+=1
            else:                    
                testCounterLabel+=10
        if testCounterLabel == 2:
            testSuccess+=1
            
        return (testSuccess/testCount)*100


    
    
    def n7UT07(self):        
        testCount = 0
        testSuccess = 0

        testCount += 1
        testCounterLabel = 0
        self.SelectCenterMode.setCurrentIndex(1)
        self.centerModeManager()
        for row in range(self.CenterModeModel.rowCount()):
            item = self.CenterModeModel.item(row)
            if item.text() == "Debug":
                testCounterLabel+=1
            elif item.text() == "Debug Comm. Packages":
                testCounterLabel+=1
            elif item.text() == "Debug Comm. Status":
                testCounterLabel+=1
            else:                    
                testCounterLabel+=10
        if testCounterLabel == 3:
            testSuccess+=1
            
        return (testSuccess/testCount)*100

            
    def n7UT08(self):        
        testCount = 0
        testSuccess = 0

        testCount += 1
        testCounterLabel = 0
        self.SelectCenterMode.setCurrentIndex(2)
        self.centerModeManager()
        for row in range(self.CenterModeModel.rowCount()):
            item = self.CenterModeModel.item(row)
            if item.text() == "Unitary Test":                    
                testCounterLabel+=1
            elif item.text() == "Validation":                    
                testCounterLabel+=1
            else:                    
                testCounterLabel+=10
        if testCounterLabel == 2:
            testSuccess+=1
            
        return (testSuccess/testCount)*100

            
    def n7UT09(self):        
        testCount = 0
        testSuccess = 0

        
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()
        
        testCount += 1
        if self.MessageListModel.rowCount() == 1:
            testSuccess+=1
            
        testCount += 1
        if self.MessageListModel.item(0).text() == "Address=0x0A Command=0x0B":
            testSuccess+=1
        
        for row in range(self.MessageListModel.rowCount()):
            item = self.MessageListModel.item(row)
            item.setCheckState(QtCore.Qt.Checked)
        
        self.delNewCommand()
            
        return (testSuccess/testCount)*100

            
    def n7UT10(self):        
        testCount = 0
        testSuccess = 0

        
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        if self.MessageListModel.rowCount() == 0:
            self.addNewCommand()
        
        testCount += 1
        if self.MessageListModel.rowCount() == 1:
            testSuccess+=1

        for row in range(self.MessageListModel.rowCount()):
            item = self.MessageListModel.item(row)
            item.setCheckState(QtCore.Qt.Checked)

        self.delNewCommand()
            
        testCount += 1
        if self.MessageListModel.rowCount() == 0:
            testSuccess+=1            

        for row in range(self.MessageListModel.rowCount()):
            item = self.MessageListModel.item(row)
            item.setCheckState(QtCore.Qt.Checked)
        
        self.delNewCommand()
            
        return (testSuccess/testCount)*100
                
    def n7UT11(self):        
        testCount = 0
        testSuccess = 0

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(1)
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        self.checkProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#90FFA1\"><b>PASS<><\div>":
            testSuccess+=1        
        
        for row in range(self.MessageListModel.rowCount()):
            item = self.MessageListModel.item(row)
            item.setCheckState(QtCore.Qt.Checked)
        
        self.delNewCommand()
            
        return (testSuccess/testCount)*100
                
    def n7UT12(self):        
        testCount = 0
        testSuccess = 0

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(1)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("10")
        
        self.checkProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#90FFA1\"><b>PASS<><\div>":
            testSuccess+=1        

        for row in range(self.MessageListModel.rowCount()):
            item = self.MessageListModel.item(row)
            item.setCheckState(QtCore.Qt.Checked)
        
        self.delNewCommand()
            
        return (testSuccess/testCount)*100

    
    def n7UT13(self):        
        testCount = 0
        testSuccess = 0

        self.ProyectNameTextEdit.setPlainText("UTs_Testing?")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(1)
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        self.checkProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#FF0000\"><b>NOT PASS<><\div>":
            testSuccess+=1        
        
        for row in range(self.MessageListModel.rowCount()):
            item = self.MessageListModel.item(row)
            item.setCheckState(QtCore.Qt.Checked)
        
        self.delNewCommand()
            
        return (testSuccess/testCount)*100
                

    def n7UT14(self):        
        testCount = 0
        testSuccess = 0

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(0)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(1)
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        self.checkProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#FF0000\"><b>NOT PASS<><\div>":
            testSuccess+=1
            
        for row in range(self.MessageListModel.rowCount()):
            item = self.MessageListModel.item(row)
            item.setCheckState(QtCore.Qt.Checked)  
        
        self.delNewCommand()
            
        return (testSuccess/testCount)*100
                

    def n7UT15(self):        
        testCount = 0
        testSuccess = 0

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x100")
        
        self.SelectCenterMode.setCurrentIndex(1)
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        self.checkProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#FF0000\"><b>NOT PASS<><\div>":
            testSuccess+=1        
            
        for row in range(self.MessageListModel.rowCount()):
            item = self.MessageListModel.item(row)
            item.setCheckState(QtCore.Qt.Checked)
        
        self.delNewCommand()
            
        return (testSuccess/testCount)*100
                


    def n7UT16(self):        
        testCount = 0
        testSuccess = 0

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(1)
        self.addressMessage.setPlainText("0x100")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        self.checkProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#FFDD90\"><b>WARNING<><\div>":
            testSuccess+=1
            
        for row in range(self.MessageListModel.rowCount()):
            item = self.MessageListModel.item(row)
            item.setCheckState(QtCore.Qt.Checked)

        self.delNewCommand()
        
        return (testSuccess/testCount)*100
    
    def n7UT17(self):        
        testCount = 0
        testSuccess = 0

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(1)
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x100")
        self.addNewCommand()

        self.checkProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#FFDD90\"><b>WARNING<><\div>":
            testSuccess+=1        
            
        for row in range(self.MessageListModel.rowCount()):
            item = self.MessageListModel.item(row)
            item.setCheckState(QtCore.Qt.Checked)

        self.delNewCommand()
        
        return (testSuccess/testCount)*100
    
    def n7UT18(self):        
        testCount = 0
        testSuccess = 0

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(0)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("10")
        
        self.checkProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#FFDD90\"><b>WARNING<><\div>":
            testSuccess+=1        
            
        return (testSuccess/testCount)*100
    
    def n7UT19(self):        
        testCount = 0
        testSuccess = 0

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(1)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("256")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("10")
        
        self.checkProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#FFDD90\"><b>WARNING<><\div>":
            testSuccess+=1        
            
        return (testSuccess/testCount)*100
    
    def n7UT20(self):        
        testCount = 0
        testSuccess = 0

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(1)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("4294967296")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("10")
        
        self.checkProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#FFDD90\"><b>WARNING<><\div>":
            testSuccess+=1        
            
        return (testSuccess/testCount)*100
    
    def n7UT21(self):        
        testCount = 0
        testSuccess = 0

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(1)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("256")
        
        self.checkProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#FFDD90\"><b>WARNING<><\div>":
            testSuccess+=1        
            
        return (testSuccess/testCount)*100
                
    def n7UT22(self):        
        testCount = 0
        testSuccess = 0
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(1)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("10")
        
        self.buildProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#90FFA1\"><b>PASS<><\div>":
            testSuccess+=1
        
        testCount += 1
        if self.BuildStatusProyectLabel.text() == "<div style=\"color:#90FFA1\"><b>PASS<><\div>":
            testSuccess+=1
           
        testCount += 1
        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testSuccess+=1

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            shutil.rmtree(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())))

            
        return (testSuccess/testCount)*100
                

                
    def n7UT23(self):        
        testCount = 0
        testSuccess = 0
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(1)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("256")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("10")
        
        self.buildProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#FFDD90\"><b>WARNING<><\div>":
            testSuccess+=1
        
        testCount += 1
        if self.BuildStatusProyectLabel.text() == "<div style=\"color:#90FFA1\"><b>PASS<><\div>":
            testSuccess+=1
           
        testCount += 1
        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testSuccess+=1
            
        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            shutil.rmtree(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())))

            
        return (testSuccess/testCount)*100

    
                

                
    def n7UT24(self):        
        testCount = 0
        testSuccess = 0
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing?")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(1)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("256")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("10")
        
        self.buildProyect()
        
        testCount += 1
        if self.CheckStatusProyectLabel.text() == "<div style=\"color:#FF0000\"><b>NOT PASS<><\div>":
            testSuccess+=1
        
        testCount += 1
        if self.BuildStatusProyectLabel.text() == "<div style=\"color:#FF0000\"><b>NOT PASS<><\div>":
            testSuccess+=1
           
        testCount += 1
        if not os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testSuccess+=1

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            shutil.rmtree(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())))
            
        return (testSuccess/testCount)*100

    
    def n7VT03(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(1)
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()
        
        self.buildProyect()

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testCount += 1
            if '#ifndef __AVR_ATmega32U4__' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1
                
            testCount += 1
            if '#error "You should select the Center Board"' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return (testSuccess/testCount)*100
        else:
            return 0

		
    def n7VT04(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x20")
        
        self.SelectCenterMode.setCurrentIndex(1)
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        
        self.buildProyect()

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testCount += 1
            if '#define WIMOS_ID 32' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return (testSuccess/testCount)*100
        else:
            return 0                

		
    def n7VT05(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(2)
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        
        self.buildProyect()

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testCount += 1
            strCheck = '(stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 10, .ucCommand = 11, .ucChecksum = 0x00 }'
            if strCheck in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return (testSuccess/testCount)*100
        else:
            return 0               

		
    def n7VT06(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(2)

        self.CenterModeModel.item(0).setCheckState(QtCore.Qt.Checked)
        
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        self.buildProyect()
        
        for row in range(self.CenterModeModel.rowCount()):
            item = self.CenterModeModel.item(row)
            item.setCheckState(QtCore.Qt.Unchecked)

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testCount += 1
            if '#define WIMOS_UNIT_TEST' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return (testSuccess/testCount)*100
        else:
            return 0
                               

		
    def n7VT07(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(2)

        self.CenterModeModel.item(1).setCheckState(QtCore.Qt.Checked)
        
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        self.buildProyect()
        
        for row in range(self.CenterModeModel.rowCount()):
            item = self.CenterModeModel.item(row)
            item.setCheckState(QtCore.Qt.Unchecked)

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testCount += 1
            if '#define WIMOS_VALIDATION_TEST' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return (testSuccess/testCount)*100
        else:
            return 0
        
    def n7VT08(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(1)

        self.CenterModeModel.item(0).setCheckState(QtCore.Qt.Checked)
        
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        self.buildProyect()
        
        for row in range(self.CenterModeModel.rowCount()):
            item = self.CenterModeModel.item(row)
            item.setCheckState(QtCore.Qt.Unchecked)

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testCount += 1
            if '#define WIMOS_DEBUG' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return (testSuccess/testCount)*100
        else:
            return 0
        
    def n7VT09(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(1)

        self.CenterModeModel.item(1).setCheckState(QtCore.Qt.Checked)
        
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        self.buildProyect()
        
        for row in range(self.CenterModeModel.rowCount()):
            item = self.CenterModeModel.item(row)
            item.setCheckState(QtCore.Qt.Unchecked)

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testCount += 1
            if '#define DEBUG_COMM_RXTX' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return (testSuccess/testCount)*100
        else:
            return 0
        
    def n7VT10(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectCenterMode.setCurrentIndex(1)

        self.CenterModeModel.item(2).setCheckState(QtCore.Qt.Checked)
        
        self.addressMessage.setPlainText("0x0A")
        self.commandMessage.setPlainText("0x0B")
        self.addNewCommand()

        self.buildProyect()
        
        for row in range(self.CenterModeModel.rowCount()):
            item = self.CenterModeModel.item(row)
            item.setCheckState(QtCore.Qt.Unchecked)

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testCount += 1
            if '#define DEBUG_COMM_STATUS' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return (testSuccess/testCount)*100
        else:
            return 0
        
    def n7VT11(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(1)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("10")

        self.buildProyect()
        
        for row in range(self.CenterModeModel.rowCount()):
            item = self.CenterModeModel.item(row)
            item.setCheckState(QtCore.Qt.Unchecked)

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testCount += 1
            if '#ifndef __SAM3X8E__' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1
                
            testCount += 1
            if '#error "You should select the Host Board"' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return (testSuccess/testCount)*100
        else:
            return 0
        
    def n7VT12(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(1)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("10")

        
        self.A2Setting.EnableCheck.setCheckState(QtCore.Qt.Unchecked) 

        self.buildProyect()
        
        for row in range(self.CenterModeModel.rowCount()):
            item = self.CenterModeModel.item(row)
            item.setCheckState(QtCore.Qt.Unchecked)

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testCount += 1
            if '#define _EN_WIMOS_PORT_A1' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1
                
            testCount += 1
            if '#undef _EN_WIMOS_PORT_A2' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return (testSuccess/testCount)*100
        else:
            return 0
        
    def n7VT13(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(1)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("20")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("25")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("35")
        self.A1Setting.AnalogDetectionTextEdit.setPlainText("ValidationTest01")

        
        self.A2Setting.EnableCheck.setCheckState(QtCore.Qt.Unchecked) 

        self.buildProyect()
        
        for row in range(self.CenterModeModel.rowCount()):
            item = self.CenterModeModel.item(row)
            item.setCheckState(QtCore.Qt.Unchecked)

        if os.path.exists(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")):
            testCount += 1
            if '#define _WIMOS_1A_AVERAGE_SIZE 	 20' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1
                
            testCount += 1
            if '#define _WIMOS_1A_AVERAGE_OFFSET  	 25' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1
                
            testCount += 1
            if '#define _WIMOS_1A_OFFSET_MAX_SIZE 	 35' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            testCount += 1
            if '#define _WIMOS_1A_DETECTION(inputValue) \t ValidationTest01(inputValue)' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return (testSuccess/testCount)*100
        else:
            return 0
                
        
    def n7VT14(self):
        testCount = 0
        testSuccess = 0
        
        self.updateReleaseList()

        self.ProyectNameTextEdit.setPlainText("UTs_Testing")
        self.SelectRelease.setCurrentIndex(1)
        self.CenterDevice.setChecked(False)
        self.HostDevice.setChecked(True)
        self.DeviceIDTextEdit.setPlainText("0x01")
        
        self.SelectHostMode.setCurrentIndex(1)
        self.A1Setting.EnableCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AnalogAverageCheck.setCheckState(QtCore.Qt.Checked) 
        self.A1Setting.AverageSizeTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetTextEdit.setPlainText("10")
        self.A1Setting.AverageOffsetSizeTextEdit.setPlainText("10")
        self.A1Setting.Tabs.AnalogSettingList[0][0].AnalogOffset1TextEdit.setPlainText("ValidationTest02 / 3")
        self.A1Setting.Tabs.AnalogSettingList[0][0].AnalogCoeficient1TextEdit.setPlainText("ValidationTest03 / 3")
        self.A1Setting.Tabs.AnalogSettingList[0][0].AnalogOffset2TextEdit.setPlainText("ValidationTest04 / 3")
        self.A1Setting.Tabs.AnalogSettingList[0][0].AnalogCoeficient2TextEdit.setPlainText("ValidationTest05 / 3")

        
        self.A2Setting.EnableCheck.setCheckState(QtCore.Qt.Unchecked) 

        self.buildProyect()
        
        for row in range(self.CenterModeModel.rowCount()):
            item = self.CenterModeModel.item(row)
            item.setCheckState(QtCore.Qt.Unchecked)
                
            testCount += 1
            if '#define _WIMOS_1A_OFFSET_1 \t ValidationTest02 / 3' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1
                
            testCount += 1
            if '#define _WIMOS_1A_COEFICIENT_1 \t ValidationTest03 / 3' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1
                
            testCount += 1
            if '#define _WIMOS_1A_OFFSET_2 \t ValidationTest04 / 3' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1
                
            testCount += 1
            if '#define _WIMOS_1A_COEFICIENT_2 \t ValidationTest05 / 3' in open(os.path.join(os.path.join(self.pathProyects, str(self.ProyectNameTextEdit.toPlainText())), "wimos_station_main/_setting.h")).read():
                testSuccess+=1

            return int(float(testSuccess/testCount)*100)
        else:
            return 0

    def TestUTs(self):
        resultn7UT03 = self.n7UT03()
        if ( resultn7UT03 == 100):
            print("[TEST] n7UT03\t[OK]\tn7.UT03 = 100%")
        else:
            print("[TEST] n7UT03\t[ERROR]\tn7.UT03 = "+(str(resultn7UT03))+"%")
            
        resultn7UT04 = self.n7UT04()
        if ( resultn7UT04 == 100):
            print("[TEST] n7UT04\t[OK]\tn7.UT04 = 100%")
        else:
            print("[TEST] n7UT04\t[ERROR]\tn7.UT04 = "+(str(resultn7UT04))+"%")
            
        resultn7UT05 = self.n7UT05()
        if ( resultn7UT05 == 100):
            print("[TEST] n7UT05\t[OK]\tn7.UT05 = 100%")
        else:
            print("[TEST] n7UT05\t[ERROR]\tn7.UT05 = "+(str(resultn7UT05))+"%")
            
        resultn7UT06 = self.n7UT06()
        if ( resultn7UT06 == 100):
            print("[TEST] n7UT06\t[OK]\tn7.UT06 = 100%")
        else:
            print("[TEST] n7UT06\t[ERROR]\tn7.UT06 = "+(str(resultn7UT06))+"%")
            
        resultn7UT07 = self.n7UT07()
        if ( resultn7UT07 == 100):
            print("[TEST] n7UT07\t[OK]\tn7.UT07 = 100%")
        else:
            print("[TEST] n7UT07\t[ERROR]\tn7.UT07 = "+(str(resultn7UT07))+"%")
            
        resultn7UT08 = self.n7UT08()
        if ( resultn7UT08 == 100):
            print("[TEST] n7UT08\t[OK]\tn7.UT08 = 100%")
        else:
            print("[TEST] n7UT08\t[ERROR]\tn7.UT08 = "+(str(resultn7UT08))+"%")
            
        resultn7UT09 = self.n7UT09()
        if ( resultn7UT09 == 100):
            print("[TEST] n7UT09\t[OK]\tn7.UT09 = 100%")
        else:
            print("[TEST] n7UT09\t[ERROR]\tn7.UT09 = "+(str(resultn7UT09))+"%")
            
        resultn7UT10 = self.n7UT10()
        if ( resultn7UT10 == 100):
            print("[TEST] n7UT10\t[OK]\tn7.UT10 = 100%")
        else:
            print("[TEST] n7UT10\t[ERROR]\tn7.UT10 = "+(str(resultn7UT10))+"%")
            
            
        resultn7UT11 = self.n7UT11()
        if ( resultn7UT11 == 100):
            print("[TEST] n7UT11\t[OK]\tn7.UT11 = 100%")
        else:
            print("[TEST] n7UT11\t[ERROR]\tn7.UT11 = "+(str(resultn7UT11))+"%")
            
        resultn7UT12 = self.n7UT12()
        if ( resultn7UT12 == 100):
            print("[TEST] n7UT12\t[OK]\tn7.UT12 = 100%")
        else:
            print("[TEST] n7UT12\t[ERROR]\tn7.UT12 = "+(str(resultn7UT12))+"%")
            
        resultn7UT13 = self.n7UT13()
        if ( resultn7UT13 == 100):
            print("[TEST] n7UT13\t[OK]\tn7.UT13 = 100%")
        else:
            print("[TEST] n7UT13\t[ERROR]\tn7.UT13 = "+(str(resultn7UT13))+"%")
            
        resultn7UT14 = self.n7UT14()
        if ( resultn7UT14 == 100):
            print("[TEST] n7UT14\t[OK]\tn7.UT14 = 100%")
        else:
            print("[TEST] n7UT14\t[ERROR]\tn7.UT14 = "+(str(resultn7UT14))+"%")
            
        resultn7UT15 = self.n7UT15()
        if ( resultn7UT15 == 100):
            print("[TEST] n7UT15\t[OK]\tn7.UT15 = 100%")
        else:
            print("[TEST] n7UT15\t[ERROR]\tn7.UT15 = "+(str(resultn7UT15))+"%")
            
        resultn7UT16 = self.n7UT16()
        if ( resultn7UT16 == 100):
            print("[TEST] n7UT16\t[OK]\tn7.UT16 = 100%")
        else:
            print("[TEST] n7UT16\t[ERROR]\tn7.UT16 = "+(str(resultn7UT16))+"%")
            
        resultn7UT17 = self.n7UT17()
        if ( resultn7UT17 == 100):
            print("[TEST] n7UT17\t[OK]\tn7.UT17 = 100%")
        else:
            print("[TEST] n7UT17\t[ERROR]\tn7.UT17 = "+(str(resultn7UT17))+"%")
            
        resultn7UT18 = self.n7UT18()
        if ( resultn7UT18 == 100):
            print("[TEST] n7UT18\t[OK]\tn7.UT18 = 100%")
        else:
            print("[TEST] n7UT18\t[ERROR]\tn7.UT18 = "+(str(resultn7UT18))+"%")
            
        resultn7UT19 = self.n7UT19()
        if ( resultn7UT19 == 100):
            print("[TEST] n7UT19\t[OK]\tn7.UT19 = 100%")
        else:
            print("[TEST] n7UT19\t[ERROR]\tn7.UT19 = "+(str(resultn7UT19))+"%")
            
        resultn7UT20 = self.n7UT20()
        if ( resultn7UT20 == 100):
            print("[TEST] n7UT20\t[OK]\tn7.UT20 = 100%")
        else:
            print("[TEST] n7UT20\t[ERROR]\tn7.UT20 = "+(str(resultn7UT20))+"%")
            
        resultn7UT21 = self.n7UT21()
        if ( resultn7UT21 == 100):
            print("[TEST] n7UT21\t[OK]\tn7.UT21 = 100%")
        else:
            print("[TEST] n7UT21\t[ERROR]\tn7.UT21 = "+(str(resultn7UT21))+"%")
            
        resultn7UT22 = self.n7UT22()
        if ( resultn7UT22 == 100):
            print("[TEST] n7UT22\t[OK]\tn7.UT22 = 100%")
        else:
            print("[TEST] n7UT22\t[ERROR]\tn7.UT22 = "+(str(resultn7UT22))+"%")
            
        resultn7UT23 = self.n7UT23()
        if ( resultn7UT23 == 100):
            print("[TEST] n7UT23\t[OK]\tn7.UT23 = 100%")
        else:
            print("[TEST] n7UT23\t[ERROR]\tn7.UT23 = "+(str(resultn7UT23))+"%")
            
            
        resultn7UT24 = self.n7UT24()
        if ( resultn7UT24 == 100):
            print("[TEST] n7UT24\t[OK]\tn7.UT24 = 100%")
        else:
            print("[TEST] n7UT24\t[ERROR]\tn7.UT24 = "+(str(resultn7UT24))+"%")
            
    def TestVT(self):
        resultn7VT03 = self.n7VT03()
        if ( resultn7VT03 == 100):
            print("[TEST] n7VT03\t[OK]\tn7.VT03 = 100%")
        else:
           print("[TEST] n7VT03\t[ERROR]\tn7.VT03 = "+(str(resultn7VT03))+"%")
           
        resultn7VT04 = self.n7VT04()
        if ( resultn7VT04 == 100):
            print("[TEST] n7VT04\t[OK]\tn7.VT04 = 100%")
        else:
           print("[TEST] n7VT04\t[ERROR]\tn7.VT04 = "+(str(resultn7VT04))+"%")
           
        resultn7VT05 = self.n7VT05()
        if ( resultn7VT05 == 100):
            print("[TEST] n7VT05\t[OK]\tn7.VT05 = 100%")
        else:
           print("[TEST] n7VT05\t[ERROR]\tn7.VT05 = "+(str(resultn7VT05))+"%")
           
        resultn7VT06 = self.n7VT06()
        if ( resultn7VT06 == 100):
            print("[TEST] n7VT06\t[OK]\tn7.VT06 = 100%")
        else:
           print("[TEST] n7VT06\t[ERROR]\tn7.VT06 = "+(str(resultn7VT06))+"%")
           
        resultn7VT07 = self.n7VT07()
        if ( resultn7VT07 == 100):
            print("[TEST] n7VT07\t[OK]\tn7.VT07 = 100%")
        else:
           print("[TEST] n7VT07\t[ERROR]\tn7.VT07 = "+(str(resultn7VT07))+"%")
           
        resultn7VT08 = self.n7VT08()
        if ( resultn7VT08 == 100):
            print("[TEST] n7VT08\t[OK]\tn7.VT08 = 100%")
        else:
           print("[TEST] n7VT08\t[ERROR]\tn7.VT08 = "+(str(resultn7VT08))+"%")
           
        resultn7VT09 = self.n7VT09()
        if ( resultn7VT09 == 100):
            print("[TEST] n7VT09\t[OK]\tn7.VT09 = 100%")
        else:
           print("[TEST] n7VT09\t[ERROR]\tn7.VT09 = "+(str(resultn7VT09))+"%")
           
        resultn7VT10 = self.n7VT10()
        if ( resultn7VT10 == 100):
            print("[TEST] n7VT10\t[OK]\tn7.VT10 = 100%")
        else:
           print("[TEST] n7VT10\t[ERROR]\tn7.VT10 = "+(str(resultn7VT10))+"%")
           
        resultn7VT11 = self.n7VT11()
        if ( resultn7VT11 == 100):
            print("[TEST] n7VT11\t[OK]\tn7.VT11 = 100%")
        else:
           print("[TEST] n7VT11\t[ERROR]\tn7.VT11 = "+(str(resultn7VT11))+"%")
           
        resultn7VT12 = self.n7VT12()
        if ( resultn7VT12 == 100):
            print("[TEST] n7VT12\t[OK]\tn7.VT12 = 100%")
        else:
           print("[TEST] n7VT12\t[ERROR]\tn7.VT12 = "+(str(resultn7VT12))+"%")
           
        resultn7VT13 = self.n7VT13()
        if ( resultn7VT13 == 100):
            print("[TEST] n7VT13\t[OK]\tn7.VT13 = 100%")
        else:
           print("[TEST] n7VT13\t[ERROR]\tn7.VT13 = "+(str(resultn7VT13))+"%")
           
        resultn7VT14 = self.n7VT14()
        if ( resultn7VT14 == 100):
            print("[TEST] n7VT14\t[OK]\tn7.VT14 = 100%")
        else:
           print("[TEST] n7VT14\t[ERROR]\tn7.VT14 = "+(str(resultn7VT14))+"%")

            

