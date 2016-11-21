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
 
#include "_setting.h"
#include "main_config.h"


void displayTV(void);

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
    coreWimosDisplay = displayTV;
    coreWimosTVTimer = millis();
    DEBUG_OK("TV output signal initialized.");
  #else
    coreWimosDisplay = communicationThread;
    DEBUG_INFO("TV output signal not initialized.");
  #endif
}


/**
 * @brief TV (RCA) destroy.
 *
 * This function destroyes the TV interface.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
extern void deleteTV(void){
  #ifdef _EN_WIMOS_TV
    coreWimosDisplay = communicationThread;
    coreWimosTVTimer = 0;
    DEBUG_OK("TV output signal destroyed.");
  #else
  
    DEBUG_INFO("TV output signal not destroyed.");
  #endif
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
  
    if(((millis() - coreWimosTVTimer)/1000) >=  _EN_WIMOS_TV_TIME_SEC )
      deleteTV();
  #else
    coreWimosDisplay = communicationThread;
    coreWimosTVTimer = 0;
  #endif
}
