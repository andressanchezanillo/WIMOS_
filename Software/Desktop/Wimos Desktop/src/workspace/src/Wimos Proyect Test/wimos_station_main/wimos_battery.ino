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
 * @file wimos_battery.ino
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main function for Wimos battery reader.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */

#ifdef __SAM3X8E__
  
  #include "_setting.h"
  #include "main_config.h"
  
  #ifdef WIMOS_UNIT_TEST
    int16_t ssUnitTestInput = 0;
  #endif
  
  /**
   * @brief Battery Reader Initialization.
   *
   * This function initializes the battery reader interface.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void initBattery(void){
    #ifdef _EN_WIMOS_BAT
      stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus |= WIMOS_DEVICE_BATT_MASK;
      DEBUG_OK("Battery Reader initialized.");
    #else
      stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus &= ~WIMOS_DEVICE_BATT_MASK;
      DEBUG_INFO("Battery Reader not initialized.");
    #endif
  }
  
  /**
   * @brief Battery Reader updater.
   *
   * This function updates the battery status value.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return _stWimosInfo It returns battery status updated into stWimosInfo struct.
   */
    extern void updateStatusBattery(stWimosInfo* _stWimosInfo){
      #ifdef _EN_WIMOS_BAT
        int32_t slVoltage = 0;
        #ifndef WIMOS_UNIT_TEST
          slVoltage  = (analogRead(WIMOS_BATT_PIN) * VCC_LOGIC)/ADC_MAX_VALUE;
        #else
          slVoltage  = (ssUnitTestInput * VCC_LOGIC)/ADC_MAX_VALUE;
        #endif
        slVoltage  *= ADC_VOLTAGE_TO_BAT_VOLTAGE;
        slVoltage = (float)((float)((float)slVoltage - (float)VCC_MIN_BATTERY) / ((float)VCC_MAX_BATTERY - (float)VCC_MIN_BATTERY))*100;
        
        if(slVoltage < 0)
          slVoltage = 0;
        
        if(slVoltage > 100)
          slVoltage = 100;
          
        _stWimosInfo->ucPercentBattery = (uint8_t)slVoltage;
      #endif
      return;
    }
  
  
  #ifdef WIMOS_DEBUG
    #ifdef WIMOS_UNIT_TEST
      
      /**
       * @brief Wimos test n3.UT03.
       *
       * Unit test n3.UT03 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT03 (void){
        const char* testName = "n3.UT03 = %d";
        /*Body_TEST:*/
        
        #ifdef _EN_WIMOS_BAT
          stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus = 0;
          initBattery();
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_BATT_MASK), 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_BATT_MASK) == WIMOS_DEVICE_BATT_MASK);
        #else
          stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus = WIMOS_DEVICE_BATT_MASK;
          initBattery();
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_BATT_MASK), 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_BATT_MASK) == 0x00);
        #endif
        
        /*End_Body_TEST:*/
      } 
  
      /**
       * @brief Wimos test n3.UT04.
       *
       * Unit test n3.UT04 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT04 (void){
        const char* testName = "n3.UT04 = %d";
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_BAT
          stGlobalWimosInfoMsg.stInfo.ucPercentBattery = 0;
          ssUnitTestInput = 2048;
          updateStatusBattery(&stGlobalWimosInfoMsg.stInfo);
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery), 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery == 100));
        #else
          uint8_t input = 0;
          stGlobalWimosInfoMsg.stInfo.ucPercentBattery = input;
          ssUnitTestInput = 2048;
          updateStatusBattery(&stGlobalWimosInfoMsg.stInfo);
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery), 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery == input));
        #endif
        /*End_Body_TEST:*/
      } 
  
  
      /**
       * @brief Wimos test n3.UT05.
       *
       * Unit test n3.UT05 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT05 (void){
        const char* testName = "n3.UT05 = %d";
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_BAT
          stGlobalWimosInfoMsg.stInfo.ucPercentBattery = 100;
          ssUnitTestInput = 0;
          updateStatusBattery(&stGlobalWimosInfoMsg.stInfo);
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery), 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery == 0));
        #else
          uint8_t input = 100;
          stGlobalWimosInfoMsg.stInfo.ucPercentBattery = input;
          ssUnitTestInput = 0;
          updateStatusBattery(&stGlobalWimosInfoMsg.stInfo);
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery), 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery == input));
        #endif
        /*End_Body_TEST:*/
      } 
  
      /**
       * @brief Wimos test n3.UT06.
       *
       * Unit test n3.UT06 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT06 (void){
        const char* testName = "n3.UT06 = %d";
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_BAT
          stGlobalWimosInfoMsg.stInfo.ucPercentBattery = 0;
          ssUnitTestInput = 788;
          updateStatusBattery(&stGlobalWimosInfoMsg.stInfo);
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery), 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery < 100 &&
                      stGlobalWimosInfoMsg.stInfo.ucPercentBattery > 0 ));
        #else
          uint8_t input = 0;
          stGlobalWimosInfoMsg.stInfo.ucPercentBattery = input;
          ssUnitTestInput = 788;
          updateStatusBattery(&stGlobalWimosInfoMsg.stInfo);
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery), 
                     (stGlobalWimosInfoMsg.stInfo.ucPercentBattery == input));
        #endif
        /*End_Body_TEST:*/
      } 
    #endif
  
    #ifdef WIMOS_VALIDATION_TEST
    
      
      /**
       * @brief Wimos test n3.VT03.
       *
       * Unit test n3.VT03 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
      extern void _test_n3VT03 (void){
        const char* testName = "n3.VT03 = %ld";
        
        /*Body_TEST:*/     
        uint32_t ulTimerVT = micros();
        updateStatusBattery(&stGlobalWimosInfoMsg.stInfo);
        DEBUG_VALID(testName , 
                   (micros() - ulTimerVT), 
                   (micros() - ulTimerVT < 100));
        /*End_Body_TEST:*/
      } 
      
    #endif
  #endif
#endif
