import sys
from PyQt4 import QtCore, QtGui
from random import randint
from datetime import datetime
from AnalogSettingTab import QAnalogSettingTab
import urllib2
import re
import os
import glob

class QAnalogSetting(QtGui.QWidget):
    def __init__(self, analogValue, sizeValue = 1):
        QtGui.QWidget.__init__(self)

        # Section Mode
        # Create the Layout structure.
        self.sizeValue = sizeValue
        self.analogValue = analogValue
        
        self.AnalogSettingLayout = QtGui.QVBoxLayout(self)

        # Create the Selection Mode Section
        
        self.EnableCheck = QtGui.QCheckBox("Enable Interface")
        self.EnableCheck.stateChanged.connect(self.enableCheck)
        self.AnalogSettingLayout.addWidget(self.EnableCheck)
        
        self.Tabs = QAnalogSettingTab(analogValue, sizeValue)
        self.AnalogSettingLayout.addWidget(self.Tabs)

        if self.sizeValue > 1:
        
            AnalogJoinLabel = QtGui.QLabel("Function Join:")
            AnalogJoinLabel.setFixedWidth(65)
            self.AnalogJoinTextEdit = QtGui.QTextEdit("MATH_VECTOR3D_LENGHT")
            self.AnalogJoinTextEdit.setFixedHeight(23)
            self.AnalogJoinButton = QtGui.QPushButton()
            self.AnalogJoinButton.setIcon(QtGui.QIcon(QtGui.QPixmap("../images/info1.png")))
            self.AnalogJoinButton.clicked.connect(self.displayInfoJoin)
            self.JoinLayout = QtGui.QHBoxLayout(self) 
            self.JoinLayout.addWidget(AnalogJoinLabel)
            self.JoinLayout.addWidget(self.AnalogJoinTextEdit)
            self.JoinLayout.addWidget(self.AnalogJoinButton)
            self.AnalogSettingLayout.addLayout(self.JoinLayout)
            

        
        self.AnalogProcessorLabel = QtGui.QLabel("Processor:")
        self.AnalogProcessorLabel.setFixedWidth(65)
        self.AnalogAverageCheck = QtGui.QCheckBox("Moving Average")  
        self.AnalogAverageCheck.setChecked(True)
        self.AnalogAverageCheck.stateChanged.connect(self.movingAverageSetting)
        self.AnalogAverageSizeLabel = QtGui.QLabel("Size:")
        self.AverageSizeTextEdit = QtGui.QTextEdit("50")
        self.AverageSizeTextEdit.setFixedHeight(23)
        self.AnalogAverageOffsetLabel = QtGui.QLabel("Offset:")
        self.AverageOffsetTextEdit = QtGui.QTextEdit("120")
        self.AverageOffsetTextEdit.setFixedHeight(23)
        self.AnalogAverageOffsetSizeLabel = QtGui.QLabel("Number:")
        self.AverageOffsetSizeTextEdit = QtGui.QTextEdit("10")
        self.AverageOffsetSizeTextEdit.setFixedHeight(23)
        self.AnalogAverageButton = QtGui.QPushButton()
        self.AnalogAverageButton.setIcon(QtGui.QIcon(QtGui.QPixmap("../images/info1.png")))
        self.AnalogAverageButton.setFixedWidth(28)
        self.AnalogAverageButton.clicked.connect(self.displayInfoProcess)
        self.AverageLayout = QtGui.QHBoxLayout(self) 
        self.AverageLayout.addWidget(self.AnalogProcessorLabel)
        self.AverageLayout.addWidget(self.AnalogAverageCheck)
        self.AverageLayout.addWidget(self.AnalogAverageSizeLabel)
        self.AverageLayout.addWidget(self.AverageSizeTextEdit)
        self.AverageLayout.addWidget(self.AnalogAverageOffsetLabel)
        self.AverageLayout.addWidget(self.AverageOffsetTextEdit)
        self.AverageLayout.addWidget(self.AnalogAverageOffsetSizeLabel)
        self.AverageLayout.addWidget(self.AverageOffsetSizeTextEdit)
        self.AverageLayout.addWidget(self.AnalogAverageButton)
        self.AnalogSettingLayout.addLayout(self.AverageLayout)

                
        self.AnalogDetectionLabel = QtGui.QLabel("Detection:")
        self.AnalogDetectionLabel.setFixedWidth(65)
        self.AnalogDetectionTextEdit = QtGui.QTextEdit("detection"+analogValue+"Default")
        self.AnalogDetectionTextEdit.setFixedHeight(23)
        self.AnalogDetectionButton = QtGui.QPushButton()
        self.AnalogDetectionButton.setIcon(QtGui.QIcon(QtGui.QPixmap("../images/info1.png")))
        self.AnalogDetectionButton.clicked.connect(self.displayInfoDetect)
        self.DetectionLayout = QtGui.QHBoxLayout(self) 
        self.DetectionLayout.addWidget(self.AnalogDetectionLabel)
        self.DetectionLayout.addWidget(self.AnalogDetectionTextEdit)
        self.DetectionLayout.addWidget(self.AnalogDetectionButton)
        self.AnalogSettingLayout.addLayout(self.DetectionLayout)
        
        # Save all Layout.              
        self.setLayout(self.AnalogSettingLayout)

        self.EnableCheck.setChecked(False)
        self.enableCheck()
        
    def getValues(self):
        listReturn = []
        
        for item in self.AnalogSettingList:
            listItem = []
            listItem.append(item.getValues())
            listItem.append(item.getValues())
            listItem.append(item.getValues())
            listReturn.append(item.getValues())
            
        return listReturn

    def displayInfoJoin(self):
        msg = QtGui.QMessageBox()
        msg.setIcon(QtGui.QMessageBox.Information)
        msg.setInformativeText("Click show details for more information")
        msg.setWindowTitle("Join Parameters Information")
        if self.sizeValue > 1:
            if self.sizeValue == 2:
                msg.setText("Math formula to Join 2 signals\t\t\t\t\t\t")
                msg.setDetailedText("Macro List:\n\n - MATH_VECTOR2D_LENGHT: Vector 2D length.\n - MATH_AVERAGE2D: Average 2D. ")
            elif self.sizeValue == 3:
                msg.setText("Math formula to Join 3 signals\t\t\t\t\t\t")
                msg.setDetailedText("Macro List:\n\n - MATH_VECTOR3D_LENGHT: Vector 3D length.\n - MATH_AVERAGE3D: Average 3D. ")
            else:
                msg.setText("Math formula to Join more than 3 signals\t\t\t\t\t\t")
                msg.setDetailedText("Macro List:\n\n *** It must be developed by the user.***")
            
        msg.setStandardButtons(QtGui.QMessageBox.Ok)
        msg.exec_()


    def displayInfoProcess(self):
        msg = QtGui.QMessageBox()
        msg.setIcon(QtGui.QMessageBox.Information)
        msg.setInformativeText("Click show details for more information")
        msg.setWindowTitle("Processor Parameter Information")
        msg.setText("Math formula to process signal\t\t\t\t\t\t")
        msg.setDetailedText("Macro List:\n\n - MATH_MOVING_AVERAGE: Moving Average.")
        msg.setStandardButtons(QtGui.QMessageBox.Ok)
        msg.exec_()


    def movingAverageSetting(self):
        if self.AnalogAverageCheck.isChecked():
            self.AverageSizeTextEdit.setDisabled(False)
            self.AverageOffsetTextEdit.setDisabled(False)
            self.AverageOffsetSizeTextEdit.setDisabled(False)
        else:            
            self.AverageSizeTextEdit.setDisabled(True)
            self.AverageOffsetTextEdit.setDisabled(True)
            self.AverageOffsetSizeTextEdit.setDisabled(True)



    def displayInfoDetect(self):
        msg = QtGui.QMessageBox()
        msg.setIcon(QtGui.QMessageBox.Information)
        msg.setInformativeText("Click show details for more information")
        msg.setWindowTitle("Detector Parameter Information")
        msg.setText("Math formula to detect alert\t\t\t\t\t\t")
        msg.setDetailedText("Macro List:\n\n - detectionA1Default: Default detection for A1.\n - detectionA2Default: Default detection for A2.\n - detectionA3Default: Default detection for A3.\n - detectionA4Default: Default detection for A4.\n - detectionA5Default: Default detection for A5.\n")
        msg.setStandardButtons(QtGui.QMessageBox.Ok)
        msg.exec_()

    def enableCheck(self):
        if self.EnableCheck.isChecked():
            self.Tabs.enable()
            if self.sizeValue > 1:
                self.AnalogJoinTextEdit.setDisabled(False)
                self.AnalogJoinButton.setDisabled(False)
            self.AnalogAverageCheck.setDisabled(False)
            self.movingAverageSetting()
            self.AnalogAverageButton.setDisabled(False)
            self.AnalogDetectionTextEdit.setDisabled(False)
            self.AnalogDetectionButton.setDisabled(False)
        else:
            self.Tabs.disable()
            if self.sizeValue > 1:
                self.AnalogJoinTextEdit.setDisabled(True)
                self.AnalogJoinButton.setDisabled(True)
            self.AnalogAverageCheck.setDisabled(True)           
            self.AverageSizeTextEdit.setDisabled(True)
            self.AverageOffsetTextEdit.setDisabled(True)
            self.AverageOffsetSizeTextEdit.setDisabled(True)
            self.AnalogAverageButton.setDisabled(True)
            self.AnalogDetectionTextEdit.setDisabled(True)
            self.AnalogDetectionButton.setDisabled(True)
            
    def checkSettingItems(self):
        itemFail = 0
        if self.EnableCheck.isChecked() and self.AnalogAverageCheck.isChecked():
            if not re.match("^[0-9_]*$", self.AverageSizeTextEdit.toPlainText()):
                itemFail = itemFail + 1
                self.AnalogAverageSizeLabel.setText("<div style=\"color:#FF0000\">Size:<\div>")
            elif int(self.AverageSizeTextEdit.toPlainText()) > 255 or int(self.AverageSizeTextEdit.toPlainText()) < 0:
                itemFail = itemFail + 1
                self.AnalogAverageSizeLabel.setText("<div style=\"color:#FF0000\">Size:<\div>")
            else:
                self.AnalogAverageSizeLabel.setText("Size:")
                
            if not re.match("^[0-9_]*$", self.AverageOffsetTextEdit.toPlainText()):
                itemFail = itemFail + 1
                self.AnalogAverageOffsetLabel.setText("<div style=\"color:#FF0000\">Offset:<\div>")
            elif int(self.AverageOffsetTextEdit.toPlainText()) > 255 or int(self.AverageOffsetTextEdit.toPlainText()) < 0:
                itemFail = itemFail + 1
                self.AnalogAverageOffsetLabel.setText("<div style=\"color:#FF0000\">Offset:<\div>")
            else:
                self.AnalogAverageOffsetLabel.setText("Offset:")
                
            if not re.match("^[0-9_]*$", self.AverageOffsetSizeTextEdit.toPlainText()):
                itemFail = itemFail + 1
                self.AnalogAverageOffsetSizeLabel.setText("<div style=\"color:#FF0000\">Number:<\div>")
            elif int(self.AverageOffsetSizeTextEdit.toPlainText()) > 255 or int(self.AverageOffsetSizeTextEdit.toPlainText()) < 0:
                itemFail = itemFail + 1
                self.AnalogAverageOffsetSizeLabel.setText("<div style=\"color:#FF0000\">Number:<\div>")
            else:
                self.AnalogAverageOffsetSizeLabel.setText("Number:")

            if itemFail > 0:
                return False
            else:
                return True
        else:
            self.AnalogAverageOffsetSizeLabel.setText("Number:")
            self.AnalogAverageOffsetLabel.setText("Offset:")
            self.AnalogAverageSizeLabel.setText("Size:")
            return True

    def buildSetting(self):
        stringBuild = ""
        if self.EnableCheck.isChecked():
            stringBuild += "#define _EN_WIMOS_PORT_"+self.analogValue+" \n"
            stringBuild += str(self.Tabs.buildSetting())
            if self.sizeValue == 3:
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_OPERATOR_JOINT(x,y,z) \t MATH_VECTOR3D_LENGHT(x,y,z)\n"
            elif self.sizeValue == 2:
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_OPERATOR_JOINT(x,y) \t MATH_VECTOR2D_LENGHT(x,y)\n"
            else:                
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_OPERATOR_JOINT(x) \t (x)\n"

            if self.AnalogAverageCheck.isChecked():
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_AVERAGE_SIZE \t "+str(int(self.AverageSizeTextEdit.toPlainText()))+"\n"
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_AVERAGE_OFFSET  \t "+str(int(self.AverageOffsetTextEdit.toPlainText()))+"\n"
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_OFFSET_MAX_SIZE \t "+str(int(self.AverageOffsetSizeTextEdit.toPlainText()))+"\n"
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_PROCESSOR(lastValue, inputValue, currentIndex) \t MATH_MOVING_AVERAGE(lastValue, inputValue, currentIndex, _WIMOS_"+self.analogValue[::-1]+"_AVERAGE_SIZE)\n"
            else:
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_AVERAGE_SIZE \t 1\n"
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_AVERAGE_OFFSET  \t 0\n"
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_OFFSET_MAX_SIZE \t 0\n"
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_PROCESSOR(lastValue, inputValue, currentIndex) \t (inputValue)\n"
            
        else:
            stringBuild += "#undef _EN_WIMOS_PORT_"+self.analogValue+" \n"
            stringBuild += str(self.Tabs.buildSetting())
            
            if self.sizeValue == 3:
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_OPERATOR_JOINT(x,y,z) \t MATH_VECTOR3D_LENGHT(x,y,z)\n"
            elif self.sizeValue == 2:
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_OPERATOR_JOINT(x,y) \t MATH_VECTOR2D_LENGHT(x,y)\n"
            else:                
                stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_OPERATOR_JOINT(x) \t (x)\n"

            stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_AVERAGE_SIZE \t 1\n"
            stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_AVERAGE_OFFSET  \t 0\n"
            stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_OFFSET_MAX_SIZE \t 0\n"
            stringBuild += "#define _WIMOS_"+self.analogValue[::-1]+"_PROCESSOR(lastValue, inputValue, currentIndex) \t (inputValue)\n"

        return stringBuild










        

