from PyQt4 import QtCore
from PyQt4 import QtGui
from NetworkWindows import QNetworkWindows
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

        self.SettingMain = QtGui.QWidget()
        self.SettingMain.setStatusTip("Setting view")

        self.UpdateMain = QtGui.QWidget()
        self.UpdateMain.setStatusTip("Update view")
        
        self.MainTabs.addTab(self.NetworkMain,QtGui.QIcon('../images/network.png'),"Network")
        self.MainTabs.addTab(self.SettingMain,QtGui.QIcon('../images/setting.png'),"Settings")
        self.MainTabs.addTab(self.UpdateMain,QtGui.QIcon('../images/update.png'),"Update")

        self.layout = QtGui.QVBoxLayout(self)
        self.layout.addWidget(self.MainTabs)
        
        self.setLayout(self.layout)
