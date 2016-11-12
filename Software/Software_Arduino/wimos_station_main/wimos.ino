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
 * @brief Thread for TV displaying.
 */
void coreWimosTV(void);
/**
 * @brief Thread for RF communication.
 */
void coreWimosRF(void);

/**
 * @brief General variable for Wimos Port values.
 */
stWimosPortValues stWimosPort;
/**
 * @brief General variable for Wimos System information.
 */
stWimosInfo stWimosInformation;

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
  #ifdef WIMOS_DEBUG
    initDebug();
    while(!SERIAL_DEBUG);
    delay(1000);
  #endif
  
  initGPS();
  
  initRF();
  initTV();
  
  if(coreWimosDisplay == NULL){
    coreWimosDisplay = noOperation;
  }
  
  initSD();
  initBattery();

  initPortA();
  initPortD();
  initPortI2C();
  initIMU();

}

/**
 * @brief Wimos TV thread.
 *
 * This thread displays all values (max. 3) throw RCA signal.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
void coreWimosTV(void){
  updateGPS(&stWimosInformation);
  updateStatusBattery(&stWimosInformation);
}


/**
 * @brief Wimos RF thread.
 *
 * This thread update all values for sending over RF.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
void coreWimosRF(void){
  updateGPS(&stWimosInformation);
  updateStatusSD(&stWimosInformation);
  updateStatusBattery(&stWimosInformation);
}

extern void coreWimos(){
  #ifdef WIMOS_DEBUG
    debugCommand();
  #endif
  
  /** Timer Control **/
  
}
