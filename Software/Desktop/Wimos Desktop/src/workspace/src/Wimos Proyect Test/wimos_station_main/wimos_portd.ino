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
 * @file wimos_portd.ino
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
  
  
  
  
  /**
   * @brief Digital Port Initialization.
   *
   * This function initializes the Digital ports interface.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void initPortD(void){
    #ifdef _EN_WIMOS_PORT_D1
      pinMode(WIMOS_D1_1_PORT,_WIMOS_PORT_MODE_D1_1);
      pinMode(WIMOS_D1_2_PORT,_WIMOS_PORT_MODE_D1_2);
      pinMode(WIMOS_D1_3_PORT,_WIMOS_PORT_MODE_D1_3);
      pinMode(WIMOS_D1_4_PORT,_WIMOS_PORT_MODE_D1_4);
      
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_D1_MASK;
      DEBUG_OK("Digital port 1 initialized.");
    #else
    
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_D1_MASK;
      DEBUG_INFO("Digital port 1 not initialized.");
    #endif
    #ifdef _EN_WIMOS_PORT_D2
      pinMode(WIMOS_D2_1_PORT,_WIMOS_PORT_MODE_D2_1);
      pinMode(WIMOS_D2_2_PORT,_WIMOS_PORT_MODE_D2_2);
      pinMode(WIMOS_D2_3_PORT,_WIMOS_PORT_MODE_D2_3);
      
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_D2_MASK;
      DEBUG_OK("Digital port 2 initialized.");
    #else
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_D2_MASK;
      DEBUG_INFO("Digital port 2 not initialized.");
    #endif
    #ifdef _EN_WIMOS_PORT_D3
      pinMode(WIMOS_D3_1_PORT,_WIMOS_PORT_MODE_D3_1);
      pinMode(WIMOS_D3_2_PORT,_WIMOS_PORT_MODE_D3_2);
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_D3_MASK;
      DEBUG_OK("Digital port 3 initialized.");
    #else
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_D3_MASK;
      DEBUG_INFO("Digital port 3 not initialized.");
    #endif
    #ifdef _EN_WIMOS_PORT_D4
      pinMode(WIMOS_D4_1_PORT,_WIMOS_PORT_MODE_D4_1);
      pinMode(WIMOS_D4_2_PORT,_WIMOS_PORT_MODE_D4_2);
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_D4_MASK;
      DEBUG_OK("Digital port 4 initialized.");
    #else
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_D4_MASK;
      DEBUG_INFO("Digital port 4 not initialized.");
    #endif
    #ifdef _EN_WIMOS_PORT_D5
      pinMode(WIMOS_D5_1_PORT,_WIMOS_PORT_MODE_D5_1);
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_D5_MASK;
      DEBUG_OK("Digital port 5 initialized.");
    #else
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_D5_MASK;
      DEBUG_INFO("Digital port 5 not initialized.");
    #endif
  }
  
  
  /**
   * @brief Digital ports updater.
   *
   * This function updates the Digital ports value.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return _stWimosInfo It returns Digital ports values updated into stWimosInfo struct.
   */
  extern void readPortD(stWimosPortValues* stWimosPort){
    #ifdef _EN_WIMOS_PORT_D1
    stWimosPort->usPortD1 = (digitalRead(WIMOS_D1_1_PORT) | digitalRead(WIMOS_D1_2_PORT) << 1 | digitalRead(WIMOS_D1_3_PORT) << 2 | digitalRead(WIMOS_D1_4_PORT) << 3);
    #endif
    #ifdef _EN_WIMOS_PORT_D2
    stWimosPort->usPortD2 = (digitalRead(WIMOS_D2_1_PORT) | digitalRead(WIMOS_D2_2_PORT) << 1 | digitalRead(WIMOS_D2_3_PORT) << 2 );
    
    #endif
    #ifdef _EN_WIMOS_PORT_D3
    stWimosPort->usPortD3 = (digitalRead(WIMOS_D3_1_PORT) | digitalRead(WIMOS_D3_2_PORT) << 1 );
    
    #endif
    #ifdef _EN_WIMOS_PORT_D4
    stWimosPort->usPortD4 = (digitalRead(WIMOS_D4_1_PORT) | digitalRead(WIMOS_D4_2_PORT) << 1 );
    
    #endif
    #ifdef _EN_WIMOS_PORT_D5
    stWimosPort->usPortD5 = (digitalRead(WIMOS_D4_1_PORT) );
    
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
       extern void _test_n3UT38 (void){
        const char* testName = "n3.UT38 = %d";
        /*TODO: do a Digital Port initializer.*/
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
       extern void _test_n3UT39 (void){
        const char* testName = "n3.UT39 = %d";
        /*TODO: Digital reader.*/
          DEBUG_VALID(testName , 
                     (true), 
                     (true));
      } 
  
      
    #endif
    
    #ifdef WIMOS_VALIDATION_TEST
    
      
      /**
       * @brief Wimos test n3.VT10.
       *
       * Unit test n3.VT10 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
      extern void _test_n3VT10 (void){
        const char* testName = "n3.VT10 = %ld microseconds";
        /*TODO: do a general validation.*/            
        DEBUG_VALID(testName , 
                   (true), 
                   (true));
      }
    #endif
  #endif
#endif
