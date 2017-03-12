import sys
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtWebKit import *
from NetworkList import QNetworkList
from random import randint
from datetime import datetime
import os

maphtml = '''
<!DOCTYPE html>
<html>
  <head>
    <style>
      html, body, #map-canvas {
        height: 100%;
        margin: 0px;
        padding: 0px
      }
    </style>
    <script src="https://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false&libraries=drawing"></script>
    <script>
      function initialize() {
        var mapOptions = {
          center: new google.maps.LatLng(39.75, -5.2),
          zoom: 6,
          mapTypeId: google.maps.MapTypeId.ROADMAP
        };

        var map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions);

      }

      google.maps.event.addDomListener(window, 'load', initialize);

    </script>
  </head>
  <body>
    <div id="map-canvas"></div>
  </body>
</html>
'''

class QNetworkGPS(QWidget):
    def __init__(self):
        QWidget.__init__(self)

        self.web = QWebView(self)
        self.web.setFixedWidth(600)
        self.web.page().mainFrame().addToJavaScriptWindowObject('self', self)
        self.web.setHtml(maphtml)

        self.layout = QVBoxLayout(self)
        self.layout.addWidget(self.web)
        
        self.setLayout(self.layout)

    def setMarkGPS(_latitude, _longitude, _info):
        print "Set Mark"
        #Send to Javascript Markset

    def setCenterGPS(_latitude, _longitude):
        print "Set Center"
        #Send to Javascript center
        


class exampleQMainWindow (QWidget):
    def __init__ (self):
        super(exampleQMainWindow, self).__init__()

        # Create QListWidget
                
        self.InfoLayout = QVBoxLayout(self)  
        self.setLayout(self.InfoLayout)
        
        self.list = QNetworkGPS()
        
        self.InfoLayout.addWidget(self.list)
        
        self.button = QPushButton()
        self.InfoLayout.addWidget(self.button)
        #self.button.clicked.connect(self.sendInfo)

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

app = QApplication([])
window = exampleQMainWindow()
window.show()
sys.exit(app.exec_())
