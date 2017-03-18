import folium
import sys
from MainWindows import QMainWindows
from PyQt4.QtGui import *
from PyQt4.QtCore import *


class QMainWimosWindows(QMainWindow):
    
    def __init__(self):
        super(QMainWimosWindows, self).__init__()
        self.setWindowTitle(self.tr('Wimos Desktop 1.0.1'))
        self.setWindowIcon(QIcon("../images/wimosApp.png"))   
        self.setMinimumSize(1000,550)

        #Central Windows - NetWork
        self.MainWindows = QMainWindows()
        self.setCentralWidget(self.MainWindows)
        
        
        
        #QAction - File
        self.aNew = QAction("New",self)
        self.aNew.setIcon(QIcon('../images/new.ico'))
        self.aNew.setShortcut("Ctrl+N")
        self.aNew.setStatusTip("Create a new setting")
                
        self.aLoad = QAction("Load...",self)
        self.aLoad.setIcon(QIcon('../images/load.ico'))
        self.aLoad.setShortcut("Ctrl+L")
        self.aLoad.setStatusTip("Load a setting file")
                
        self.aSave = QAction("Save...",self)
        self.aSave.setIcon(QIcon('../images/save.ico'))
        self.aSave.setShortcut("Ctrl+S")
        self.aSave.setStatusTip("Save the setting file")
            
        self.aSaveAs = QAction("Save as...",self)
        self.aSaveAs.setShortcut("Ctrl+Shift+S")
        self.aSaveAs.setStatusTip("Save the current setting as...")
        
        self.aExit = QAction("Exit",self)
        self.aExit.setIcon(QIcon('../images/exit.ico'))
        self.aExit.setShortcut("Ctrl+Q")
        self.aExit.setStatusTip("Close the program")


        #QAction - Edit
                
        self.aCut = QAction("Cut",self)
        self.aCut.setShortcut("Ctrl+X")
        self.aCut.setStatusTip("Cut content")

        self.aCopy = QAction("Copy",self)
        self.aCopy.setShortcut("Ctrl+C")
        self.aCopy.setStatusTip("Copy content")
        
        self.aClear = QAction("Clear",self)
        self.aClear.setShortcut("Ctrl+E")
        self.aClear.setStatusTip("Erase content")


        #QAction - Tool
                
        self.aNetwork = QAction("Network",self)
        self.aNetwork.setIcon(QIcon('../images/network.ico'))
        self.aNetwork.setStatusTip("Go to Network Tab")
        
        self.aSetting = QAction("Setting Module",self)
        self.aSetting.setIcon(QIcon("../images/setting.ico"))
        self.aSetting.setStatusTip("Go to Setting Tab")
        
        self.aUpdate = QAction("Update",self)
        self.aUpdate.setIcon(QIcon('../images/update.ico'))
        self.aUpdate.setStatusTip("Go to Update Tab")
        
        self.aPreferences = QAction("Preferences",self)
        self.aPreferences.setStatusTip("Tool preferences")

        
        #QAction - Help
                
        self.aAbout = QAction("About",self)
        self.aAbout.setStatusTip("Tool Information")


        #Status Bar
        self.setStatusBar(QStatusBar())

        #Tool Bar
        #self.toolBar = QToolBar(self)
        #self.toolBar.setIconSize(QSize(40,40))
        #self.toolBar.layout().setSpacing(10)
        #self.toolBar.setMovable(False)
        
        #self.toolBar.addAction(self.aNetwork)
        #self.toolBar.addAction(self.aSetting)
        #self.toolBar.addAction(self.aUpdate)
        #self.addToolBar(Qt.LeftToolBarArea, self.toolBar)
        
        #Menu
        menu = self.menuBar()

        mFile = menu.addMenu("&File")
        mFile.addAction(self.aNew)
        mFile.addAction(self.aLoad)
        mFile.addAction(self.aSave)
        mFile.addAction(self.aSaveAs)
        mFile.addAction(self.aExit)
        
        mEdit = menu.addMenu("&Edit")
        mEdit.addAction(self.aCut)
        mEdit.addAction(self.aCopy)
        mEdit.addAction(self.aClear)

        mTool = menu.addMenu("&Tool")
        mTool.addAction(self.aNetwork)
        mTool.addAction(self.aSetting)
        mTool.addAction(self.aUpdate)
        mTool.addAction(self.aPreferences)
        
        mHelp = menu.addMenu("&Help")
        mHelp.addAction(self.aAbout)

def main():
    import sys
    app = QApplication(sys.argv)
    w = QMainWimosWindows()
    w.show()
    
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
