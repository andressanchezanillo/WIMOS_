# -*- coding: cp1252 -*-
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtWebKit import *
from NetworkList import QNetworkList
from random import randint
from datetime import datetime
import sys
import os

maphtml = '''
<!DOCTYPE html>
<html>

<body>

<div id="map" style="height:410px;width:100%"></div>

<script>

var listMark = [];
var listCircle = [];
var GlobalMap = {};

function myMap() {
	var mapCanvas = document.getElementById("map");
	var myCenter=new google.maps.LatLng(39.75, -5.2);
	var mapOptions = {
		center: myCenter, 
		zoom: 6, 
		mapTypeId: google.maps.MapTypeId.HYBRID,
		disableDefaultUI: true
	};
	var map = new google.maps.Map(mapCanvas, mapOptions);
	GlobalMap = map;
}

function addInfo(_id, _latitude, _longitude, _info) {
	var station = {};
	var i;
	var location=new google.maps.LatLng(_latitude,_longitude);  
	var marker = new google.maps.Marker({
		position: location,
		map: GlobalMap
	});

	for (i=0;i<listMark.length;i++){
		station = listMark[i];
		if (station.id === _id){
			station.mark.setMap(null);
			station.mark = null;
			station.mark = marker;
			station.info = _info;
			station.mark.setMap(GlobalMap);
			if (station.alert != null){
                            station.alert.setMap(null);
                            station.alert.setCenter(location);
                            station.alert.setMap(GlobalMap);
                        
			}
			return;
		}
	}

	station.id = _id;
	station.mark = marker;
	station.info = _info;
	marker.setMap(GlobalMap);
	listMark.push(station)      
	return;  
}

function addAlert(_id, _level, _radius) {
	var station = {};
	var i;
	
	for (i=0;i<listMark.length;i++){
		station = listMark[i];
		if (station.id === _id){
			var location=station.mark.getPosition();  
			var Alert = new google.maps.Circle({
				center: station.mark.getPosition(),
				radius:_radius,
				strokeColor:_level,
				strokeOpacity:0.8,
				strokeWeight:2,
				fillColor:_level,
				fillOpacity:0.4
			});
			if (station.alert != null){
				station.alert.setMap(null);
			}
			station.alert = Alert;
			station.alert.setMap(GlobalMap);
            listMark[i] = station;
			return;
		}
	}
	
	
    
	return;  
}

function eraseMark(_id) {
	var i;
	var station = {};
	for (i=0;i<listMark.length;i++){
		station = listMark[i];
		if (station.id === _id){
			station.mark.setMap(null);
			return;
		}
	}
}

function eraseAlert(_id) {
	var i;
	var station = {};
	for (i=0;i<listMark.length;i++){
		station = listMark[i];
		if (station.id === _id){
			station.alert.setMap(null);
			return;
		}
	}
}


</script>

 <script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyAluSktgm5aAVFZqHRfmvSYSYgkdy4OHHQ&callback=myMap"></script>
<!--
To use this code on your website, get a free API key from Google.
Read more at: https://www.w3schools.com/graphics/google_maps_basic.asp
-->

</body>
</html>

'''
# <script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyAluSktgm5aAVFZqHRfmvSYSYgkdy4OHHQ&callback=myMap"></script>
class QNetworkGPS(QWidget):
    def __init__(self):
        QWidget.__init__(self)

        self.web = QWebView(self)
        self.web.setFixedWidth(600)
        self.FrameGPS = self.web.page().mainFrame()
        self.FrameGPS.addToJavaScriptWindowObject('self', self)
        self.web.setHtml(maphtml)

        self.layout = QVBoxLayout(self)
        self.layout.addWidget(self.web)
        
        self.setLayout(self.layout)

    def addInfo(self, idDevice, latitudeDevice, longitudeDevice, infoDevice):
        self.FrameGPS.evaluateJavaScript("addInfo("+str(idDevice)+","+latitudeDevice+","+longitudeDevice+",\""+infoDevice+"\")")
        
    def addAlert(self, idDevice, alertLevel):        
        if (alertLevel is 'none'):
            self.FrameGPS.evaluateJavaScript("eraseAlert("+str(idDevice)+")")
        elif (alertLevel is 'low'):
            self.FrameGPS.evaluateJavaScript("addAlert("+str(idDevice)+",\"#01DF01\",1000)")
        elif (alertLevel is 'medium'):
            self.FrameGPS.evaluateJavaScript("addAlert("+str(idDevice)+",\"#FFBF00\",2000)")
        elif (alertLevel is 'high'):
            self.FrameGPS.evaluateJavaScript("addAlert("+str(idDevice)+",\"#DF0101\",4000)")
        
       
        
    def disconnect(self, idDevice):        
        self.FrameGPS.evaluateJavaScript("eraseAlert("+str(idDevice)+")")
        self.FrameGPS.evaluateJavaScript("eraseMark("+str(idDevice)+")")
                
