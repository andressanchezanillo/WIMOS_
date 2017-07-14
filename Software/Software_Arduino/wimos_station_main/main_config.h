/****************************************************************************
 * Copyright (C) 2015 by Andrés Sánchez Anillo                              *
 *                                                                          *
 * This file is part of Wimos Firmware.                                                *
 *                                                                          *
 *   Wimos Firmware is free software: you can redistribute it and/or modify it         *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Wimos Firmware is distributed in the hope that it will be useful,                 *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Wimos Firmware.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

/**
 * @file main_config.h
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main configuration for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */

#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_

    #define PACKAGE_SIZE_RF   0x04
    /***************************************************************
     * Debug interface definition:
     ***************************************************************/
    
    #define COMMAND_GET_GENERAL_INFO  0xA1
    #define COMMAND_GET_ALERT_LIST    0xA2
    #define BROADCAST_ID              0xFE
    
    #ifdef __SAM3X8E__ 
      /**
       * @brief Timeout ACK.
       */
      #define TIMEOUT_ACK             250
      
      /**
       * @brief Timeout Alert.
       */
      #define _WIMOS_ALERT_TIMEOUT    ((uint32_t)0x0FA0u)
    #endif
    
    #ifdef  __AVR_ATmega32U4__
      /**
       * @brief Timeout ACK.
       */
      #define TIMEOUT_ACK 250
    #endif

    
    #ifndef CRITICAL_REFRESH_MS
      #define CRITICAL_REFRESH_MS     100
    #endif
    
    #ifndef INFO_REFRESH_MS
      #define INFO_REFRESH_MS         250
    #endif
    
    
    
    #if defined(WIMOS_UNIT_TEST) or defined(WIMOS_VALIDATION_TEST)
      #undef _WIMOS_1A_OFFSET_1
      #define _WIMOS_1A_OFFSET_1                 0x10
      
      #undef _WIMOS_1A_COEFICIENT_1             
      #define _WIMOS_1A_COEFICIENT_1             0x02
      
      #undef _WIMOS_1A_OFFSET_2                 
      #define _WIMOS_1A_OFFSET_2                 0x20
      
      #undef _WIMOS_1A_COEFICIENT_2             
      #define _WIMOS_1A_COEFICIENT_2             0x04
      
      #undef _WIMOS_2A_OFFSET_1
      #define _WIMOS_2A_OFFSET_1                 0x10
      
      #undef _WIMOS_2A_COEFICIENT_1             
      #define _WIMOS_2A_COEFICIENT_1             0x02
      
      #undef _WIMOS_2A_OFFSET_2                 
      #define _WIMOS_2A_OFFSET_2                 0x20
      
      #undef _WIMOS_2A_COEFICIENT_2             
      #define _WIMOS_2A_COEFICIENT_2             0x04
      
      #undef _WIMOS_3A_OFFSET_1
      #define _WIMOS_3A_OFFSET_1                 0x10
      
      #undef _WIMOS_3A_COEFICIENT_1             
      #define _WIMOS_3A_COEFICIENT_1             0x02
      
      #undef _WIMOS_3A_OFFSET_2                 
      #define _WIMOS_3A_OFFSET_2                 0x20
      
      #undef _WIMOS_3A_COEFICIENT_2             
      #define _WIMOS_3A_COEFICIENT_2             0x04
      
      #undef _WIMOS_4A_OFFSET_1
      #define _WIMOS_4A_OFFSET_1                 0x10
      
      #undef _WIMOS_4A_COEFICIENT_1             
      #define _WIMOS_4A_COEFICIENT_1             0x02
      
      #undef _WIMOS_4A_OFFSET_2                 
      #define _WIMOS_4A_OFFSET_2                 0x20
      
      #undef _WIMOS_4A_COEFICIENT_2             
      #define _WIMOS_4A_COEFICIENT_2             0x04

      #undef _WIMOS_5A1_OFFSET_1
      #define _WIMOS_5A1_OFFSET_1                 (- VCC_LOGIC / 2)
      #undef _WIMOS_5A1_COEFICIENT_1
      #define _WIMOS_5A1_COEFICIENT_1             (float)(GRAVITY_MM_S2/(VCC_LOGIC/5))
      #undef _WIMOS_5A1_OFFSET_2
      #define _WIMOS_5A1_OFFSET_2                 0x00
      #undef _WIMOS_5A1_COEFICIENT_2
      #define _WIMOS_5A1_COEFICIENT_2             0x01

      #undef _WIMOS_5A2_OFFSET_1
      #define _WIMOS_5A2_OFFSET_1                 (- VCC_LOGIC / 2)
      #undef _WIMOS_5A2_COEFICIENT_1
      #define _WIMOS_5A2_COEFICIENT_1             (float)(GRAVITY_MM_S2/(VCC_LOGIC/5))
      #undef _WIMOS_5A2_OFFSET_2
      #define _WIMOS_5A2_OFFSET_2                 0x00
      #undef _WIMOS_5A2_COEFICIENT_2
      #define _WIMOS_5A2_COEFICIENT_2             0x01

      #undef _WIMOS_5A3_OFFSET_1
      #define _WIMOS_5A3_OFFSET_1                 (- VCC_LOGIC / 2)
      #undef _WIMOS_5A3_COEFICIENT_1
      #define _WIMOS_5A3_COEFICIENT_1             (float)(GRAVITY_MM_S2/(VCC_LOGIC/5))
      #undef _WIMOS_5A3_OFFSET_2
      #define _WIMOS_5A3_OFFSET_2                 0x00
      #undef _WIMOS_5A3_COEFICIENT_2
      #define _WIMOS_5A3_COEFICIENT_2             0x01

    #endif
      
    /**
     * @brief Date data struct.
     */
    typedef struct __attribute__((packed)) _date{
      uint8_t ucDay; /**< Day value. */
      uint8_t ucMonth; /**< Month value. */
      uint8_t ucYear;  /**< Year value [2000 -> 2255]. */
    }stWimosDate;
    
    /**
     * @brief Time data struct.
     */
    typedef struct __attribute__((packed)) _time{
      uint8_t ucSecond; /**< Second value. */
      uint8_t ucMinute; /**< Minute value. */
      uint8_t ucHour; /**< Year value. */
    }stWimosTime;
    
    /**
     * @brief DateTime data struct.
     */
    typedef struct __attribute__((packed)) _dateTime{
      stWimosDate stDate; /**< Date value. */
      stWimosTime stTime; /**< Time value. */
    }stWimosDateTime;
    
    /**
     * @brief Localization data struct.
     */
    typedef struct __attribute__((packed)) _gpsCoordenate{
      uint8_t ucDegree;  /**< Degree for Coordenate. */
      int32_t slMinute;  /**< Minute (multiplicated by 1000). */
    }stWimosGpsCoordenate;
    
    /**
     * @brief GPS position data struct
     */
    typedef struct __attribute__((packed)) _gpsPosition{
      stWimosGpsCoordenate stLatitude; /**< Latitude position. */
      stWimosGpsCoordenate stLongitude;/**< Longitude position. */
    }stWimosGpsPosition;
    
    
    /**
     * @brief System Status data struct.
     */
    typedef struct __attribute__((packed)) _status{
      uint16_t usPortStatus; /**< Status of System Ports. */
      uint8_t ucDeviceStatus;/**< Status of System Devices. */
    }stWimosStatus;
    
    
    /**
     * @brief System Signature data struct.
     */
    typedef struct __attribute__((packed)) _signature{
      stWimosDateTime stDateTime;/**< DateTime Device. */  
      stWimosStatus stStatus;/**< Status of System Devices. */  
      float usPort1A; /**< portA1 Processed. */ 
      float usOffset1A; /**< portA1 Offset calculated. */ 
      uint8_t usAlert1A; /**< portA1 Alert detected */ 
      float usPort2A; /**< portA2 Processed. */ 
      float usOffset2A; /**< portA2 Offset calculated. */ 
      uint8_t usAlert2A; /**< portA2 Alert detected */ 
      float usPort3A; /**< portA3 Processed. */ 
      float usOffset3A; /**< portA3 Offset calculated. */ 
      uint8_t usAlert3A; /**< portA3 Alert detected */ 
      float usPort4A; /**< portA4 Processed. */       
      float usOffset4A; /**< portA4 Offset calculated. */ 
      uint8_t usAlert4A; /**< portA4 Alert detected */ 
      float usPort5A1; /**< portA5 Processed. */ 
      float usPort5A2; /**< portA5 Processed. */ 
      float usPort5A3; /**< portA5 Processed. */ 
      float usPort5A; /**< portA5 Processed. */ 
      float usOffset5A; /**< portA5 Offset calculated. */ 
      uint8_t usAlert5A; /**< portA5 Alert detected */ 
      uint8_t usPortD1; /**< portD1 Alert detected */ 
      uint8_t usPortD2; /**< portD2 Alert detected */ 
      uint8_t usPortD3; /**< portD3 Alert detected */ 
      uint8_t usPortD4; /**< portD4 Alert detected */ 
      uint8_t usPortD5; /**< portD5 Alert detected */ 
      uint8_t ucPortI2C[_WIMOS_PORT_I2C_DATA_SIZE];
      uint32_t ulImuAcc; /**< IMU X axe */ 
      uint32_t ulImuGx; /**< IMU Y axe */ 
      uint32_t ulImuGy; /**< IMU Z axe */ 
    }stWimosStorage;
    
    
    /**
     * @brief Wimos information Frame
     */
    typedef struct __attribute__((packed)) _wimosInfo{
      stWimosDateTime stDateTime; /**< Date Time value. */
      stWimosGpsPosition stGpsPosition; /**< GPS position value. */
      uint8_t ucPercentMemory;  /**< Percent memory used value. */
      uint8_t ucPercentBattery; /**< Percent battery available value. */
      stWimosStatus stStatus; /**< System status value. */
    } stWimosInfo;
    
    /**
     * @brief Wimos information Frame
     */
    typedef struct __attribute__((packed)) _wimosInfoMsg{
      uint8_t ucBegin; /**< Constant value for frame begin. */
      uint8_t ucFrameID; /**< Frame size value. */
      
      uint8_t ucMessageFrom;/**< Address of system what send the message . */
      uint8_t ucMessageTo;/**< The message is for this address . */
    
      stWimosInfo stInfo;/**< Wimos internal information. */
      
      uint8_t ucChecksum;  /**< checksum value. */
    } stWimosInfoMessage;
    
    /**
     * @brief ACK message frame.
     */
    typedef struct __attribute__((packed)) _ackMessage{
      uint8_t ucBegin; /**< Constant frame begin value. */
      uint8_t ucFrameID; /**< Const size value. */
      
      uint8_t ucMessageFrom;/**< Address of system what send the message . */
      uint8_t ucMessageTo;/**< The message is for this address . */
      
      uint8_t ucACK; /**< checksum value. */
    } stWimosACK;

    #define COMMAND_BEGIN_BYTE_CONST  0xFF
    #define COMMAND_SIZE_BYTE_CONST   0x04
    #define ACK_SIZE_BYTE_CONST   0x03
    
    typedef struct __attribute__((packed)) _commandMessage{ 
      uint8_t ucBegin; /**< Constant frame begin value. */
      uint8_t ucFrameID; /**< Const size value. */
      
      uint8_t ucMessageFrom;/**< Address of system what send the message . */
      uint8_t ucMessageTo;/**< The message is for this address . */
      
      uint8_t ucCommand; /**< Command value. */
      uint8_t ucChecksum; /**< Checksum value. */
    } stCommandMessage;

    
    typedef struct __attribute__((packed)) _WimosAlert{ 
      stWimosDateTime stDateTime; /**< Internal. */
      uint32_t ulInternalTime; /**< Alert level A1. */
      uint8_t ucAlertA1;/**< Alert level A1. */
      uint8_t ucAlertA2;/**< Alert level A2. */
      uint8_t ucAlertA3;/**< Alert level A3. */
      uint8_t ucAlertA4;/**< Alert level A4. */
      uint8_t ucAlertA5;/**< Alert level A5. */
    } stWimosAlert;

    
    typedef struct __attribute__((packed)) _AlertMessage{ 
      uint8_t ucBegin; /**< Constant frame begin value. */
      uint8_t ucFrameID; /**< Const size value. */
      
      uint8_t ucMessageFrom;/**< Address of system what send the message . */
      uint8_t ucMessageTo;/**< The message is for this address . */
      
      stWimosAlert stAlert; /**< Alert value. */
      uint8_t ucChecksum; /**< Checksum value. */
    } stAlertMessage;

    
    
    /**
     * @brief Core Wimos function.
     */
    void coreWimos(void);
    
    /**
     * @brief Initialize Debug.
     */
    void initDebug(void);
    
    /**
     * @brief Initialize WIMOS.
     */
    void initWimos(void);
    
    /**
     * @brief Initialize the internal peripherals.
     */
    void initInterPeriph(void);
    
    /**
     * @brief Initialize communication state.
     */
    void initRF(void);
    
    /**
     * @brief Communication thread pointer.
     */
    void (*communicationThread)(void);
        
    /**
     * @brief Label for none elements.
     */
    #define NONE 0x00
    
    /**
     * @brief Label for ERROR response.
     */
    #define _ERROR ((int8_t)-1)
    /**
     * @brief Label for OK response.
     */
    #define _OK ((int8_t)0)
    
    /**
     * @brief Label for OK response.
     */
    #define _NEXT_STEP ((int8_t)1)

    #ifdef __AVR_ATmega32U4__  
      /**
       * @brief DEBUG Serial interface.
       */
      #define SERIAL_DEBUG Serial
      
      /**
       * @brief DEBUG Serial baudrate.
       */
      #define BAUDRATE_DEBUG (115200)
      
      /**
       * @brief Debug Macro for Test.
       */
      #define DEBUG_VALID(x,data,cond) SERIAL_DEBUG.print(x);SERIAL_DEBUG.print(data);SERIAL_DEBUG.println(cond?"\t\tOK":"\t\tERROR"); 
        
    #endif

  
  #ifdef __SAM3X8E__  
    
    /***************************************************************
     * Device Masks definition:
     ***************************************************************/
    
    /**
     * @brief DEBUG Serial interface.
     */
    #define SERIAL_DEBUG Serial
    /**
     * @brief DEBUG Serial baudrate.
     */
    #define BAUDRATE_DEBUG (115200)
        
    #ifdef WIMOS_DEBUG
    
      #ifdef DEBUG_COLOR
        /**
         * @brief Info label displaying with colour.
         */
        #define D_INFO "[\x1B[36mINFO\x1B[0m]  "
        /**
         * @brief Error label displaying with colour.
         */
        #define D_ERROR "[\x1B[31mERROR\x1B[0m] "
        /**
         * @brief Data label displaying with colour.
         */
        #define D_DATA "[\x1B[33mDATA\x1B[0m]  "
        /**
         * @brief Data label displaying with colour.
         */
        #define D_OK "[\x1B[32mOK\x1B[0m]    "
        
      #else
        /**
         * @brief Info label displaying.
         */
        #define D_INFO "[INFO]  "
        /**
         * @brief Error label displaying.
         */
        #define D_ERROR "[ERROR] "
        /**
         * @brief Data label displaying.
         */
        #define D_DATA "[DATA]  "
        /**
         * @brief OK label displaying.
         */
        #define D_OK "[OK]    "
      
      #endif
    
      /**
       * @brief Debug display enumeration.
       */
      enum eDebugMode{
        eINFO = 1,
        eDATA = 2,
        eOK = 7,
        eERROR = 7,
      };
      
      /**
       * @brief Debug display function.
       */
      void debug(const char* pFunction, const char* pLabel, const char* pData, eDebugMode eMode);
      
      /**
       * @brief Debug display function.
       */
      void debugUTest(const char* pFunction, const char* pLabel, const char* pData, eDebugMode eMode);
      
      /**
       * @brief Debug Macro for ERROR displays.
       */
      #define DEBUG_ERROR(x) debug(__func__,D_ERROR,x,eERROR) 
      
      /**
       * @brief Debug Macro for ERROR displays.
       */
      #define DEBUG_OK(x) debug(__func__,D_OK,x,eOK) 
      
      /**
       * @brief Debug Macro for ERROR displays.
       */
      #define DEBUG_DATA(x,data) sprintf(pDebug,x,data); debug(__func__,D_DATA,pDebug,eDATA);sprintf(pDebug,"") 
      
      /**
       * @brief Debug Macro for ERROR displays.
       */
      #define DEBUG_INFO(x) debug(__func__,D_INFO,x,eINFO) 
      
      /**
       * @brief Debug Macro for Test.
       */
      #define DEBUG_VALID(x,data,cond) sprintf(pDebug,x,data); debugUTest(__func__, (cond?D_OK:D_ERROR) ,pDebug,(cond?eOK:eERROR));sprintf(pDebug,"") 
            
      void debugCommand(void);
    
    #else
      /**
       * @brief Debug Macro for ERROR displays.
       */
      #define DEBUG_ERROR(x) {}
      /**
       * @brief Debug Macro for OK displays.
       */
      #define DEBUG_OK(x) {}
      /**
       * @brief Debug Macro for ERROR displays.
       */
      #define DEBUG_DATA(x,y) {}
      /**
       * @brief Debug Macro for ERROR displays.
       */
      #define DEBUG_INFO(x) {}
    #endif

    
    /**
     * @brief Analog Port 1 mask.
     */
    #define WIMOS_PORT_A1_MASK     ((uint16_t)0x01)
    
    /**
     * @brief Analog Port 2 mask.
     */
    #define WIMOS_PORT_A2_MASK     ((uint16_t)0x02)
    
    /**
     * @brief Analog Port 3 mask.
     */
    #define WIMOS_PORT_A3_MASK     ((uint16_t)0x04)
    
    /**
     * @brief Analog Port 4 mask.
     */
    #define WIMOS_PORT_A4_MASK     ((uint16_t)0x08)
    
    /**
     * @brief Analog Port 5 mask.
     */
    #define WIMOS_PORT_A5_MASK     ((uint16_t)0x10)
    
    
    /**
     * @brief Digital Port 1 mask.
     */
    #define WIMOS_PORT_D1_MASK     ((uint16_t)0x20)
    
    /**
     * @brief Digital Port 2 mask.
     */
    #define WIMOS_PORT_D2_MASK     ((uint16_t)0x40)
    
    /**
     * @brief Digital Port 3 mask.
     */
    #define WIMOS_PORT_D3_MASK     ((uint16_t)0x80)
    
    /**
     * @brief Digital Port 4 mask.
     */
    #define WIMOS_PORT_D4_MASK     ((uint16_t)0x100)
    
    /**
     * @brief Digital Port 5 mask.
     */
    #define WIMOS_PORT_D5_MASK     ((uint16_t)0x200)
    
    /**
     * @brief Digital To Analog Converter Port 1 mask.
     */
    #define WIMOS_PORT_DAC1_MASK   ((uint16_t)0x400)
    /**
     * @brief Digital To Analog Converter Port 2 mask.
     */
    #define WIMOS_PORT_DAC2_MASK   ((uint16_t)0x800)
    /**
     * @brief UART Port mask.
     */
    #define WIMOS_PORT_UART_MASK   ((uint16_t)0x1000)
    /**
     * @brief I2C Port mask.
     */
    #define WIMOS_PORT_I2C_MASK    ((uint16_t)0x2000)
    
    /**
     * @brief Internal GPS mask.
     */
    #define WIMOS_DEVICE_GPS_MASK  ((uint8_t)0x01)
    
    /**
     * @brief Internal IMU mask.
     */
    #define WIMOS_DEVICE_IMU_MASK  ((uint8_t)0x02)
    
    /**
     * @brief Internal SD mask.
     */
    #define WIMOS_DEVICE_SD_MASK   ((uint8_t)0x04)
    
    /**
     * @brief Internal Battery Reader mask.
     */
    #define WIMOS_DEVICE_BATT_MASK  ((uint8_t)0x08)
    
    /**
     * @brief Internal TV mask.
     */
    #define WIMOS_DEVICE_TV_MASK     ((uint8_t)0x10)
    
    /**
     * @brief Internal RF mask.
     */
    #define WIMOS_DEVICE_RF_MASK     ((uint8_t)0x20)
    
    /**
     * @brief Internal Debug mask.
     */
    #define WIMOS_DEVICE_DEBUG_MASK     ((uint8_t)0x40)
    
    
    
    /***************************************************************
     * Pinouts definition:
     ***************************************************************/
    
    
    /**
     * @brief Internal Battery Reader mask.
     */
    #define WIMOS_BATT_PIN   (A8)
    /**
     * @brief External analog port A1.
     */
    #define WIMOS_A1_PORT   (A0)
    /**
     * @brief External analog port A2.
     */
    #define WIMOS_A2_PORT   (A1)
    /**
     * @brief External analog port A3.
     */
    #define WIMOS_A3_PORT   (A2)
    /**
     * @brief External analog port A4.
     */
    #define WIMOS_A4_PORT   (A3)
    /**
     * @brief External analog port A5_1.
     */
    #define WIMOS_A5_1_PORT   (A4)
    /**
     * @brief External analog port A5_2.
     */
    #define WIMOS_A5_2_PORT   (A5)
    /**
     * @brief External analog port A5_3.
     */
    #define WIMOS_A5_3_PORT   (A6)
    
    /**
     * @brief Pin enable SD.
     */
    #define WIMOS_SD_CS_PIN   (10)
    
    /**
     * @brief External digital port.
     */
    #define WIMOS_D1_1_PORT   (47)
    /**
     * @brief External digital port.
     */
    #define WIMOS_D1_2_PORT   (49)
    /**
     * @brief External digital port.
     */
    #define WIMOS_D1_3_PORT   (51)
    /**
     * @brief External digital port.
     */
    #define WIMOS_D1_4_PORT   (53)
    
    /**
     * @brief External digital port.
     */
    #define WIMOS_D2_1_PORT   (43)
    /**
     * @brief External digital port.
     */
    #define WIMOS_D2_2_PORT   (42)
    /**
     * @brief External digital port.
     */
    #define WIMOS_D2_3_PORT   (45)
    
    /**
     * @brief External digital port.
     */
    #define WIMOS_D3_1_PORT   (3)
    /**
     * @brief External digital port.
     */
    #define WIMOS_D3_2_PORT   (2)
    
    
    /**
     * @brief External digital port.
     */
    #define WIMOS_D4_1_PORT   (5)
    /**
     * @brief External digital port.
     */
    #define WIMOS_D4_2_PORT   (4)
    
    /**
     * @brief External digital port.
     */
    #define WIMOS_D5_1_PORT   (6)
    
    /**
     * @brief External digital port.
     */
    #define WIMOS_LED_PORT   (9)
    
    /**
     * @brief Label for Digital LED Port Mode.
     */
    #define _WIMOS_PORT_MODE_LED INPUT
    
    
    /**
     * @brief Internal Battery Reader mask.
     */
    #define WIMOS_INTERNAL_IMU_ADDRESS   (0x68)
    
    
    
    
    /***************************************************************
     * Math Constants definition:
     ***************************************************************/
    
     #define VCC_LOGIC                          (3300U)
     #define ADC_MAX_VALUE                      (65535U)
     #define ADC_VOLTAGE_TO_BAT_VOLTAGE         (4U)
     #define VCC_MAX_BATTERY                    (13000U)       
     #define VCC_MIN_BATTERY                    (7150U)    
     #define GRAVITY_MM_S2                      ((float)9806.65f)
     #define MATH_VECTOR3D_LENGHT(x,y,z)        (float)(sqrt((x*x)+(y*y)+(z*z)));
     #define MATH_AVERAGE3D(x,y,z)              (float)((x+y+z)/3U);
     #define MATH_VECTOR2D_LENGHT(x,y)          (float)sqrt((x*x)+(y*y));
     #define MATH_AVERAGE2D(x,y)                (float)((x+y)/2U);
     
     #define MATH_MOVING_AVERAGE(lastAverage, inputValue, currentIndex, maxIndex)       {\
                                                                                            lastAverage = ((lastAverage * currentIndex) - lastAverage)/(currentIndex-1);\
                                                                                            lastAverage = ((lastAverage * (currentIndex-1)) + inputValue)/currentIndex;\
                                                                                            if(currentIndex < maxIndex)currentIndex++;\
                                                                                        }
     
     #define MATH_DEFAULT_THRESHOLD(inputValue, inputThreshold, inputOffset)           ((inputOffset >= 0)?(inputValue >= (inputThreshold + inputOffset)):(inputValue <= (inputThreshold+inputOffset)))        

    
    uint8_t detectionA1Custom(float fInputValue);
    uint8_t detectionA1Default(float fInputValue);
    
    uint8_t detectionA2Custom(float fInputValue);
    uint8_t detectionA2Default(float fInputValue);
    
    uint8_t detectionA3Custom(float fInputValue);
    uint8_t detectionA3Default(float fInputValue);
    
    uint8_t detectionA4Custom(float fInputValue);
    uint8_t detectionA4Default(float fInputValue);
    
    uint8_t detectionA5Custom(float fInputValue);
    uint8_t detectionA5Default(float fInputValue);



    
    /***************************************************************
     * Memory definition:
     ***************************************************************/
    
     #define WIMOS_SIGN_FILE                    "Wms.dat"
     #define WIMOS_NON_VOLATILE_DIR             "nvoltl"
     #define WIMOS_NON_VOLATILE_FILE            "Wnv"
     #define WIMOS_NON_VOLATILE_FILE_MAX        65535
     #define WIMOS_VOLATILE_FILE_NUMBER_MAX     10
     #define WIMOS_NON_VOLATILE_EXT             ".log\0"
     #define WIMOS_VOLATILE_EXT                 ".log\0"
     #define WIMOS_VOLATILE_DIR                 "voltl"
     #define WIMOS_VOLATILE_FILE                "Wv"
     #define WIMOS_MEM_BUFFER_SIZE              200
    
    
    
    /***************************************************************
     * Serial interfaces definition:
     ***************************************************************/
    
    /**
     * @brief RF Serial interface.
     */
    #define SERIAL_RF Serial2
    /**
     * @brief RF Serial baudrate.
     */
    #define BAUDRATE_RF (115200)
        
    /**
     * @brief GPS Serial interface.
     */
    #define SERIAL_GPS Serial3
        
    /**
     * @brief GPS Serial interface.
     */
    #define GPS_CONFIG_RMC "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
    /**
     * @brief GPS Serial baudrate.
     */
    #define BAUDRATE_GPS (9600)
        
    
    /***************************************************************
     * Enums definition:
     ***************************************************************/
    
    enum PeriphericErrors{
      OK_INITIALIZATION = 0,
      ERROR_INITIALIZATION = 1,
      UKNOWN_ERROR = 2,
    };
    
    /**
     * @brief Debug display enumeration.
     */
    enum eRegisterI2C{
      eConfigWrite = 1,
      eConfigRead= 2,
      eDataRead= 3,
    };
    
    
    
    /***************************************************************
     * Structs definition:
     ***************************************************************/
    
    typedef struct _i2cConfig{
      eRegisterI2C eRegisterType;
      uint8_t ucDeviceAddress;
      uint16_t ucRegisterAddress;
      uint8_t ucRegisterValue;
    }stWimosI2CRegister;
    
    
    /***************************************************************
     * Global Functions definition:
     ***************************************************************/
    
    /**
     * @brief Core Wimos function.
     */
    void coreWimos(void);
    
    /**
     * @brief Initialize Debug.
     */
    void initDebug(void);
    /**
     * @brief Initialize WIMOS.
     */
    void initWimos(void);

    /**
     * @brief Initialize GPS.
     */
    void initGPS(void);
    /**
     * @brief Update GPS position and datetime.
     */
    void updateGPS(stWimosInfo* _stWimosInfo);
    /**
     * @brief Initialize SD.
     */
    int32_t initSD(void);
    /**
     * @brief Write Frame in SD.
     */
     uint32_t writeFrameSD (const stWimosStorage* stWimosPort);
    /**
     * @brief Initialize Battery Reader.
     */
    void initBattery(void);
    
    /**
     * @brief Update battery status.
     */
    void updateStatusBattery(stWimosInfo* _stWimosInfo);
    
    /**
     * @brief Thread for wimos display (TV or RF).
     */
    void (*coreWimosDisplay)(void);
    
    /**
     * @brief Initialize IMU.
     */
    void initIMU(void);
    
    /**
     * @brief Update IMU value.
     */
    void readIMU(stWimosStorage* stWimosPort);
    /**
     * @brief Initialize Analog Ports.
     */
    void initPortA(void);
    /**
     * @brief Update Analog Ports values.
     */
    void readPortA(stWimosStorage* stWimosPort);
    
    /**
     * @brief Initialize Digital Port.
     */
    void initPortD(void);
    /**
     * @brief Update Digital Ports values.
     */
    void readPortD(stWimosStorage* stWimosPort);
    /**
     * @brief Initialize I2C Port.
     */
    void initPortI2C(void);
    /**
     * @brief Update I2C Ports values.
     */
    void readPortI2C(stWimosStorage* stWimosPort);
    /**
     * @brief No operation function.
     */
    void noOperation(void);
    
    void stopTV(void);
    
    uint32_t coreWimosTVTimer;
    
    int8_t storeVolatile(void* ptrBuffer, uint8_t ucBufferSize);
    
    #endif
    
    
    
    #ifdef WIMOS_DEBUG
      #ifdef WIMOS_UNIT_TEST
        void _test_n3UT00 (void);
        void _test_n3UT01 (void);
        void _test_n3UT02 (void);
        void _test_n3UT03 (void);
        void _test_n3UT04 (void);
        void _test_n3UT05 (void);
        void _test_n3UT06 (void);
        void _test_n3UT07 (void);
        void _test_n3UT08 (void);
        void _test_n3UT09 (void);
        void _test_n3UT10 (void);
        void _test_n3UT11 (void);
        void _test_n3UT12 (void);
        void _test_n3UT13 (void);
        void _test_n3UT14 (void);
        void _test_n3UT15 (void);
        void _test_n3UT16 (void);
        void _test_n3UT17 (void);
        void _test_n3UT18 (void);
        void _test_n3UT19 (void);
        void _test_n3UT20 (void);
        void _test_n3UT21 (void);
        void _test_n3UT22 (void);
        void _test_n3UT23 (void);
        void _test_n3UT24 (void);
        void _test_n3UT25 (void);
        void _test_n3UT26 (void);
        void _test_n3UT27 (void);
        void _test_n3UT28 (void);
        void _test_n3UT29 (void);
        void _test_n3UT30 (void);
        void _test_n3UT31 (void);
        void _test_n3UT32 (void);
        void _test_n3UT33 (void);
        void _test_n3UT34 (void);
        void _test_n3UT35 (void);
        void _test_n3UT36 (void);
        void _test_n3UT37 (void);
        void _test_n3UT38 (void);
        void _test_n3UT39 (void);
        void _test_n3UT42 (void);
        void _test_n4UT01 (void);
        void _test_n4UT02 (void);
        void _test_n4UT03 (void);
        void _test_n4UT04 (void);
        void _test_n4UT05 (void);
        #ifdef __SAM3X8E__
          void _test_n4UT06 (void);
          void _test_n4UT07 (void);
          void _test_n4UT08 (void);
          void _test_n4UT09 (void);
          void _test_n4UT10 (void);
        #endif
        #ifdef __AVR_ATmega32U4__        
          void _test_n4UT11 (void);
        #endif
        void _test_n4UT12 (void);
        void _test_n4UT13 (void);
        void _test_n4UT14 (void);
        void _test_n4UT15 (void);
        void _test_n4UT16 (void);
        void _test_n4UT17 (void);
        #ifdef __SAM3X8E__
          void _test_n4UT18 (void);
        #endif
        #ifdef __AVR_ATmega32U4__        
          void _test_n4UT19 (void);
          void _test_n4UT20 (void);
        #endif
        #ifdef __SAM3X8E__
          void _test_n5UT01 (void);
          void _test_n5UT02 (void);
          void _test_n5UT03 (void);
          void _test_n5UT04 (void);
          void _test_n5UT05 (void);
          void _test_n5UT06 (void);
          void _test_n5UT07 (void);
          void _test_n5UT08 (void);
          void _test_n5UT09 (void);
          void _test_n5UT10 (void);
          void _test_n5UT11 (void);
          void _test_n5UT12 (void);
          void _test_n5UT13 (void);
          void _test_n5UT14 (void);
          void _test_n5UT15 (void);
          void _test_n5UT16 (void);
          void _test_n5UT17 (void);
          void _test_n5UT18 (void);
          void _test_n5UT19 (void);
          void _test_n5UT20 (void);
          void _test_n5UT21 (void);
          void _test_n5UT22 (void);
          void _test_n5UT23 (void);
          void _test_n5UT24 (void);
          void _test_n5UT25 (void);
          void _test_n5UT26 (void);
          void _test_n5UT27 (void);
          void _test_n5UT28 (void);
          void _test_n5UT29 (void);
          void _test_n5UT30 (void);
          void _test_n5UT31 (void);
          void _test_n5UT32 (void);
          void _test_n5UT33 (void);
          void _test_n5UT34 (void);
        #endif
        #ifdef __SAM3X8E__
          void _test_n6UT01 (void);
          void _test_n6UT02 (void);
          void _test_n6UT03 (void);
          void _test_n6UT04 (void);
          void _test_n6UT05 (void);
          void _test_n6UT06 (void);
          void _test_n6UT07 (void);
          void _test_n6UT08 (void);
          void _test_n6UT09 (void);
          void _test_n6UT10 (void);
        #endif
      #endif
      
      #ifdef WIMOS_VALIDATION_TEST
        void _test_n3VT01 (void);
        void _test_n3VT02 (void);
        void _test_n3VT03 (void);
        void _test_n3VT05 (void);
        void _test_n3VT06 (void);
        void _test_n3VT07 (void);
        void _test_n3VT08 (void);
        void _test_n3VT09 (void);
        void _test_n3VT10 (void);
        #ifdef __AVR_ATmega32U4__    
          void _test_n4VT01 (void);
        #endif
      #endif
    #endif
  #endif
  #ifdef __AVR_ATmega32U4__
    
      /**
       * @brief Debug Macro for ERROR displays.
       */
      #define DEBUG_ERROR(x) {}
      /**
       * @brief Debug Macro for OK displays.
       */
      #define DEBUG_OK(x) {}
      /**
       * @brief Debug Macro for ERROR displays.
       */
      #define DEBUG_DATA(x,y) {}
      /**
       * @brief Debug Macro for ERROR displays.
       */
      #define DEBUG_INFO(x) {}   
    
    /**
     * @brief RF Serial interface.
     */
    #define SERIAL_USB Serial
    /**
     * @brief RF Serial baudrate.
     */
    #define BAUDRATE_USB (115200)
    
    /**
     * @brief RF Serial interface.
     */
    #define SERIAL_RF SerialRF
    /**
     * @brief RF Serial baudrate.
     */
    #define BAUDRATE_RF (57600)

    
    
  #endif
  

