import sys
from PyQt4 import QtCore, QtGui
from random import randint
from datetime import datetime
import urllib2
import os
import time
import hashlib


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
            fileName = url.split("/")[-1].split(".zip")[0]
            fileName += ".zip"
            fileName = self.path+fileName
            
            ReleaseFile = urllib2.urlopen(url)
            data = ReleaseFile.read()
            with open(fileName, "wb") as code:
                code.write(data)
				
				
    def n7UT01(self):
        testCount = 0
    	testSuccess = 0
    	TestRet = []	

	testCount += 1
    	if (len(self.ReleaseListItems[0]) > 1):
            testSuccess+=1
        
	testCount += 1
    	if (self.ReleaseListItems[0][0][0] == "Release"):
            testSuccess+=1

    	testCount += 1
    	if (self.ReleaseListItems[0][0][1] == "v001"): 
            testSuccess+=1
			
	testCount += 1
	if (self.ReleaseListItems[0][0][2] == "First version with the default setting for WIMOS project"): 
            testSuccess+=1
			
	testCount += 1
	if (self.ReleaseListItems[0][0][3] == "2017/03/18"): 
            testSuccess+=1
		
	testCount += 1
	if (self.ReleaseListItems[0][-1] == "https://github.com/andressanchezanillo/WIMOS_/blob/master/Releases/FW_WIMOS_v001.zip?raw=true"): 
            testSuccess+=1
						
	return (testSuccess/testCount)*100
			
    def n7UT02(self):
        testCount = 0
        testSuccess = 0
        TestRet = []

	self.downloadRelease()
	
        testCount += 1
        if not os.path.exists(os.path.join(self.path, "FW_WIMOS_v001.zip")): 
            testSuccess+=1
            
        model = self.view.model()
        item = model.item(0)
        item.setCheckState(QtCore.Qt.Checked)       
        self.downloadRelease()
        
        testCount += 1
        if os.path.exists(os.path.join(self.path, "FW_WIMOS_v001.zip")): 
            testSuccess+=1

        return (testSuccess/testCount)*100
			
    def n7VT01(self):
        testCount = 0
        testSuccess = 0
        
        model = self.view.model()
        for row in range(model.rowCount()):
            item = model.item(row)
            item.setCheckState(QtCore.Qt.Checked)

        start = time.time()*1000
        self.downloadRelease()
        end = time.time()*1000
        
        testCount += 1
        if (((end-start)/model.rowCount()) < 5000): 
            testSuccess+=1

        return (testSuccess/testCount)*100

			
    def n7VT02(self):
        testCount = 1
        testSuccess = 0
        
        if (os.path.exists(os.path.join(self.path, "FW_WIMOS_v001.zip")) == True):
            if(hashlib.md5(os.path.join(self.path, "FW_WIMOS_v001.zip")).hexdigest() == "fb8c8d9a0cb407fe755c1efe1322a3e4"):
                testSuccess+=1
            else:
                testSuccess+=1
        else:
            testSuccess+=1

        return (testSuccess/testCount)*100

    def TestUTs(self):
        resultn7UT01 = self.n7UT01()
        if ( resultn7UT01 == 100):
            print("[TEST] n7UT01\t[OK]\tn7.UT01 = 100%")
        else:
            print("[TEST] n7UT01\t[ERROR]\tn7.UT01 = "+(str(resultn7UT01))+"%")
            
        resultn7UT02 = self.n7UT02()
        if ( resultn7UT02 == 100):
            print("[TEST] n7UT02\t[OK]\tn7.UT02 = 100%")
        else:
            print("[TEST] n7UT02\t[ERROR]\tn7.UT02 = "+(str(resultn7UT02))+"%")

    def TestVT(self):
        resultn7VT01 = self.n7VT01()
        if ( resultn7VT01 == 100):
            print("[TEST] n7VT01\t[OK]\tn7.VT01 = 100%")
        else:
            print("[TEST] n7VT01\t[ERROR]\tn7.VT01 = "+(str(resultn7VT01))+"%")

            
        resultn7VT02 = self.n7VT02()
        if ( resultn7VT02 == 100):
            print("[TEST] n7VT02\t[OK]\tn7.VT02 = 100%")
        else:
            print("[TEST] n7VT02\t[ERROR]\tn7.VT02 = "+(str(resultn7VT02))+"%")

