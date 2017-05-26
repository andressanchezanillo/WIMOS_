import sys
from PyQt4 import QtCore, QtGui
from random import randint
from datetime import datetime
import urllib2
import os
import glob


class QAnalogValueSetting(QtGui.QWidget):
    def __init__(self):
        QtGui.QWidget.__init__(self)

        # Section Mode
        # Create the Layout structure.

        self.AnalogSettingTabLayout = QtGui.QVBoxLayout(self)

        # Create the Selection Mode Section
        
        AnalogOffset1Label = QtGui.QLabel("Offset 1:")
        AnalogOffset1Label.setFixedWidth(80)
        self.AnalogOffset1TextEdit = QtGui.QTextEdit("- VCC_LOGIC / 2")
        self.AnalogOffset1TextEdit.setFixedHeight(23)
        self.AnalogOffset1Button = QtGui.QPushButton()
        self.AnalogOffset1Button.setIcon(QtGui.QIcon(QtGui.QPixmap("../images/info1.png")))
        self.AnalogOffset1Button.clicked.connect(self.displayInfo)
        self.Offset1Layout = QtGui.QHBoxLayout(self)  
        self.Offset1Layout.addWidget(AnalogOffset1Label)
        self.Offset1Layout.addWidget(self.AnalogOffset1TextEdit)
        self.Offset1Layout.addWidget(self.AnalogOffset1Button)
        self.AnalogSettingTabLayout.addLayout(self.Offset1Layout)
        
        AnalogCoeficient1Label = QtGui.QLabel("Coeficient 1:")
        AnalogCoeficient1Label.setFixedWidth(80)
        self.AnalogCoeficient1TextEdit = QtGui.QTextEdit("GRAVITY_MM_S2/(VCC_LOGIC/5)")
        self.AnalogCoeficient1TextEdit.setFixedHeight(23)
        self.AnalogCoeficient1Button = QtGui.QPushButton()
        self.AnalogCoeficient1Button.setIcon(QtGui.QIcon(QtGui.QPixmap("../images/info1.png")))
        self.AnalogCoeficient1Button.clicked.connect(self.displayInfo)
        self.Coeficient1Layout = QtGui.QHBoxLayout(self)  
        self.Coeficient1Layout.addWidget(AnalogCoeficient1Label)
        self.Coeficient1Layout.addWidget(self.AnalogCoeficient1TextEdit)
        self.Coeficient1Layout.addWidget(self.AnalogCoeficient1Button)
        self.AnalogSettingTabLayout.addLayout(self.Coeficient1Layout)
        
        AnalogOffset2Label = QtGui.QLabel("Offset 2:")
        AnalogOffset2Label.setFixedWidth(80)
        self.AnalogOffset2TextEdit = QtGui.QTextEdit("NULL")
        self.AnalogOffset2TextEdit.setFixedHeight(23)
        self.AnalogOffset2Button = QtGui.QPushButton()
        self.AnalogOffset2Button.setIcon(QtGui.QIcon(QtGui.QPixmap("../images/info1.png")))
        self.AnalogOffset2Button.clicked.connect(self.displayInfo)
        self.Offset2Layout = QtGui.QHBoxLayout(self) 
        self.Offset2Layout.addWidget(AnalogOffset2Label)
        self.Offset2Layout.addWidget(self.AnalogOffset2TextEdit)
        self.Offset2Layout.addWidget(self.AnalogOffset2Button)
        self.AnalogSettingTabLayout.addLayout(self.Offset2Layout)
        
        AnalogCoeficient2Label = QtGui.QLabel("Coeficient 2:")
        AnalogCoeficient2Label.setFixedWidth(80)
        self.AnalogCoeficient2TextEdit = QtGui.QTextEdit("NULL")
        self.AnalogCoeficient2TextEdit.setFixedHeight(23)
        self.AnalogCoeficient2Button = QtGui.QPushButton()
        self.AnalogCoeficient2Button.setIcon(QtGui.QIcon(QtGui.QPixmap("../images/info1.png")))
        self.AnalogCoeficient2Button.clicked.connect(self.displayInfo)
        self.Coeficient2Layout = QtGui.QHBoxLayout(self)  
        self.Coeficient2Layout.addWidget(AnalogCoeficient2Label)
        self.Coeficient2Layout.addWidget(self.AnalogCoeficient2TextEdit)
        self.Coeficient2Layout.addWidget(self.AnalogCoeficient2Button)
        self.AnalogSettingTabLayout.addLayout(self.Coeficient2Layout)
        

        # Save all Layout.              
        self.setLayout(self.AnalogSettingTabLayout)
        
    def getValues(self):
        listReturn = []
        
        listReturn.append(self.AnalogOffset1TextEdit.toPlainText())
        listReturn.append(self.AnalogCoeficient1TextEdit.toPlainText())
        listReturn.append(self.AnalogOffset2TextEdit.toPlainText())
        listReturn.append(self.AnalogCoeficient2TextEdit.toPlainText())
        
        return listReturn

    def displayInfo(self):
        msg = QtGui.QMessageBox()
        msg.setIcon(QtGui.QMessageBox.Information)
        msg.setText("Analog Math Formula:\t\t\t\t\t\t\n\nFinalValue = ((((Value + Offset1) * Coeficient1) + Offset2) * Coeficient2)")
        msg.setInformativeText("Click show details for more information                                   ")
        msg.setWindowTitle("Parameter Information")
        msg.setDetailedText("Macro List:\n\n - VCC_LOGIC: System voltage logic in mV.\n - ADC_MAX_VALUE: Max value of 16 bits.\n -GRAVITY_MM_S2: Gravity constant in mm/s^2\n - NULL: Disable parameter.")
        msg.setStandardButtons(QtGui.QMessageBox.Ok)
        msg.exec_()

    def disable(self):
        self.AnalogOffset1TextEdit.setDisabled(True)
        self.AnalogOffset1Button.setDisabled(True)
        self.AnalogCoeficient1TextEdit.setDisabled(True)
        self.AnalogCoeficient1Button.setDisabled(True)
        self.AnalogOffset2TextEdit.setDisabled(True)
        self.AnalogOffset2Button.setDisabled(True)
        self.AnalogCoeficient2TextEdit.setDisabled(True)
        self.AnalogCoeficient2Button.setDisabled(True)

    def enable(self):        
        self.AnalogOffset1TextEdit.setDisabled(False)
        self.AnalogOffset1Button.setDisabled(False)
        self.AnalogCoeficient1TextEdit.setDisabled(False)
        self.AnalogCoeficient1Button.setDisabled(False)
        self.AnalogOffset2TextEdit.setDisabled(False)
        self.AnalogOffset2Button.setDisabled(False)
        self.AnalogCoeficient2TextEdit.setDisabled(False)
        self.AnalogCoeficient2Button.setDisabled(False)

    def buildSetting(self, analogName):
        stringBuild = ""
        if self.AnalogOffset1TextEdit.toPlainText() == "NULL":            
            stringBuild += "#define _WIMOS_"+analogName+"_OFFSET_1 \t (uint8_t)(0)\n"
        else:
            stringBuild += "#define _WIMOS_"+analogName+"_OFFSET_1 \t "+self.AnalogOffset1TextEdit.toPlainText()+"\n"            

        if self.AnalogCoeficient1TextEdit.toPlainText() == "NULL":            
            stringBuild += "#define _WIMOS_"+analogName+"_COEFICIENT_1 \t (uint8_t)(1)\n"
        else:
            stringBuild += "#define _WIMOS_"+analogName+"_COEFICIENT_1 \t "+self.AnalogCoeficient1TextEdit.toPlainText()+"\n"
        
        if self.AnalogOffset2TextEdit.toPlainText() == "NULL":            
            stringBuild += "#define _WIMOS_"+analogName+"_OFFSET_2 \t (uint8_t)(0)\n"
        else:
            stringBuild += "#define _WIMOS_"+analogName+"_OFFSET_2 \t "+self.AnalogOffset2TextEdit.toPlainText()+"\n"
        
        if self.AnalogCoeficient2TextEdit.toPlainText() == "NULL":            
            stringBuild += "#define _WIMOS_"+analogName+"_COEFICIENT_2 \t (uint8_t)(1)\n"
        else:
            stringBuild += "#define _WIMOS_"+analogName+"_COEFICIENT_2 \t "+self.AnalogCoeficient2TextEdit.toPlainText()+"\n"
            
        return stringBuild




        
        
