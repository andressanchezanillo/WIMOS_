import sys
from PyQt4 import QtCore, QtGui
from NetworkItem import QNetworkItem

class QNetworkList (QtGui.QWidget):
    def __init__ (self, parent = None):
        super(QNetworkList, self).__init__(parent)
        self.NetworkList = QtGui.QListWidget(self)
        
    def addInfo(self, idDevice, infoDevice):
        varFound = False
        [typeD,batteryD,alertD] = self.processInfo(infoDevice)

        seachItem = self.NetworkList.findItems(" "+idDevice, QtCore.Qt.MatchRegExp)
        for item in seachItem:
            print "HOLA"
            if item.DeviceID is idDevice:
                item.addInfo(idDevice,typeD,batteryD,alertD)
                varFound = True

        if not varFound:
            print "HELLO"
            item = QNetworkItem()
            item.addInfo(idDevice,typeD,batteryD,alertD)
            myQListWidgetItem = QtGui.QListWidgetItem(self.NetworkList)
            myQListWidgetItem.setSizeHint(item.sizeHint())
            self.NetworkList.addItem(myQListWidgetItem)
            self.NetworkList.setItemWidget(myQListWidgetItem, item)
            
    def processInfo(self, infoD):
        return infoD.split(" ")
            
class exampleQMainWindow (QtGui.QWidget):
    def __init__ (self):
        super(exampleQMainWindow, self).__init__()

        # Create QListWidget
        self.label = QtGui.QLabel()
        self.label.setText("Hola")
        self.InfoLayout = QtGui.QVBoxLayout()  
        self.list = QNetworkList()
        self.button = QtGui.QPushButton()
        
        self.InfoLayout.addWidget(self.label)
        self.InfoLayout.addWidget(self.list)
        self.InfoLayout.addWidget(self.button)
        self.setLayout(self.InfoLayout)
        self.button.clicked.connect(self.sendInfo)

    def sendInfo(self):
        self.list.addInfo('1','Center 25 low')

app = QtGui.QApplication([])
window = exampleQMainWindow()
window.show()
sys.exit(app.exec_())
