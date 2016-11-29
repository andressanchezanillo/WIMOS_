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


/**
 * @brief Thread for wimos display (TV or RF).
 */
extern void (*coreWimosDisplay)(void);
/**
 * @brief The Time of TV begin.
 */
extern uint32_t coreWimosTVTimer;


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
  initDebug();
  
  initGPS();
  
  initRF();
  initTV();
  
  if(coreWimosDisplay == NULL){
    coreWimosDisplay = noOperation;
  }
  
  initBattery();

  initPortA();
  initPortD();
  initPortI2C();
  initIMU();
  
  stGlobalWimosInfoMsg.stInfo.ucPercentMemory = initSD();

}

extern void deleteWimos(void){
  
  deleteTV();
  //deleteSD();

}

void updateInfoWimos(){
  #ifdef _EN_WIMOS_GPS
    updateGPS(&stGlobalWimosInfoMsg.stInfo);
  #endif
  #ifdef _EN_WIMOS_SD
    //updateStatusSD(&stGlobalWimosInfoMsg.stInfo);
  #endif
  #ifdef _EN_WIMOS_BAT
    updateStatusBattery(&stGlobalWimosInfoMsg.stInfo);
  #endif
}


void readExternSensorsWimos(){
  /* TODO: lectura de sensores */
}

/**
 * @brief Wimos RF thread.
 *
 * This thread update all values for sending over RF.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
extern void coreWimos(){
  #ifdef WIMOS_DEBUG
    debugCommand();
  #endif
  //updateInfoWimos();
  
  /** Timer Control **/
  
}


#ifdef WIMOS_DEBUG
  #ifdef WIMOS_UNIT_TEST
    
    /**
     * @brief Wimos test n1.UT01.
     *
     * Unit test n1.UT01 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     void _test_n1UT01 (void){
      const char* testName = "n1.UT01";
      /*Body_TEST:*/
      
      /*End_Body_TEST:*/
      
    } 

    /**
     * @brief Wimos test n1.UT01.
     *
     * Unit test n1.UT01 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     void _test_n1UT02 (void){
      const char* testName = "n1.UT02";
      /*Body_TEST:*/
      
      /*End_Body_TEST:*/
      
    } 
    
  #endif

  #ifdef WIMOS_VALIDATION_TEST
  
    
    /**
     * @brief Wimos test n1.UT01.
     *
     * Unit test n1.UT01 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     void _test_n1VT01 (void){
      const char* testName = "n1.VT01";
      /*Body_TEST:*/
      
      /*End_Body_TEST:*/
      
    } 
    
    
    /**
     * @brief Wimos test n1.UT01.
     *
     * Unit test n1.UT01 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     void _test_n1VT02 (void){
      const char* testName = "n1.VT02";
      /*Body_TEST:*/
      
      /*End_Body_TEST:*/
      
    } 
    
  #endif
#endif
