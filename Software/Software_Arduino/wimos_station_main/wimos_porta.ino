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
 * @file wimos_porta.ino
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
 * @brief Analog Port Initialization.
 *
 * This function initializes the Analog ports interface.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
extern void initPortA(void){
  analogReadResolution(16);
  #ifdef _EN_WIMOS_PORT_A1
      
    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_A1_MASK;
    DEBUG_OK("Analog port 1 initialized.");
  #else

    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_A1_MASK;
    DEBUG_INFO("Analog port 1 not initialized.");
  #endif
  #ifdef _EN_WIMOS_PORT_A2

    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_A2_MASK;
    DEBUG_OK("Analog port 2 initialized.");
  #else

    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_A2_MASK;
    DEBUG_INFO("Analog port 2 not initialized.");
  #endif
  #ifdef _EN_WIMOS_PORT_A3

    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_A3_MASK;
    DEBUG_OK("Analog port 3 initialized.");
  #else

    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_A3_MASK;
    DEBUG_INFO("Analog port 3 not initialized.");
  #endif
  #ifdef _EN_WIMOS_PORT_A4

    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_A4_MASK;
    DEBUG_OK("Analog port 4 initialized.");
  #else

    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_A4_MASK;
    DEBUG_INFO("Analog port 4 not initialized.");
  #endif
  #ifdef _EN_WIMOS_PORT_A5

    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_A5_MASK;
    DEBUG_OK("Analog port 5 initialized.");
  #else

    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_A5_MASK;
    DEBUG_INFO("Analog port 5 not initialized.");
  #endif
}
/**
 * @brief Analog ports updater.
 *
 * This function updates all Analog ports value.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return _stWimosInfo It returns Analog ports values updated into stWimosInfo struct.
 */
extern void readPortA(stWimosPortValues* stWimosPort){
  #ifdef _EN_WIMOS_PORT_A1
    stWimosPort->usPortA1 = analogRead(WIMOS_A1_PORT);
  #endif
  #ifdef _EN_WIMOS_PORT_A2
    stWimosPort->usPortA2 = analogRead(WIMOS_A2_PORT);  
  #endif
  #ifdef _EN_WIMOS_PORT_A3
    stWimosPort->usPortA3 = analogRead(WIMOS_A3_PORT);   
  #endif
  #ifdef _EN_WIMOS_PORT_A4
    stWimosPort->usPortA4 = analogRead(WIMOS_A4_PORT);   
  #endif
  #ifdef _EN_WIMOS_PORT_A5
    stWimosPort->usPort5A1 = analogRead(WIMOS_A5_1_PORT); 
    stWimosPort->usPort5A2 = analogRead(WIMOS_A5_2_PORT); 
    stWimosPort->usPort5A3 = analogRead(WIMOS_A5_3_PORT); 
  
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
     extern void _test_n3UT36 (void){
      const char* testName = "n3.UT36 = %d";
      /*TODO: do a Analog Port initializer.*/
      DEBUG_VALID(testName , 
                 (true), 
                 (true));
    } 


    /**
     * @brief Wimos test n3.UT37.
     *
     * Unit test n3.UT37 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n3UT37 (void){
      const char* testName = "n3.UT37 = %d";
      /*TODO: Analog reader.*/
        DEBUG_VALID(testName , 
                   (true), 
                   (true));
    } 

    
  #endif
  
  #ifdef WIMOS_VALIDATION_TEST
  
    
    /**
     * @brief Wimos test n3.VT09.
     *
     * Unit test n3.VT09 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
    extern void _test_n3VT09 (void){
      const char* testName = "n3.VT09 = %ld microseconds";
      /*TODO: do a general validation.*/            
      DEBUG_VALID(testName , 
                 (true), 
                 (true));
    }
  #endif
#endif
