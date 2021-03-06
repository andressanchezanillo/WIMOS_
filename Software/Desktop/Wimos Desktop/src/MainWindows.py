from PyQt4 import QtCore
from PyQt4 import QtGui
from NetworkWindows import QNetworkWindows
from UpdateWindows import QUpdateWindows
from SettingWindows import QSettingWindows
from random import randint
from datetime import datetime
import os
import time
import sys

class QMainWindows(QtGui.QWidget):
    def __init__(self):
        QtGui.QWidget.__init__(self)

        self.MainTabs = QtGui.QTabWidget()
        self.MainTabs.setTabPosition(QtGui.QTabWidget.West)

        self.NetworkMain = QNetworkWindows()
        self.NetworkMain.setStatusTip("Network view")

        self.SettingMain = QSettingWindows()
        self.SettingMain.setStatusTip("Setting view")

        self.UpdateMain = QUpdateWindows()
        self.UpdateMain.setStatusTip("Update view")
        
        self.MainTabs.addTab(self.NetworkMain,QtGui.QIcon('../images/network.png'),"Network")
        self.MainTabs.addTab(self.SettingMain,QtGui.QIcon('../images/setting.png'),"Settings")
        self.MainTabs.addTab(self.UpdateMain,QtGui.QIcon('../images/update.png'),"Update")

        self.layout = QtGui.QVBoxLayout(self)
        self.layout.addWidget(self.MainTabs)
        
        self.setLayout(self.layout)

        self.MainTabs.currentChanged.connect(self.SettingMain.updateReleaseList)

    def TestUTs(self):
        print("--- Unitaty Test Started ---")
        
        msg = QtGui.QMessageBox()
        msg.setIcon(QtGui.QMessageBox.Information)
        msg.setText("Unitary Tests are running.\n Please don't touch anything.")
        msg.setWindowTitle("Unitary Test")
        msg.setStandardButtons(QtGui.QMessageBox.Ok)
        msg.exec_()

        self.UpdateMain.TestUTs()
        self.SettingMain.TestUTs()
        self.NetworkMain.TestUTs()
        
        msg = QtGui.QMessageBox()
        msg.setIcon(QtGui.QMessageBox.Information)
        msg.setText("Unitary Tests has finished.")
        msg.setWindowTitle("Unitary Test Finished")
        msg.setStandardButtons(QtGui.QMessageBox.Ok)
        msg.exec_()

    def TestVT(self):
        print("--- Unitaty Test Started ---")
        
        msg = QtGui.QMessageBox()
        msg.setIcon(QtGui.QMessageBox.Information)
        msg.setText("Validation Tests are running.\n Please don't touch anything.")
        msg.setWindowTitle("Validation Test")
        msg.setStandardButtons(QtGui.QMessageBox.Ok)
        msg.exec_()
        
        self.UpdateMain.TestVT()
        self.SettingMain.TestVT()
        self.NetworkMain.TestVT()
        
        msg = QtGui.QMessageBox()
        msg.setIcon(QtGui.QMessageBox.Information)
        msg.setText("Validation Tests has finished.")
        msg.setWindowTitle("Validation Test Finished")
        msg.setStandardButtons(QtGui.QMessageBox.Ok)
        msg.exec_()

    
