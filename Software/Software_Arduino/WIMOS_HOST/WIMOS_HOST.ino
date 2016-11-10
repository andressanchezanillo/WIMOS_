#include <Adafruit_GPS.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#define mySerialGPS Serial3
#define mySerialDebug SerialUSB
#define GET_DEBUG
#define GET_GPS
#define GET_IMU
#define SEND_XBEE
//#define GPSDEBUG

#ifdef SEND_XBEE
  #define mySerialXbee Serial2
#endif

#ifdef GET_IMU
  MPU6050 mpu(0x68);
  bool dmpReady = false;  // set true if DMP init was successful
  uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
  uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
  uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
  uint16_t fifoCount;     // count of all bytes currently in FIFO
  uint8_t fifoBuffer[64]; // FIFO storage buffer
  // orientation/motion vars
  Quaternion q;           // [w, x, y, z]         quaternion container
  VectorInt16 aa;         // [x, y, z]            accel sensor measurements
  VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
  VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float euler[3];         // [psi, theta, phi]    Euler angle container
  float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
#endif

#ifdef GET_GPS
  Adafruit_GPS GPS(&mySerialGPS);
#endif

void setup(){
  #ifdef SEND_XBEE
    mySerialXbee.begin(9600);
  #endif
  
  #ifdef GET_DEBUG
    mySerialDebug.begin(115200);
  #endif
  
  #ifdef GET_GPS
    GPS.begin(9600);
    mySerialGPS.begin(9600);
    
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
    // Request updates on antenna status, comment out to keep quiet
    GPS.sendCommand(PGCMD_ANTENNA);
    delay(1000);
    // Ask for firmware version
    mySerialGPS.println(PMTK_Q_RELEASE);
  #endif
  
  #ifdef GET_IMU
    Wire.begin();
    mpu.initialize();
    
    if(!mpu.testConnection()){
      mySerialDebug.println("MPU TEST ERROR");
    }
    
    devStatus = mpu.dmpInitialize();
    
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
    
      if (devStatus == 0) {
        mpu.setDMPEnabled(true);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        mySerialDebug.print(F("DMP Initialization failed."));
    }
    
  #endif
  
        
}


uint32_t timer = millis();
void loop(){
  GPS_data();
  IMU_data();
  Xbee_send_data("WIMOS TEST 07/01/2016");
  delay(10);
}

void GPS_data(){  
  #ifdef GET_GPS
    char c = GPS.read();
    #ifdef GPSDEBUG
      if (c) mySerialDebug.print(c);
    #endif
    
    if (GPS.newNMEAreceived()) {
      if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
        return;  // we can fail to parse a sentence in which case we should just wait for another
    }
  
    if (timer > millis())  timer = millis();
  
    if (millis() - timer > 2000) { 
      timer = millis(); // reset the timer
      
      mySerialDebug.print("\nTime: ");
      mySerialDebug.print(GPS.hour, DEC); mySerialDebug.print(':');
      mySerialDebug.print(GPS.minute, DEC); mySerialDebug.print(':');
      mySerialDebug.print(GPS.seconds, DEC); mySerialDebug.print('.');
      mySerialDebug.println(GPS.milliseconds);
      mySerialDebug.print("Date: ");
      mySerialDebug.print(GPS.day, DEC); mySerialDebug.print('/');
      mySerialDebug.print(GPS.month, DEC); mySerialDebug.print("/20");
      mySerialDebug.println(GPS.year, DEC);
      mySerialDebug.print("Fix: "); mySerialDebug.print((int)GPS.fix);
      mySerialDebug.print(" quality: "); mySerialDebug.println((int)GPS.fixquality); 
      if (GPS.fix) {
        mySerialDebug.print("Location: ");
        mySerialDebug.print(GPS.latitude, 4); mySerialDebug.print(GPS.lat);
        mySerialDebug.print(", "); 
        mySerialDebug.print(GPS.longitude, 4); mySerialDebug.println(GPS.lon);
        
        mySerialDebug.print("Speed (knots): "); mySerialDebug.println(GPS.speed);
        mySerialDebug.print("Angle: "); mySerialDebug.println(GPS.angle);
        mySerialDebug.print("Altitude: "); mySerialDebug.println(GPS.altitude);
        mySerialDebug.print("Satellites: "); mySerialDebug.println((int)GPS.satellites);
      }
    #endif
  }  
}

void IMU_data(){
    #ifdef GET_IMU
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        mySerialDebug.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
        
        // display real acceleration, adjusted to remove gravity
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        mySerialDebug.print("\t");
        mySerialDebug.print(aaReal.x);
        mySerialDebug.print("\t");
        mySerialDebug.print(aaReal.y);
        mySerialDebug.print("\t");
        mySerialDebug.print(aaReal.z);
        mySerialDebug.print("\t");
        mySerialDebug.print(ypr[0]);
        mySerialDebug.print("\t");
        mySerialDebug.print(ypr[1]);
        mySerialDebug.print("\t");
        mySerialDebug.println(ypr[2]);
    }  
  #endif
}

void Xbee_send_data(const String& msg){
  #ifdef SEND_XBEE
    mySerialXbee.println(msg);
  #endif
}


