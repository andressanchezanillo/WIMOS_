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
 * @file wimos_port.ino
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main function for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */


#ifdef __SAM3X8E__
  
  #include "_setting.h"
  #include "main_config.h"
  
  stWimosI2CRegister stWimosI2CConfig[_WIMOS_PORT_I2C_CONFIG_SIZE] = _WIMOS_I2C_CONFIG;
  
  stWimosI2CRegister stWimosI2CData[_WIMOS_PORT_I2C_DATA_SIZE] = _WIMOS_I2C_DATA;
  
  void I2CWrite(uint8_t ucDeviceAddress, uint8_t ucRegisterAddress, uint8_t ucValue);
  uint8_t I2CRead(uint8_t ucDeviceAddress, uint16_t ucRegisterAddress);
  
  
  /**
   * @brief I2C port Initialization.
   *
   * This function initializes the I2C interface.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void initPortI2C(void){
    #ifdef _EN_WIMOS_PORT_I2C
      if(stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_IMU_MASK == 0x00){
        Wire.begin();
        for(uint8_t i=0; i<_WIMOS_PORT_I2C_CONFIG_SIZE ; i++){        
          I2CWrite(stWimosI2CData[i].ucDeviceAddress, stWimosI2CData[i].ucRegisterAddress, stWimosI2CData[i].ucRegisterValue);
        }  
      }
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_I2C_MASK;
      DEBUG_OK("I2C port initialized.");
    #else
    
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_I2C_MASK;
      DEBUG_INFO("I2C port not initialized.");
    #endif
  }
  
  /**
   * @brief I2C devices updater.
   *
   * This function updates the I2C devices values.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return _stWimosInfo It returns the I2C devices values updated into stWimosInfo struct .
   */
  extern void readPortI2C(stWimosPortValues* stWimosPort){
    for(uint8_t i=0; i<_WIMOS_PORT_I2C_DATA_SIZE; i++){
      stWimosPort->ucPortI2C[i] = I2CRead(stWimosI2CData[i].ucDeviceAddress, stWimosI2CData[i].ucRegisterAddress);
    }
    return;
  }
  
  
  void I2CWrite(uint8_t ucDeviceAddress, uint16_t ucRegisterAddress, uint8_t ucValue){  
      Wire.beginTransmission((int)ucDeviceAddress);
      Wire.write((int)ucRegisterAddress);
      Wire.write(ucValue);
      Wire.endTransmission();  
  }
  
  
  uint8_t I2CRead(uint8_t ucDeviceAddress, uint16_t ucRegisterAddress){
      Wire.beginTransmission((int)ucDeviceAddress);   
      Wire.write((int)ucRegisterAddress);                              
      Wire.requestFrom((int)ucDeviceAddress,1);         
      uint8_t ucRetValue = Wire.read();      
      Wire.endTransmission();   
      return ucRetValue;  
  }
  
  
  
  
  #ifdef WIMOS_DEBUG
    #ifdef WIMOS_UNIT_TEST
  
      /**
       * @brief Wimos test n3.UT33.
       *
       * Unit test n3.UT33 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT33 (void){
        const char* testName = "n3.UT33 = %d";
        /*TODO: do a Port I2C initializer.*/
          DEBUG_VALID(testName , 
                     (true), 
                     (true));
      } 
  
  
      /**
       * @brief Wimos test n3.UT34.
       *
       * Unit test n3.UT34 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT34 (void){
        const char* testName = "n3.UT34 = %d";
        /*TODO: I2C writer.*/
          DEBUG_VALID(testName , 
                     (true), 
                     (true));
      } 
  
  
      /**
       * @brief Wimos test n3.UT35.
       *
       * Unit test n3.UT35 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT35 (void){
        const char* testName = "n3.UT35 = %d";
        /*TODO: I2C reader.*/
          DEBUG_VALID(testName , 
                     (true), 
                     (true));
      } 
  
      
    #endif
    
    #ifdef WIMOS_VALIDATION_TEST
    
      
      /**
       * @brief Wimos test n3.VT08.
       *
       * Unit test n3.VT08 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
      extern void _test_n3VT08 (void){
        const char* testName = "n3.VT08 = %ld microseconds";
        /*TODO: do a general validation.*/            
        DEBUG_VALID(testName , 
                   (true), 
                   (true));
      }
    #endif
  #endif
#endif  
  

