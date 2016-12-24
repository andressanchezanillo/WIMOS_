/****************************************************************************
 * Copyright (C) 2015 by Andrés Sánchez Anillo                              *
 *                                                                          *
 * This file is part of Box.                                                *
 *                                                                          *
 *   Box is free software: you can redistribute it and/or modify it         *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Box is distributed in the hope that it will be useful,                 *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Box.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

/**
 * @file wimos_imu.ino
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main function for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */

#include "_setting.h"
#include "main_config.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

MPU6050 mpu(0x68);

// MPU control/status vars
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

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };


volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

/***************************************************************************************
 * NO Compatible Wire.h con la librería VGA.
****************************************************************************************/
/**
 * @brief IMU Initialization.
 *
 * This function initializes the IMU interface.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
extern void initIMU(void){
  #ifdef _EN_WIMOS_IMU
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

    mpu.initialize();

    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        mpu.setDMPEnabled(true);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
      stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus |= WIMOS_DEVICE_IMU_MASK;
      DEBUG_OK("Internal IMU initialized.");
    }else{
      stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus &= ~WIMOS_DEVICE_IMU_MASK;
      DEBUG_ERROR("Internal IMU not initialized.");
      
    }
  #else    
    stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus &= ~WIMOS_DEVICE_IMU_MASK;
    DEBUG_INFO("Internal IMU not initialized.");
  #endif
}


/**
 * @brief IMU reader.
 *
 * This function reads the internal IMU value.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return _stWimosInfo It returns the IMU updated into stWimosInfo struct .
 */
extern void readIMU(stWimosPortValues* stWimosPort){
  #ifdef _EN_WIMOS_IMU
    DEBUG_INFO("Reading IMU values.");

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        DEBUG_INFO("FIFO overflow!");

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        if (fifoCount >= packetSize){

          // read a packet from FIFO
          mpu.getFIFOBytes(fifoBuffer, packetSize);
          
          // track FIFO count here in case there is > 1 packet available
          // (this lets us immediately read more without waiting for an interrupt)
          fifoCount -= packetSize;
        
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
            
            DEBUG_DATA("IMU AccX = %d",aaWorld.x);
            DEBUG_DATA("IMU AccY = %d",aaWorld.y);
            DEBUG_DATA("IMU AccZ = %d",aaWorld.z);
            DEBUG_DATA("IMU AccMod = %f",sqrt((aaWorld.x*aaWorld.x)+(aaWorld.y*aaWorld.y)+(aaWorld.z*aaWorld.z)));
            DEBUG_DATA("IMU GyrX = %f",ypr[1] * 180/M_PI);
            DEBUG_DATA("IMU GyrY = %f",ypr[2] * 180/M_PI);
            DEBUG_DATA("IMU GyrZ = %f",ypr[0] * 180/M_PI);
            
            stWimosPort->usInternalIMUAcc = (uint32_t) sqrt((aaWorld.x*aaWorld.x)+(aaWorld.y*aaWorld.y)+(aaWorld.z*aaWorld.z)/4);
            stWimosPort->usInternalIMUGyrosX = (int8_t) (ypr[1] * 180/M_PI);
            stWimosPort->usInternalIMUGyrosY = (int8_t) (ypr[2] * 180/M_PI);
        }
    }
  #endif
}



#ifdef WIMOS_DEBUG
  #ifdef WIMOS_UNIT_TEST
    
    /**
     * @brief Wimos test n3.UT25.
     *
     * Unit test n3.UT25 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n3UT25 (void){
      const char* testName = "n3.UT25 = %d";
      /*Body_TEST:*/
      
      #ifdef _EN_WIMOS_IMU
        stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus = 0;
        initIMU();
        DEBUG_VALID(testName , 
                   (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_IMU_MASK), 
                   (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_IMU_MASK) == WIMOS_DEVICE_IMU_MASK);
      #else
        stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus = WIMOS_DEVICE_RF_MASK;
        initIMU();
        DEBUG_VALID(testName , 
                   (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_IMU_MASK), 
                   (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_IMU_MASK) == 0x00);
      #endif
      delay(100);
      /*End_Body_TEST:*/
    } 

    /**
     * @brief Wimos test n3.UT26.
     *
     * Unit test n3.UT26 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n3UT26 (void){
      const char* testName = "n3.UT26 = %d";
      /*Body_TEST:*/     
        
      stGlobalWimosPortMsg.stPortValues.usInternalIMUAcc = 0;
      readIMU(&stGlobalWimosPortMsg.stPortValues);
      
      #ifdef _EN_WIMOS_IMU
        DEBUG_VALID(testName , 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUAcc), 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUAcc != 0));
      #else      
        
        DEBUG_VALID(testName , 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUAcc), 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUAcc == 0));
      #endif
      delay(10);
      /*End_Body_TEST:*/
    } 

    /**
     * @brief Wimos test n3.UT27.
     *
     * Unit test n3.UT27 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n3UT27 (void){
      const char* testName = "n3.UT27 = %d";
      /*Body_TEST:*/     
      
      stGlobalWimosPortMsg.stPortValues.usInternalIMUGyrosX = 0;
      readIMU(&stGlobalWimosPortMsg.stPortValues);
      
      #ifdef _EN_WIMOS_IMU
        DEBUG_VALID(testName , 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUGyrosX), 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUGyrosX != 0));
      #else      
        
        DEBUG_VALID(testName , 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUGyrosX), 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUGyrosX == 0));
      #endif
      delay(10);
      /*End_Body_TEST:*/
    } 

    /**
     * @brief Wimos test n3.UT28.
     *
     * Unit test n3.UT28 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n3UT28 (void){
      const char* testName = "n3.UT28 = %d";
      /*Body_TEST:*/     
      
      stGlobalWimosPortMsg.stPortValues.usInternalIMUGyrosY = 0;
      readIMU(&stGlobalWimosPortMsg.stPortValues);
      
      #ifdef _EN_WIMOS_IMU
        DEBUG_VALID(testName , 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUGyrosY), 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUGyrosY != 0));
      #else      
        
        DEBUG_VALID(testName , 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUGyrosY), 
                   (stGlobalWimosPortMsg.stPortValues.usInternalIMUGyrosY == 0));
      #endif
      delay(10);
      /*End_Body_TEST:*/
    } 

    
  #endif
  
  #ifdef WIMOS_VALIDATION_TEST
  
    
    /**
     * @brief Wimos test n3.VT05.
     *
     * Unit test n3.VT05 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
    extern void _test_n3VT06 (void){
      const char* testName = "n3.VT06 = %ld microseconds";
      
      /*Body_TEST:*/     
      initIMU();
      uint32_t ulTimerVT = micros();
      readIMU(&stGlobalWimosPortMsg.stPortValues);
      DEBUG_VALID(testName , 
                 (micros() - ulTimerVT), 
                 (micros() - ulTimerVT < 1000));
      /*End_Body_TEST:*/
    } 
    
  #endif
#endif
