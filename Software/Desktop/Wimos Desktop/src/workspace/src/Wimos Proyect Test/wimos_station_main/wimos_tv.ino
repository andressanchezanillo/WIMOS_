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
 * @file wimos_tv.ino
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main function for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */
 
#ifdef __SAM3X8E__
  #include <VGA.h>
  #include "_image.h"
  #include "_setting.h"
  #include "main_config.h"
  
  
  uint32_t ulTimerTV = 0;
  void displayTV(void);
  void displayMainScreen(const uint8_t* ucImage, uint16_t usSize);
  
  /**
   * @brief TV (RCA) Initialization.
   *
   * This function initializes the TV interface.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void initTV(void){
    #ifdef _EN_WIMOS_TV
      if(_EN_WIMOS_TV_TIME_SEC != 0){
        VGA.beginPAL();
        coreWimosDisplay = displayTV;
        coreWimosTVTimer = millis();
        ulTimerTV = millis();
        displayMainScreen(mainWindows,MAIN_WINDOWS_SIZE);
        stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus |= WIMOS_DEVICE_TV_MASK;
        DEBUG_DATA("TV Time Counter for %d Seconds",_EN_WIMOS_TV_TIME_SEC);
        DEBUG_OK("TV output signal initialized.");
      }else{
        coreWimosDisplay = communicationThread;
        stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus &= ~WIMOS_DEVICE_TV_MASK;
        DEBUG_INFO("TV output signal not initialized.");
        initInterPeriph();
      }
    #else
      coreWimosDisplay = communicationThread;
      stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus &= ~WIMOS_DEVICE_TV_MASK;
      DEBUG_INFO("TV output signal not initialized.");
      initInterPeriph();
    #endif
    
    if(coreWimosDisplay == NULL){
      coreWimosDisplay = noOperation;
    }
  }
  
  
  /**
   * @brief Displayer the main screen image.
   *
   * This function displays the main screen.
   * @param ucImage: Pointer to image byte matrix.
   * @param usSizeX: Size of image for X axi.
   * @param usSizeY: Size of image for Y axi.
   * @return none.
   */
  void displayMainScreen(const uint8_t* ucImage, uint16_t usSize){
    uint16_t i, b;
    for( i=0; i<usSize ; i++ ){
        for( b=0; b<8 ; b++ ){
          VGA.drawPixel(((i*8)%SCREEN_SIZE_X)+b,((i*8)/SCREEN_SIZE_X),((uint8_t)0x80 & ((uint8_t)(ucImage[i]) << b)));
        }
    }
    
    return;
  }
  
  /**
   * @brief TV displayer.
   *
   * This function displays each system values (max. 3).
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  void displayTV(void){
    #if defined(_EN_WIMOS_TV)
    
      /*Displaying sensor values.*/
      
      if(((millis() - coreWimosTVTimer)/1000) >=  _EN_WIMOS_TV_TIME_SEC ){
        DEBUG_INFO("TV DISPLAY STOPPED.");
        stopTV();
      }
      else{      
        DEBUG_DATA("TV TIMER COUNTER = %d seconds.",( _EN_WIMOS_TV_TIME_SEC - ((millis() - coreWimosTVTimer)/1000)) );
      }
      
    #else
      coreWimosDisplay = communicationThread;
      coreWimosTVTimer = 0;
    #endif
  }
  
  
  
  #ifdef WIMOS_DEBUG
    #ifdef WIMOS_UNIT_TEST
  
      /**
       * @brief Wimos test n3.UT36.
       *
       * Unit test n3.UT36 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT40 (void){
        const char* testName = "n3.UT40 = %d";
        /*TODO: do a Tv Signal initializer.*/
        DEBUG_VALID(testName , 
                   (true), 
                   (true));
      } 
  
  
      /**
       * @brief Wimos test n3.UT41.
       *
       * Unit test n3.UT41 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT41 (void){
        const char* testName = "n3.UT41 = %d";
        /*TODO: Screen test.*/
          DEBUG_VALID(testName , 
                     (true), 
                     (true));
      } 
  
      
    #endif
    
    #ifdef WIMOS_VALIDATION_TEST
    
      
      /**
       * @brief Wimos test n3.VT11.
       *
       * Unit test n3.VT11 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
      extern void _test_n3VT11 (void){
        const char* testName = "n3.VT11 = %ld microseconds";
        /*TODO: do a general validation.*/            
        DEBUG_VALID(testName , 
                   (true), 
                   (true));
      }
    #endif
  #endif
#endif
