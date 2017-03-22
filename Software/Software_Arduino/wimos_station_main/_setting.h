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
 * @file _setting.h
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing variable setting for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */

#ifndef _SETTING_H_
#define _SETTING_H_
  
  
  
  /**
   * @brief Label Debug traces activation.
   */
  //#define WIMOS_DEBUG
  //#define WIMOS_UNIT_TEST
  //#define DEBUG_COMM_RXTX
  //#define DEBUG_COMM_STATUS
  #define DEBUG_ANALOG_A5
  //#define WIMOS_VALIDATION_TEST
  #define DEBUG_COLOR
  
  /**
   * @brief Label for RF activation.
   */
  #define _EN_WIMOS_RF
  
  
  #ifdef __SAM3X8E__  
  
    #define WIMOS_ID 0x10
    
    #define _WIMOS_IMU
    /**
     * @brief Label for GPS activation.
     */
    #define _EN_WIMOS_GPS
    /**
     * @brief Label for SD activation.
     */
    #define _EN_WIMOS_SD
    /**
     * @brief Label for IMU activation.
     */
    #define _EN_WIMOS_IMU
    /**
     * @brief Label for Battery Reader activation.
     */
    #define _EN_WIMOS_BAT
    /**
     * @brief Label for TV activation.
     */
    #define _EN_WIMOS_TV
    /**
     * @brief Time for TV display.
     */
    #define _EN_WIMOS_TV_TIME_SEC ((uint16_t) 10)
    
    /**
     * @brief Label for Analog Port A1 activation.
     */
    #define _EN_WIMOS_PORT_A1
    /**
     * @brief Label for Analog Port A2 activation.
     */
    #define _EN_WIMOS_PORT_A2
    /**
     * @brief Label for Analog Port A3 activation.
     */
    #define _EN_WIMOS_PORT_A3
    /**
     * @brief Label for Analog Port A4 activation.
     */
    #define _EN_WIMOS_PORT_A4
    /**
     * @brief Label for Analog Port A5 activation.
     */
    #define _EN_WIMOS_PORT_A5
    
    /**
     * @brief Label for Digital Port D1 activation.
     */
    #define _EN_WIMOS_PORT_D1
    /**
     * @brief Label for Digital Port D1.1 Mode.
     */
    #define _WIMOS_PORT_MODE_D1_1 INPUT
    /**
     * @brief Label for Digital Port D1.2 Mode.
     */
    #define _WIMOS_PORT_MODE_D1_2 INPUT
    /**
     * @brief Label for Digital Port D1.3 Mode.
     */
    #define _WIMOS_PORT_MODE_D1_3 INPUT
    /**
     * @brief Label for Digital Port D1.4 Mode.
     */
    #define _WIMOS_PORT_MODE_D1_4 INPUT
    
    /**
     * @brief Label for Digital Port D2 activation.
     */
    #define _EN_WIMOS_PORT_D2
    /**
     * @brief Label for Digital Port D2.1 Mode.
     */
    #define _WIMOS_PORT_MODE_D2_1 INPUT
    /**
     * @brief Label for Digital Port D2.2 Mode.
     */
    #define _WIMOS_PORT_MODE_D2_2 INPUT
    /**
     * @brief Label for Digital Port D2.3 Mode.
     */
    #define _WIMOS_PORT_MODE_D2_3 INPUT
    
    /**
     * @brief Label for Digital Port D3 activation.
     */
    #define _EN_WIMOS_PORT_D3
    /**
     * @brief Label for Digital Port D3.1 Mode.
     */
    #define _WIMOS_PORT_MODE_D3_1 INPUT
    /**
     * @brief Label for Digital Port D3.2 Mode.
     */
    #define _WIMOS_PORT_MODE_D3_2 INPUT
    
    /**
     * @brief Label for Digital Port D4 activation.
     */
    #define _EN_WIMOS_PORT_D4
    /**
     * @brief Label for Digital Port D4.1 Mode.
     */
    #define _WIMOS_PORT_MODE_D4_1 INPUT
    /**
     * @brief Label for Digital Port D4.2 Mode.
     */
    #define _WIMOS_PORT_MODE_D4_2 INPUT
    
    /**
     * @brief Label for Digital Port D5 activation.
     */
    #define _EN_WIMOS_PORT_D5
    /**
     * @brief Label for Digital Port D5.1 Mode.
     */
    #define _WIMOS_PORT_MODE_D5_1 INPUT
    
    /**
     * @brief Label for UART Port activation.
     */
    #define _EN_WIMOS_PORT_UART
    /**
     * @brief Label for I2C Port activation.
     */
    #define _EN_WIMOS_PORT_I2C
    /**
     * @brief Size of I2C config address devices.
     */
    #define _WIMOS_PORT_I2C_CONFIG_SIZE 0x02
    /**
     * @brief Size of I2C list devices.
     */
    #define _WIMOS_PORT_I2C_DATA_SIZE 0x02
    
    #define _WIMOS_I2C_CONFIG { \
                                (stWimosI2CRegister) { .eRegisterType = eConfigWrite, .ucDeviceAddress = 0x68, .ucRegisterAddress = 0x00, .ucRegisterValue = 0x01 }, \
                                (stWimosI2CRegister) { .eRegisterType = eConfigWrite, .ucDeviceAddress = 0x68,  .ucRegisterAddress = 0x00, .ucRegisterValue = 0x01 }  \                                                                   
                              }
                              
    
    #define _WIMOS_I2C_DATA { \
                              (stWimosI2CRegister) { .eRegisterType = eDataRead, .ucDeviceAddress = 0x68,  .ucRegisterAddress = 0x00, .ucRegisterValue = 0x01 }, \
                              (stWimosI2CRegister) { .eRegisterType = eDataRead, .ucDeviceAddress = 0x68,  .ucRegisterAddress = 0x01, .ucRegisterValue = 0x02 }  \                                                                   
                            }
                            
    #define _WIMOS_5A1_OFFSET_1                 (- VCC_LOGIC / 2)
    #define _WIMOS_5A1_COEFICIENT_1             (float)(GRAVITY_MM_S2/(VCC_LOGIC/5))
    #define _WIMOS_5A1_OFFSET_2                 0x00
    #define _WIMOS_5A1_COEFICIENT_2             0x01
    
    #define _WIMOS_5A2_OFFSET_1                 (- VCC_LOGIC / 2)
    #define _WIMOS_5A2_COEFICIENT_1             (float)(GRAVITY_MM_S2/(VCC_LOGIC/5))
    #define _WIMOS_5A2_OFFSET_2                 0x00
    #define _WIMOS_5A2_COEFICIENT_2             0x01
    
    #define _WIMOS_5A3_OFFSET_1                 (- VCC_LOGIC / 2)
    #define _WIMOS_5A3_COEFICIENT_1             (float)(GRAVITY_MM_S2/(VCC_LOGIC/5))
    #define _WIMOS_5A3_OFFSET_2                 0x00
    #define _WIMOS_5A3_COEFICIENT_2             0x01
    
    #define _WIMOS_5A_OPERATOR_JOINT(x,y,z)     MATH_VECTOR3D_LENGHT(x,y,z)
    
    #define _WIMOS_5A_AVERAGE_SIZE              ((uint8_t)50)
    #define _WIMOS_5A_AVERAGE_OFFSET            ((uint8_t)120)
    #define _WIMOS_5A_OFFSET_MAX_SIZE           ((uint8_t)10)
    
    #define _WIMOS_5A_PROCESSOR(lastValue, inputValue, currentIndex)                 MATH_MOVING_AVERAGE(lastValue, inputValue, currentIndex, _WIMOS_5A_AVERAGE_SIZE)
    
    #define _WIMOS_5A_DETECTION(inputValue)     detectionA5Default(inputValue)
    #define _WIMOS_ALERT_TIMEOUT                ((uint32_t)0x1388u)
    #ifdef _EN_WIMOS_TV
      /**
       * @brief Define to element 1 to display over TV signal.
       */
      #define _WIMOS_TV_D1  &stWimosInformation.stDateTime.ucDate
      /**
       * @brief Define to element 2 to display over TV signal.
       */
      #define _WIMOS_TV_D2  &stWimosInformation.stDateTime.ucDate
      /**
       * @brief Define to element 3 to display over TV signal.
       */
      #define _WIMOS_TV_D3  &stWimosInformation.stDateTime.ucDate

      
    #endif
  #endif
  
  
  
  
  
  
  #ifdef __AVR_ATmega32U4__
  
    #define WIMOS_ID 0x00
    
    #define TIME_COMMAND_RATE 60000
    
      #if defined(WIMOS_UNIT_TEST) && defined(WIMOS_DEBUG)
        #define _WIMOS_COMMAND_LIST_SIZE 0x01 
        #define _WIMOS_COMMAND_LIST {\
                                      (stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 0x10, .ucCommand = COMMAND_GET_GENERAL_INFO, .ucChecksum = 0x10 } \                                                       
                                    }  
      #else
    
        #define _WIMOS_COMMAND_LIST_SIZE 0x07
        #define _WIMOS_COMMAND_LIST {\
                                      (stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 0x10, .ucCommand = COMMAND_GET_GENERAL_INFO, .ucChecksum = 0x00 },\
                                      (stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 0x10, .ucCommand = COMMAND_GET_ALERT_LIST, .ucChecksum = 0x00 }, \ 
                                      (stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 0x10, .ucCommand = COMMAND_GET_ALERT_LIST, .ucChecksum = 0x00 }, \ 
                                      (stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 0x10, .ucCommand = COMMAND_GET_ALERT_LIST, .ucChecksum = 0x00 }, \ 
                                      (stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 0x10, .ucCommand = COMMAND_GET_ALERT_LIST, .ucChecksum = 0x00 }, \ 
                                      (stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 0x10, .ucCommand = COMMAND_GET_ALERT_LIST, .ucChecksum = 0x00 }, \ 
                                      (stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 0x10, .ucCommand = COMMAND_GET_ALERT_LIST, .ucChecksum = 0x00 } \                                                        
                                    }      
      #endif
  #endif
#endif
