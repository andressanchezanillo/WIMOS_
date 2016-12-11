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




/***************************************************************
 * Device Masks definition:
 ***************************************************************/

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
 * @brief Internal RF mask.
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
 * @brief Internal Battery Reader mask.
 */
#define WIMOS_INTERNAL_IMU_ADDRESS   (0x68)




/***************************************************************
 * Math Constants definition:
 ***************************************************************/

 #define VCC_LOGIC                          3300
 #define ADC_MAX_VALUE                      1024
 #define ADC_VOLTAGE_TO_BAT_VOLTAGE         4
 #define VCC_MAX_BATTERY                    13000       
 #define VCC_MIN_BATTERY                    7150


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
#define SERIAL_RF Serial1
/**
 * @brief RF Serial baudrate.
 */
#define BAUDRATE_RF (115200)

/**
 * @brief GPS Serial interface.
 */
#define SERIAL_GPS Serial3
/**
 * @brief GPS Serial baudrate.
 */
#define BAUDRATE_GPS (9600)

/**
 * @brief DEBUG Serial interface.
 */
#define SERIAL_DEBUG Serial
/**
 * @brief DEBUG Serial baudrate.
 */
#define BAUDRATE_DEBUG (115200)



/***************************************************************
 * Enums definition:
 ***************************************************************/


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

enum PeriphericErrors{
  OK_INITIALIZATION = 0,
  ERROR_INITIALIZATION = 1,
  UKNOWN_ERROR = 2,
};




/***************************************************************
 * Debug interface definition:
 ***************************************************************/


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

#else
  /**
   * @brief Debug Macro for ERROR displays.
   */
  #define DEBUG_ERROR(x) {}
  /**
   * @brief Debug Macro for ERROR displays.
   */
  #define DEBUG_DATA(x) {}
  /**
   * @brief Debug Macro for ERROR displays.
   */
  #define DEBUG_INFO(x) {}
#endif


/***************************************************************
 * Structs definition:
 ***************************************************************/

/**
 * @brief Debug display enumeration.
 */
enum eRegisterI2C{
  eConfigWrite = 1,
  eConfigRead= 2,
  eDataRead= 3,
};

typedef struct _i2cConfig{
  eRegisterI2C eRegisterType;
  uint8_t ucDeviceAddress;
  uint16_t ucRegisterAddress;
  uint8_t ucRegisterValue;
}stWimosI2CRegister;

/**
 * @brief Date data struct.
 */
typedef struct _date{
  uint8_t ucDay; /**< Day value. */
  uint8_t ucMonth; /**< Month value. */
  uint8_t ucYear;  /**< Year value [2000 -> 2255]. */
}stWimosDate;

/**
 * @brief Time data struct.
 */
typedef struct _time{
  uint8_t ucSecond; /**< Second value. */
  uint8_t ucMinute; /**< Minute value. */
  uint8_t ucHour; /**< Year value. */
}stWimosTime;

/**
 * @brief DateTime data struct.
 */
typedef struct _dateTime{
  stWimosDate stDate; /**< Date value. */
  stWimosTime stTime; /**< Time value. */
}stWimosDateTime;

/**
 * @brief Localization data struct.
 */
typedef struct _gpsCoordenate{
  uint8_t ucDegree;  /**< Degree for Coordenate. */
  int32_t slMinute;  /**< Minute (multiplicated by 1000). */
}stWimosGpsCoordenate;

/**
 * @brief GPS position data struct
 */
typedef struct _gpsPosition{
  stWimosGpsCoordenate stLatitude; /**< Latitude position. */
  stWimosGpsCoordenate stLongitude;/**< Longitude position. */
}stWimosGpsPosition;


/**
 * @brief System Status data struct.
 */
typedef struct _status{
  uint16_t usPortStatus; /**< Status of System Ports. */
  uint8_t ucDeviceStatus;/**< Status of System Devices. */
}stWimosStatus;


/**
 * @brief System Signature data struct.
 */
typedef struct _signature{
  uint8_t ucWimosID; /**< Status of System Ports. */
  stWimosStatus stStatus;/**< Status of System Devices. */  
}stWimosSignature;


/**
 * @brief Wimos information Frame
 */
typedef struct _wimosInfo{
  stWimosDateTime stDateTime; /**< Date Time value. */
  stWimosGpsPosition stGpsPosition; /**< GPS position value. */
  uint8_t ucPercentMemory;  /**< Percent memory used value. */
  uint8_t ucPercentBattery; /**< Percent battery available value. */
  stWimosStatus stStatus; /**< System status value. */
} stWimosInfo;

/**
 * @brief Wimos information Frame
 */
typedef struct _wimosInfoMsg{
  const uint8_t ucBegin = 0xFF; /**< Constant value for frame begin. */
  const uint8_t ucFrameSize = 0x18; /**< Frame size value. */
  
  uint8_t ucMessageFrom;/**< Address of system what send the message . */
  uint8_t ucMessageTo;/**< The message is for this address . */

  stWimosInfo stInfo;/**< Wimos internal information. */
  
  uint8_t checksum;  /**< checksum value. */
} stWimosInfoMessage;

