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
 * @file wimos.ino
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main function for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */

#include "_setting.h"
#include "main_config.h"


void initExterPeriph(void);
/**
 * @brief Thread for wimos display (TV or RF).
 */
extern void (*coreWimosDisplay)(void);
/**
 * @brief The Time of TV begin.
 */
extern uint32_t coreWimosCriticalTimer = 0;
/**
 * @brief The Time of TV begin.
 */
extern uint32_t coreWimosInfoTimer = 0;

/**
 * @brief Wimos initialization service.
 *
 * This function initializes all threads, devices and ports system.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */
void initWimosService(void){
      
      #ifdef __AVR_ATmega32U4__
        initDebug();
      #endif
      
      #ifdef __SAM3X8E__
        initDebug();
      #endif
      
      initInterPeriph();
      #ifdef __SAM3X8E__
        initExterPeriph(); 
        coreWimosInfoTimer = millis();
      #endif 
      coreWimosCriticalTimer = millis();
}

/**
 * @brief Wimos initialization.
 *
 * This function initializes all threads, devices and ports system.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */
extern void initWimos(void){  
#if defined(WIMOS_UNIT_TEST) and defined(WIMOS_DEBUG)
    
    initDebug();
    #ifdef __SAM3X8E__
      _test_n3UT00();
      _test_n3UT01();
      _test_n3UT02();
      _test_n3UT03();
      _test_n3UT04();
      _test_n3UT05();
      _test_n3UT06();
      _test_n3UT07();
      _test_n3UT10();
      _test_n3UT11();
      _test_n3UT12();
      _test_n3UT13();
      _test_n3UT14();
      _test_n3UT15();
      _test_n3UT16();
      _test_n3UT17();
      _test_n3UT18();
      _test_n3UT19();
      _test_n3UT20();
      _test_n3UT21();
      _test_n3UT22();
      _test_n3UT23();
      _test_n3UT24();
      _test_n3UT25();
      _test_n3UT26();
      _test_n3UT27();
      _test_n3UT28();
      _test_n3UT29();
      _test_n3UT30();
      _test_n3UT31();
      _test_n3UT32();
      _test_n3UT33();
      _test_n3UT34();
      _test_n3UT35();
      _test_n3UT36();
      _test_n3UT37();
      _test_n3UT38();
      _test_n3UT39();
      _test_n3UT40();
      _test_n3UT41();
    #endif
      _test_n4UT01();
      _test_n4UT02();
      _test_n4UT03();
      _test_n4UT04();
      _test_n4UT05();
      #ifdef __SAM3X8E__
        _test_n4UT06 ();
        _test_n4UT07 ();
        _test_n4UT08 ();
        _test_n4UT09 ();
        _test_n4UT10 ();
      #endif
      #ifdef __AVR_ATmega32U4__        
        _test_n4UT11 ();
      #endif      
      _test_n4UT12 ();
      _test_n4UT13 ();
      _test_n4UT14 ();
      _test_n4UT15 ();
      _test_n4UT16 ();
      _test_n4UT17 ();
      #ifdef __SAM3X8E__
        _test_n4UT18 ();
      #endif
      #ifdef __AVR_ATmega32U4__  
        _test_n4UT19 ();
        _test_n4UT20 ();
      #endif    
      #ifdef __SAM3X8E__
        _test_n5UT01();
        _test_n5UT02();
        _test_n5UT03();
        _test_n5UT04();
        _test_n5UT05();
        _test_n5UT06();
        _test_n5UT07();
        _test_n5UT08();
        _test_n5UT09();
        _test_n5UT10();
        _test_n5UT11();
        _test_n5UT12();
        _test_n5UT13();
        _test_n5UT14();
        _test_n5UT15();
        _test_n5UT16();
        _test_n5UT17();
        _test_n5UT18();
        _test_n5UT19();
        _test_n5UT20();
        _test_n5UT21();
        _test_n5UT22();
        _test_n5UT23();
        _test_n5UT24();
        _test_n5UT25();
        _test_n5UT26();
        _test_n5UT27();
        _test_n5UT28();
        _test_n5UT29();
        _test_n5UT30();
        _test_n5UT31();
        _test_n5UT32();
        _test_n5UT33();
        _test_n5UT34();
      #endif
      #ifdef __SAM3X8E__
        _test_n6UT01();
        _test_n6UT02();
        _test_n6UT03();
        _test_n6UT04();
        _test_n6UT05();
        _test_n6UT06();
        _test_n6UT07();
        _test_n6UT08();
        _test_n6UT09();
        _test_n6UT10();
      #endif
      
      while(true);
  #else 
    #if defined(WIMOS_VALIDATION_TEST) and defined(WIMOS_DEBUG)
      initDebug();
      
      #ifdef __SAM3X8E__
        _test_n3VT01();
        _test_n3VT02();
        _test_n3VT03();
        _test_n3VT05();
        _test_n3VT06();
        _test_n3VT07();
        _test_n3VT08();
        _test_n3VT09();
        _test_n3VT10();
        _test_n3VT11();
      #endif
      #ifdef __AVR_ATmega32U4__  
        _test_n4VT01();
      #endif
      
      while(true);
    #else
      initWimosService();
      #ifdef __AVR_ATmega32U4__
            delay(3000);
            while(SERIAL_USB.available())
              SERIAL_USB.read();
            SERIAL_USB.println("FRAME1: Begin.");
      #endif
    #endif
  #endif

}

