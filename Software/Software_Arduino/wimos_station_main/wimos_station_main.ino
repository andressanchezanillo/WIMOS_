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
 * @file wimos_station_main.ino
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main function for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */
#include "_setting.h"
#include "main_config.h"
#include <SD.h>


/**
 * @brief Global variable for Wimos Port Values.
 */
stWimosPortValuesMessage stGlobalWimosPortMsg;

/**
 * @brief Global variable for Wimos System Information.
 */
stWimosInfoMessage stGlobalWimosInfoMsg;

char pDebug[150];
int8_t moveFileToDirectory(const char* ptrDirNameFrom, const char* ptrFileNameFrom, const char* ptrDirNameTo, const char* ptrFileNameTo);
void _test_n1UT03 (void);
void _test_n1UT04 (void);
void _test_n1UT05 (void);
void _test_n1UT06 (void);
void _test_n1VT03 (void);

/**
 * @brief Setup the system config.
 *
 * This function set up the system configuration.
 * @verbatim TODO @endverbatim 
 * @param none.
 * @return none.
 */
void setup(void) {

  #ifdef WIMOS_UNIT_TEST
    initDebug();
    _test_n1UT03();
    _test_n1UT04();
    _test_n1UT05();
    _test_n1UT06();
  #else 
    #ifdef WIMOS_VALIDATION_TEST
      initDebug();
      _test_n1VT03();
    #else
      initWimos();
    #endif
  #endif
  
  /*File stVolatileFile;
  char prtFileName[22];
  
  uint32_t timeTest = millis();
  for(uint8_t i=0; i<100; i++){
    stGlobalWimosPortMsg.stPortValues.usPortA1 = i;
    storeVolatile(&stGlobalWimosPortMsg.stPortValues, sizeof(&stGlobalWimosPortMsg.stPortValues) );
  }
  DEBUG_DATA("TEST n1.TSF04, Time for a storage = %d ms",(millis()-timeTest)/100);
  
  timeTest = millis();
  moveFileToDirectory(WIMOS_VOLATILE_DIR, "Wv1.log", WIMOS_NON_VOLATILE_DIR, "Wnv1.log");
  DEBUG_DATA("TEST n1.TSF05, Time for moving a file = %d ms",(millis()-timeTest)/2);*/

}

/**
 * @brief The main thread execution.
 *
 * This function run the main thread.
 * @verbatim TODO @endverbatim 
 * @param none.
 * @return none.
 */
void loop(void) {
  //coreWimosDisplay();
  coreWimos();
  delay(100);
}
