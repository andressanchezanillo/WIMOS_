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
 * @file wimos_debug.ino
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main function for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */


#include "_setting.h"
#include "main_config.h"

#ifdef WIMOS_DEBUG  //~(eINFO | eDATA);
  static uint8_t ucCurrentDebugMode = eDATA;

  
  extern void initDebug(void){
    SERIAL_DEBUG.begin(BAUDRATE_DEBUG);
  }
  
  extern void debug_print(const char* pFunction, const char* pLabel, const char* pData){
    uint8_t i=0;
    SERIAL_DEBUG.print(pFunction);
    
    for(i=strlen(pFunction); i<12; i++)
      SERIAL_DEBUG.print(" ");
      
    SERIAL_DEBUG.print(":\t");
    SERIAL_DEBUG.print(pLabel);
    
    for(i=strlen(pLabel); i<12; i++)
      SERIAL_DEBUG.print(" ");
      
    SERIAL_DEBUG.print("\t");
    SERIAL_DEBUG.println(pData);
  }
  
  
  extern void debug(const char* pFunction, const char* pLabel, const char* pData, eDebugMode eMode){
    if( eMode == eERROR || eMode == eOK ){
      debug_print(pFunction,pLabel,pData);
    }else{
      if( (uint8_t)(ucCurrentDebugMode & eMode) ==  (uint8_t)eMode){
        debug_print(pFunction,pLabel,pData);
      }
    }
  }
  
  extern void debugCommand(void){
    uint8_t ucRecv = 0;
    if(SERIAL_DEBUG.available() >= 2){
      if(SERIAL_DEBUG.read() == 'C'){
        
        ucRecv = SERIAL_DEBUG.read();
        DEBUG_DATA("CMD Debug parameter received (cmd = %c).", (char)ucRecv);
        
        if(ucRecv >= '0'  && ucRecv < '4'){
          
          ucCurrentDebugMode = ucRecv - '0';
          DEBUG_OK("CMD Debug received.");
          
        }else{
          DEBUG_ERROR("CMD Debug parameter error.");
        }
        while(SERIAL_DEBUG.available()){
          SERIAL_DEBUG.read();
        }
      }else{
        DEBUG_ERROR("CMD Debug unknown.");
      }      
    }
  }
  
#endif