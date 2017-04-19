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
        self.path = "workspace/releases/"
        fileList = glob.glob(self.path+"*.rar")              
        for file_ in fileList:
            self.SelectRelease.addItem(file_.split('\\')[-1])
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
            print str(self.buildSettingItems())




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
            

