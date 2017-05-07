import sys
from PyQt4 import QtCore, QtGui
from random import randint
from datetime import datetime
from AnalogSettingTab import QAnalogSettingTab
import urllib2
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
        self.Tabs = QAnalogSettingTab(analogValue, sizeValue)
        self.AnalogSettingLayout.addWidget(self.Tabs)

        if sizeValue > 1:
        
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
            

        
        AnalogProcessorLabel = QtGui.QLabel("Processor:")
        AnalogProcessorLabel.setFixedWidth(65)
        self.AnalogAverageCheck = QtGui.QCheckBox("Moving Average")  
        self.AnalogAverageCheck.setChecked(True)
        self.AnalogAverageCheck.stateChanged.connect(self.movingAverageSetting)
        AnalogAverageSizeLabel = QtGui.QLabel("Size:")
        self.AverageSizeTextEdit = QtGui.QTextEdit("50")
        self.AverageSizeTextEdit.setFixedHeight(23)
        AnalogAverageOffsetLabel = QtGui.QLabel("Offset:")
        self.AverageOffsetTextEdit = QtGui.QTextEdit("120")
        self.AverageOffsetTextEdit.setFixedHeight(23)
        AnalogAverageOffsetSizeLabel = QtGui.QLabel("Number:")
        self.AverageOffsetSizeTextEdit = QtGui.QTextEdit("10")
        self.AverageOffsetSizeTextEdit.setFixedHeight(23)
        self.AnalogAverageButton = QtGui.QPushButton()
        self.AnalogAverageButton.setIcon(QtGui.QIcon(QtGui.QPixmap("../images/info1.png")))
        self.AnalogAverageButton.setFixedWidth(28)
        self.AnalogAverageButton.clicked.connect(self.displayInfoProcess)
        self.AverageLayout = QtGui.QHBoxLayout(self) 
        self.AverageLayout.addWidget(AnalogProcessorLabel)
        self.AverageLayout.addWidget(self.AnalogAverageCheck)
        self.AverageLayout.addWidget(AnalogAverageSizeLabel)
        self.AverageLayout.addWidget(self.AverageSizeTextEdit)
        self.AverageLayout.addWidget(AnalogAverageOffsetLabel)
        self.AverageLayout.addWidget(self.AverageOffsetTextEdit)
        self.AverageLayout.addWidget(AnalogAverageOffsetSizeLabel)
        self.AverageLayout.addWidget(self.AverageOffsetSizeTextEdit)
        self.AverageLayout.addWidget(self.AnalogAverageButton)
        self.AnalogSettingLayout.addLayout(self.AverageLayout)

                
        AnalogDetectionLabel = QtGui.QLabel("Detection:")
        AnalogDetectionLabel.setFixedWidth(65)
        self.AnalogDetectionTextEdit = QtGui.QTextEdit("detection"+analogValue+"Default")
        self.AnalogDetectionTextEdit.setFixedHeight(23)
        self.AnalogDetectionButton = QtGui.QPushButton()
        self.AnalogDetectionButton.setIcon(QtGui.QIcon(QtGui.QPixmap("../images/info1.png")))
        self.AnalogDetectionButton.clicked.connect(self.displayInfoDetect)
        self.DetectionLayout = QtGui.QHBoxLayout(self) 
        self.DetectionLayout.addWidget(AnalogDetectionLabel)
        self.DetectionLayout.addWidget(self.AnalogDetectionTextEdit)
        self.DetectionLayout.addWidget(self.AnalogDetectionButton)
        self.AnalogSettingLayout.addLayout(self.DetectionLayout)
        
        # Save all Layout.              
        self.setLayout(self.AnalogSettingLayout)
        
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



class exampleQMainWindow (QtGui.QWidget):
    def __init__ (self):
        super(exampleQMainWindow, self).__init__()

        # Create QListWidget
                
        self.InfoLayout = QtGui.QVBoxLayout(self)  
        self.setLayout(self.InfoLayout)
        
        self.list = QAnalogSetting("A5",3)
        
        self.InfoLayout.addWidget(self.list)
        
        self.button = QtGui.QPushButton()
        self.InfoLayout.addWidget(self.button)
        self.button.clicked.connect(self.list.getValues)

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