/**
 * @brief Wimos Ports values Frame
 */
typedef struct _portStatus{
  #ifdef _WIMOS_IMU
    uint32_t usInternalIMUAcc; /**< Internal IMU module value. */
    int8_t usInternalIMUGyrosX; /**< Internal IMU module value. */
    int8_t usInternalIMUGyrosY; /**< Internal IMU module value. */
  #endif
  #ifdef _EN_WIMOS_PORT_A1
    int16_t usPortA1; /**< Analog port 1 value. */
  #endif
  #ifdef _EN_WIMOS_PORT_A2
    int16_t usPortA2; /**< Analog port 2 value. */
  #endif
  #ifdef _EN_WIMOS_PORT_A3
    int16_t usPortA3; /**< Analog port 3 value. */
  #endif
  #ifdef _EN_WIMOS_PORT_A4
    int16_t usPortA4; /**< Analog port 4 value. */
  #endif
  #ifdef _EN_WIMOS_PORT_A5
    int16_t usPort5A1; /**< Analog port 5 pin 1 value. */
    int16_t usPort5A2; /**< Analog port 5 pin 2 value. */
    int16_t usPort5A3; /**< Analog port 5 pin 3 value. */
  #endif
  
  #ifdef _EN_WIMOS_PORT_D1
    uint8_t usPortD1; /**< Digital port 1 value. */
  #endif
  #ifdef _EN_WIMOS_PORT_D2
    uint8_t usPortD2; /**< Digital port 2 value. */
  #endif
  #ifdef _EN_WIMOS_PORT_D3
    uint8_t usPortD3; /**< Digital port 3 value. */
  #endif
  #ifdef _EN_WIMOS_PORT_D4
    uint8_t usPortD4; /**< Digital port 4 value. */
  #endif
  #ifdef _EN_WIMOS_PORT_D5
    uint8_t usPortD5; /**< Digital port 5 value. */
  #endif  
  #if (_WIMOS_PORT_I2C_DATA_SIZE > 0)
    uint8_t ucPortI2C [ _WIMOS_PORT_I2C_DATA_SIZE ]; /**< I2C devices values. */
  #endif
} stWimosPortValues;


/**
 * @brief Wimos Ports values Frame
 */
typedef struct _portStatusMsg{
  const uint8_t ucBegin = 0xFF; /**< Constant value for . */
  const uint8_t usFrameSize = sizeof(stWimosPortValues)+5; /**< Frame size value. */

  uint8_t ucMessageFrom;/**< Address of system what send the message . */
  uint8_t ucMessageTo;/**< The message is for this address . */

  stWimosPortValues stPortValues; /**< Wimos sensors values . */
  
  uint8_t checksum; /**< checksum value. */
} stWimosPortValuesMessage;

/**
 * @brief ACK message frame.
 */
typedef struct _ackMessage{
  const uint8_t ucBegin = 0xFF; /**< Constant frame begin value. */
  const uint8_t ucFrameSize = 0x01; /**< Const size value. */
  uint8_t ucLastChecksum; /**< checksum value. */
} stWimosACK;





/***************************************************************
 * Global Functions definition:
 ***************************************************************/

/**
 * @brief Core Wimos function.
 */
void coreWimos(void);
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
 * @brief Initialize Battery Reader.
 */
void initBattery(void);

/**
 * @brief Update battery status.
 */
void updateStatusBattery(stWimosInfo* _stWimosInfo);

/**
 * @brief Communication thread pointer.
 */
void (*communicationThread)(void);

/**
 * @brief Thread for wimos display (TV or RF).
 */
void (*coreWimosDisplay)(void);

/**
 * @brief Initialize communication state.
 */
void initRF(void);

/**
 * @brief Initialize IMU.
 */
void initIMU(void);

/**
 * @brief Update IMU value.
 */
void readIMU(stWimosPortValues* stWimosPort);
/**
 * @brief Initialize Analog Ports.
 */
void initPortA(void);
/**
 * @brief Update Analog Ports values.
 */
void readPortA(stWimosPortValues* stWimosPort);

/**
 * @brief Initialize the internal peripherals.
 */
void initInterPeriph(void);
/**
 * @brief Initialize Digital Port.
 */
void initPortD(void);
/**
 * @brief Update Digital Ports values.
 */
void readPortD(stWimosPortValues* stWimosPort);
/**
 * @brief Initialize I2C Port.
 */
void initPortI2C(void);
/**
 * @brief Update I2C Ports values.
 */
void readPortI2C(stWimosPortValues* stWimosPort);
/**
 * @brief No operation function.
 */
void noOperation(void);

#ifdef WIMOS_DEBUG
  void initDebug(void);
  void debugCommand(void);
#endif

void stopTV(void);

uint32_t coreWimosTVTimer;

int8_t storeVolatile(void* ptrBuffer, uint8_t ucBufferSize);

#endif
