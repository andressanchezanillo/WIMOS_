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

/**
 * @brief Global variable for Wimos Alert.
 */
stAlertMessage stGlobalWimosAlertMsg;



char pDebug[150];

/**
 * @brief Setup the system config.
 *
 * This function set up the system configuration.
 * @verbatim TODO @endverbatim 
 * @param none.
 * @return none.
 */
void setup(void) {
  initWimos();
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
  coreWimos();
}
