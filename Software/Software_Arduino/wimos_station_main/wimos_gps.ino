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
 * @file wimos_gps.ino
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
 * @brief GPS Initialization.
 *
 * This function initializes the GPS interface.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
extern void initGPS(void){
  #ifdef _EN_WIMOS_GPS
    SERIALGPS.begin(BAUDRATEGPS);  
  #endif
}

/**
 * @brief GPS updater.
 *
 * This function updates the GPS Position and DateTime value.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return _stWimosInfo It returns GPS position and Datetime updated into stWimosInfo struct .
 */
extern void updateGPS(stWimosInfo* _stWimosInfo){
  #ifdef _EN_WIMOS_GPS
  
  #endif
}
