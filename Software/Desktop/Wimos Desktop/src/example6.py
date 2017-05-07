import sys
from PyQt4 import QtCore, QtGui
from random import randint
from datetime import datetime
from AnalogSettingTab import QAnalogSettingTab
import urllib2
import os
import re
import glob


class QCommandI2CList(QtGui.QWidget):
    def __init__(self):
        QtGui.QWidget.__init__(self)
        
        self.EnableInterface = QtGui.QCheckBox("Enable interface")
        self.EnableInterface.stateChanged.connect(self.settingManager)
        self.EnableInterface.setFixedHeight(23)
        self.EnableInterface.setFixedWidth(200)
        self.EnableInterface.setChecked(False)
        
        self.I2CCommandList = QtGui.QLabel("I2C Command List:")        
        self.CommandI2CListModel = QtGui.QStandardItemModel()

        #item = QtGui.QStandardItem("Debug")
        #item.setCheckable(True)
        #item.setEditable(False)
        #MessageListModel.appendRow(item)

        self.CommandI2CView = QtGui.QListView()
        self.CommandI2CView.setModel(self.CommandI2CListModel)
        
        self.CenterModeLayoutRight = QtGui.QVBoxLayout()
        self.MessageLayout = QtGui.QHBoxLayout()
        self.ButtonLayout = QtGui.QHBoxLayout()

        self.addressLabel = QtGui.QLabel("Address:")
        self.addressMessage = QtGui.QTextEdit(str(00))
        self.addressMessage.setFixedHeight(23)
        self.addressMessage.setFixedWidth(50)
        
        self.registerLabel = QtGui.QLabel("Register:")
        self.registerTextEdit = QtGui.QTextEdit(str(00))
        self.registerTextEdit.setFixedHeight(23)
        self.registerTextEdit.setFixedWidth(50)
        
        self.commandLabel = QtGui.QLabel("Command:")
        self.commandMessage = QtGui.QTextEdit(str(00))
        self.commandMessage.setFixedHeight(23)
        self.commandMessage.setFixedWidth(50)
        
        self.EnableWrite = QtGui.QCheckBox("Write")
        self.EnableWrite.stateChanged.connect(self.writeManager)
        self.EnableWrite.setChecked(True)
        self.EnableWrite.setFixedHeight(23)
        self.EnableWrite.setFixedWidth(50)

        self.addMessageButton = QtGui.QPushButton("Add Message")
        self.addMessageButton.clicked.connect(self.addNewCommand)
        self.delMessageButton = QtGui.QPushButton("Delete Messages")
        self.delMessageButton.clicked.connect(self.delCommand)
        
        self.MessageLayout.addWidget(self.addressLabel)
        self.MessageLayout.addWidget(self.addressMessage)
        self.MessageLayout.addWidget(self.registerLabel)
        self.MessageLayout.addWidget(self.registerTextEdit)
        self.MessageLayout.addWidget(self.commandLabel)
        self.MessageLayout.addWidget(self.commandMessage)
        self.ButtonLayout.addWidget(self.addMessageButton)
        self.ButtonLayout.addWidget(self.delMessageButton)
        
        # Save the Device Widget.
        self.CenterModeLayoutRight.addWidget(self.EnableInterface)
        self.CenterModeLayoutRight.addWidget(self.I2CCommandList)
        self.CenterModeLayoutRight.addWidget(self.CommandI2CView)
        self.CenterModeLayoutRight.addWidget(self.EnableWrite)
        self.CenterModeLayoutRight.addLayout(self.MessageLayout)
        self.CenterModeLayoutRight.addLayout(self.ButtonLayout)

        self.setLayout(self.CenterModeLayoutRight)
        self.settingManager()

    def settingManager(self):
        
        if self.EnableInterface.isChecked():            
            self.CommandI2CView.setDisabled(False)        
            self.addressMessage.setDisabled(False)
            self.registerTextEdit.setDisabled(False)
            self.writeManager()
            self.EnableWrite.setDisabled(False)
            self.addMessageButton.setDisabled(False)
            self.delMessageButton.setDisabled(False)
        else:            
            self.CommandI2CView.setDisabled(True)        
            self.addressMessage.setDisabled(True)
            self.registerTextEdit.setDisabled(True)
            self.commandMessage.setDisabled(True)
            self.EnableWrite.setDisabled(True)
            self.addMessageButton.setDisabled(True)
            self.delMessageButton.setDisabled(True)
        

    def writeManager(self):        
        if self.EnableWrite.isChecked():
            self.commandMessage.setDisabled(False)
        else:
            self.commandMessage.setDisabled(True)
        

    def addNewCommand(self):        
        if self.EnableWrite.isChecked():
            item = QtGui.QStandardItem("[W] Address="+self.addressMessage.toPlainText()+" Register="+self.registerTextEdit.toPlainText()+" Command="+self.commandMessage.toPlainText())
        else:
            item = QtGui.QStandardItem("[R] Address="+self.addressMessage.toPlainText()+" Register="+self.registerTextEdit.toPlainText()+" Command=NULL")
        
        item.setCheckable(True)
        item.setEditable(False)
        self.CommandI2CListModel.appendRow(item)

    def delCommand(self):
        eraseList = []
        eraseListSorted = []
        for row in range(self.CommandI2CListModel.rowCount()):
            item = self.CommandI2CListModel.item(row)
            if item.checkState() == QtCore.Qt.Checked:
                eraseList.append(row)
        
        eraseListSorted = sorted(eraseList,reverse=True)
        for row in eraseListSorted:
            self.CommandI2CListModel.removeRow(row)

    def checkSetting(self):
        itemFail = 0
        stringFail = ""
                        
        for row in range(self.CommandI2CListModel.rowCount()):
            item = self.CommandI2CListModel.item(row)
            itemText = item.text().split(" ")
            if len(itemText) != 4:
                itemFail = itemFail + 1
                item.setBackground(QtGui.QColor("#FFDD90"))
                stringFail += "\t- Message ("+str(row)+"): Format wrong.\n"
            else:
                if not itemText[0] == "[W]" and not itemText[0] == "[R]":
                    itemFail = itemFail + 1
                    item.setBackground(QtGui.QColor("#FFDD90"))
                    stringFail += "\t- Message ("+str(row)+"): Message type wrong.\n"
                elif not re.match("^[a-fA-F0-9_x]*$", itemText[1].split("=")[-1]) or not re.match("^[a-fA-F0-9_x]*$", itemText[2].split("=")[-1]) or (not re.match("^[a-fA-F0-9_x]*$", itemText[3].split("=")[-1]) and itemText[0] == "[W]") or (not itemText[3].split("=")[-1] == "NULL" and itemText[0] == "[R]"):
                    itemFail = itemFail + 1
                    item.setBackground(QtGui.QColor("#FFDD90"))
                    stringFail += "\t- Message ("+str(row)+"): Data wrong.\n"
                elif int(str(itemText[1].split("=")[-1]), 16) > 255 or int(str(itemText[2].split("=")[-1]), 16) > 255 or ( itemText[0] == "[W]" and int(str(itemText[3].split("=")[-1]), 16) > 255  ):
                    itemFail = itemFail + 1
                    item.setBackground(QtGui.QColor("#FFDD90"))
                    stringFail += "\t- Message ("+str(row)+"): Range wrong.\n"
                else:
                    item.setBackground(QtGui.QColor(255,255,255))

        return stringFail

    def buildSetting(self):
        stringBuild = ""
        stringConfig = "#define _WIMOS_I2C_CONFIG { "
        sizeConfig = 0
        stringData = "#define _WIMOS_I2C_DATA { "
        sizeData = 0

        for row in range(self.CommandI2CListModel.rowCount()):
            item = self.CommandI2CListModel.item(row)
            itemText = item.text().split(" ")
            if itemText[0] == "[R]":
                if sizeData > 0:
                    stringData += ","
                stringData += "\\ \n (stWimosI2CRegister) {"
                stringData += ".eRegisterType = eDataRead, "
                stringData += ".ucDeviceAddress = "+str(int(str(itemText[1].split("=")[-1]),16))+", "
                stringData += ".ucRegisterAddress = "+str(int(str(itemText[2].split("=")[-1]),16))+", "
                stringData += ".ucRegisterValue = 0x00 }"
                sizeData += 1

            elif itemText[0] == "[W]":
                if stringConfig > 0:
                    stringConfig += ","
                stringConfig += "\\ \n (stWimosI2CRegister) {"
                stringConfig += ".eRegisterType = eDataRead, "
                stringConfig += ".ucDeviceAddress = "+str(int(str(itemText[1].split("=")[-1]),16))+", "
                stringConfig += ".ucRegisterAddress = "+str(int(str(itemText[2].split("=")[-1]),16))+", "
                stringConfig += ".ucRegisterValue = "+str(int(str(itemText[3].split("=")[-1]),16))+" }"
                sizeConfig += 1

        stringConfig += "\\ \n }\n"
        stringData += "\\ \n }\n"
        stringBuild += "#define _WIMOS_PORT_I2C_CONFIG_SIZE "+str(sizeConfig)+"\n"
        stringBuild += "#define _WIMOS_PORT_I2C_DATA_SIZE "+str(sizeData)+"\n"
        stringBuild += stringConfig
        stringBuild += stringData
        
        return stringBuild
        
class exampleQMainWindow (QtGui.QWidget):
    def __init__ (self):
        super(exampleQMainWindow, self).__init__()

        # Create QListWidget
                
        self.InfoLayout = QtGui.QVBoxLayout(self)  
        self.setLayout(self.InfoLayout)
        
        self.list = QAnalogSetting()
        
        self.InfoLayout.addWidget(self.list)
        
        self.button = QtGui.QPushButton()
        self.InfoLayout.addWidget(self.button)
        self.button.clicked.connect(self.list.buildSetting)

    def sendInfo(self):
        ranValue = str(randint(1, 10))
        self.list.addInfo('01','TestFrame',
                          '0x01','0x02',
                          '2017-03-11',
                          '12:03:54.234',
                          '36.56234',
                          '-6.45312',
                          '45%', '76%',
                          '120',
                          datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
        
        self.list.addAlert('01','TestFrame',
                          '0x01','0x02',
                          '2017-03-11',
                          '12:03:54.234',
                          '1','2','3','4','5',
                          datetime.now().strftime('%Y-%m-%d %H:%M:%S'))

        print ranValue

app = QtGui.QApplication([])
window = exampleQMainWindow()
window.show()
sys.exit(app.exec_())
