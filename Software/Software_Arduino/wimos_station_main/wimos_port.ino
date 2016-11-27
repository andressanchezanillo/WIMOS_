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
 * @file wimos_port.ino
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
 * @brief I2C port Initialization.
 *
 * This function initializes the I2C interface.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
extern void initPortI2C(void){
  #ifdef _EN_WIMOS_PORT_I2C
    
    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_I2C_MASK;
    DEBUG_OK("I2C port initialized.");
  #else
  
    stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_I2C_MASK;
    DEBUG_INFO("I2C port not initialized.");
  #endif
}

/**
 * @brief I2C devices updater.
 *
 * This function updates the I2C devices values.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return _stWimosInfo It returns the I2C devices values updated into stWimosInfo struct .
 */
extern void readPortI2C(stWimosPortValues* stWimosPort){
  return;
}
