import sys
from PyQt4 import QtCore, QtGui
from random import randint
from datetime import datetime
import urllib2
import os


class QUpdateWindows(QtGui.QWidget):
    def __init__ (self):
        super(QUpdateWindows, self).__init__()
        
        self.UpdateLayout = QtGui.QVBoxLayout(self)
        self.ButtonUpdate = QtGui.QPushButton("Download")
        self.ReleaseListItems = []

        try:
            ReleaseList = urllib2.urlopen('https://raw.githubusercontent.com/andressanchezanillo/WIMOS_/master/Releases/reg.ini')
            fileList = ReleaseList.read()

            for releaseItem in fileList.split("\n"):
                infoRelease = releaseItem.split(";")
                if infoRelease[0] == "Release":
                    self.ReleaseListItems.append([[infoRelease[0],infoRelease[1],infoRelease[2],infoRelease[3]],infoRelease[4]])

        
        except urllib2.URLError, e:
            msg = QtGui.QMessageBox()
            msg.setIcon(QtGui.QMessageBox.Critical)
            msg.setInformativeText("Click show details for more information")
            msg.setWindowTitle("HTTP connection error")
            msg.setText("Check you connection and restart the app\t\t.")
            msg.setDetailedText("Your system has lost the Internet connection, it lose these services:\n \t- Auto Update Releases. \n\t- GPS map displayment.")
            msg.setStandardButtons(QtGui.QMessageBox.Ok)
            msg.exec_()


            
        model = QtGui.QStandardItemModel()
        self.layout = QtGui.QVBoxLayout(self)

        for releaseItem in self.ReleaseListItems:                  
            item = QtGui.QStandardItem(releaseItem[0][3]+"\t\tWIMOS_"+releaseItem[0][1]+"\t\t\t"+releaseItem[0][2])
            item.setCheckable(True)
            item.setEditable(False)
            model.appendRow(item)


        self.view = QtGui.QListView()
        self.view.setModel(model)
                           
        self.UpdateLayout.addWidget(self.view)
        self.UpdateLayout.addWidget(self.ButtonUpdate)
        self.setLayout(self.UpdateLayout)

        self.path = "workspace/releases/"

        self.ButtonUpdate.clicked.connect(self.downloadRelease)

    def downloadRelease(self):
        downloadList = []
        model = self.view.model()
        for row in range(model.rowCount()):
            item = model.item(row)
            if item.checkState() == QtCore.Qt.Checked:                
                downloadList.append(self.ReleaseListItems[row][-1])

        if not os.path.exists(self.path):
            os.makedirs(self.path)
            
        for itemDownload in downloadList:                      
            
            url = itemDownload
            fileName = url.split("/")[-1].split(".rar")[0]
            fileName += ".rar"
            fileName = self.path+fileName
            
            ReleaseFile = urllib2.urlopen(url)
            data = ReleaseFile.read()
            with open(fileName, "wb") as code:
                code.write(data)
        
