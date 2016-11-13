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
 * @file wimos_imu.ino
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
 * @brief IMU Initialization.
 *
 * This function initializes the IMU interface.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
extern void initIMU(void){
  #ifdef _EN_WIMOS_IMU
  
  #endif
}


/**
 * @brief IMU reader.
 *
 * This function reads the internal IMU value.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return _stWimosInfo It returns the IMU updated into stWimosInfo struct .
 */
extern void readIMU(stWimosPortValues* stWimosPort){
  #ifdef _EN_WIMOS_IMU
  
  #endif
}