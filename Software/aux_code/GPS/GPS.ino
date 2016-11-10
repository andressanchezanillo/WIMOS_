// Test code for Adafruit GPS modules using MTK3329/MTK3339 driver
//
// This code shows how to listen to the GPS module in an interrupt
// which allows the program to have more 'freedom' - just parse
// when a new NMEA sentence is available! Then access data when
// desired.
//
// Tested and works great with the Adafruit Ultimate GPS module
// using MTK33x9 chipset
//    ------> http://www.adafruit.com/products/746
// Pick one up today at the Adafruit electronics shop 
// and help support open source hardware & software! -ada

#include <Adafruit_GPS.h>

#define mySerialGPS Serial3

Adafruit_GPS GPS(&mySerialGPS);


// Set GPSDEBUG to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences. 
#define GPSDEBUG  false

void setup()  
{
    
  
  SerialUSB.begin(115200);
  SerialUSB.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  mySerialGPS.begin(9600);
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!

  delay(1000);
  // Ask for firmware version
  mySerialGPS.println(PMTK_Q_RELEASE);
}

uint32_t timer = millis();
void loop()                     // run over and over again
{
  // in case you are not using the interrupt above, you'll
  // need to 'hand query' the GPS, not suggested :(
  if (! false) {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    #ifdef GPSDEBUG
      if (c) SerialUSB.print(c);
    #endif
  }
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //SerialUSB.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) { 
    timer = millis(); // reset the timer
    
    SerialUSB.print("\nTime: ");
    SerialUSB.print(GPS.hour, DEC); SerialUSB.print(':');
    SerialUSB.print(GPS.minute, DEC); SerialUSB.print(':');
    SerialUSB.print(GPS.seconds, DEC); SerialUSB.print('.');
    SerialUSB.println(GPS.milliseconds);
    SerialUSB.print("Date: ");
    SerialUSB.print(GPS.day, DEC); SerialUSB.print('/');
    SerialUSB.print(GPS.month, DEC); SerialUSB.print("/20");
    SerialUSB.println(GPS.year, DEC);
    SerialUSB.print("Fix: "); SerialUSB.print((int)GPS.fix);
    SerialUSB.print(" quality: "); SerialUSB.println((int)GPS.fixquality); 
    if (GPS.fix) {
      SerialUSB.print("Location: ");
      SerialUSB.print(GPS.latitude, 4); SerialUSB.print(GPS.lat);
      SerialUSB.print(", "); 
      SerialUSB.print(GPS.longitude, 4); SerialUSB.println(GPS.lon);
      
      SerialUSB.print("Speed (knots): "); SerialUSB.println(GPS.speed);
      SerialUSB.print("Angle: "); SerialUSB.println(GPS.angle);
      SerialUSB.print("Altitude: "); SerialUSB.println(GPS.altitude);
      SerialUSB.print("Satellites: "); SerialUSB.println((int)GPS.satellites);
    }
  }
}
