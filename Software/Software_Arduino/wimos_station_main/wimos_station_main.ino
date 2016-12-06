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
void _test_n1UT07 (void);
void _test_n1UT08 (void);
void _test_n1UT09 (void);
void _test_n1UT10 (void);
void _test_n1UT11 (void);
void _test_n1UT12 (void);
void _test_n1UT13 (void);
void _test_n1UT14 (void);
void _test_n1UT15 (void);
void _test_n1UT16 (void);
void _test_n1UT17 (void);
void _test_n1UT18 (void);
void _test_n1UT19 (void);
void _test_n1UT20 (void);
void _test_n1UT21 (void);
void _test_n1UT22 (void);
void _test_n1UT23 (void);
void _test_n1UT24 (void);
void _test_n1VT03 (void);
void _test_n1VT04 (void);
void _test_n1VT05 (void);

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
    _test_n1UT07();
    _test_n1UT08();
    _test_n1UT09();
    _test_n1UT10();
    _test_n1UT11();
    _test_n1UT12();
    _test_n1UT13();
    _test_n1UT14();
    _test_n1UT15();
    _test_n1UT16();
    _test_n1UT17();
    _test_n1UT18();
    _test_n1UT19();
    _test_n1UT20();
    _test_n1UT21();
    _test_n1UT22();
    _test_n1UT23();
    _test_n1UT24();
  #else 
    #ifdef WIMOS_VALIDATION_TEST
      initDebug();
      _test_n1VT03();
      _test_n1VT04();
      _test_n1VT05();
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