extern void initInterPeriph(void){
  DEBUG_INFO("Starting internal peripherals.");
  initRF();
  #ifdef __SAM3X8E__
    initBattery();
    initGPS();  
    initIMU();  
    stGlobalWimosInfoMsg.stInfo.ucPercentMemory = initSD();
  #endif
}

extern void initExterPeriph(void){
  DEBUG_INFO("Starting external peripherals.");
  #ifdef __SAM3X8E__
    initPortA();
    initPortD();
    initPortI2C();
  #endif
}

void updateInfoWimos(){
  #ifdef __SAM3X8E__
    #ifdef _EN_WIMOS_GPS
      updateGPS(&stGlobalWimosInfoMsg.stInfo);
    #endif
    #ifdef _EN_WIMOS_BAT
      updateStatusBattery(&stGlobalWimosInfoMsg.stInfo);
    #endif
  #endif
}


void readSensorsWimos(){
  #ifdef __SAM3X8E__
    readIMU(&stGlobalWimosPortMsg.stPortValues);
    readPortI2C(&stGlobalWimosPortMsg.stPortValues);
    readPortA(&stGlobalWimosPortMsg.stPortValues);
    readPortD(&stGlobalWimosPortMsg.stPortValues);
  #endif
}

/**
 * @brief Wimos RF thread.
 *
 * This thread update all values for sending over RF.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */

extern void coreWimos(void){
  
  communicationThread();
  #ifdef __SAM3X8E__
    #ifdef WIMOS_DEBUG
      debugCommand();
    #endif
    if((millis() - coreWimosCriticalTimer) > CRITICAL_REFRESH_MS){
      coreWimosCriticalTimer = millis();
      readSensorsWimos();
    }
    communicationThread();
    if((millis() - coreWimosInfoTimer) > INFO_REFRESH_MS){
      coreWimosInfoTimer = millis();
      updateInfoWimos();
    }
  #endif
}


#ifdef WIMOS_DEBUG
  #ifdef WIMOS_UNIT_TEST
    
    /**
     * @brief Wimos test n3.UT00.
     *
     * Unit test n3.UT00 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     void _test_n3UT00 (void){
      const char* testName = "n3.UT00";
      /*Body_TEST:*/
      /*TODO: Check the initialization with bottom enabled.*/
      /*End_Body_TEST:*/
      
    } 
    
    /**
     * @brief Wimos test n3.UT01.
     *
     * Unit test n3.UT01 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     void _test_n3UT01 (void){
      const char* testName = "n3.UT01";
      /*Body_TEST:*/
      /*TODO: Check the initialization with bottom disabled.*/
      /*End_Body_TEST:*/
      
    } 

    /**
     * @brief Wimos test n3.UT01.
     *
     * Unit test n3.UT01 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     void _test_n3UT02 (void){
      const char* testName = "n3.UT02";
      /*Body_TEST:*/
      /*TODO: Check the timer control.*/      
      /*End_Body_TEST:*/
      
    } 
    
  #endif

  #ifdef WIMOS_VALIDATION_TEST
  
    
    /**
     * @brief Wimos test n3.VT01.
     *
     * Unit test n3.UT01 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     void _test_n3VT01 (void){
      const char* testName = "n3.VT01";
      /*Body_TEST:*/
      /*TODO: Validation the initialization wimos.*/ 
      
      DEBUG_VALID(testName , 
                 (true), 
                 (true));
      /*End_Body_TEST:*/
      
    } 
    
    
    /**
     * @brief Wimos test n3.VT02.
     *
     * Unit test n3.VT02 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     void _test_n3VT02 (void){
      const char* testName = "n3.VT02";
      /*Body_TEST:*/
      /*TODO: Validation the wimos timer.*/ 
      
      DEBUG_VALID(testName , 
                 (true), 
                 (true));
      /*End_Body_TEST:*/
      
    } 
    
  #endif
#endif
