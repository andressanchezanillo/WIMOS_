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
 * @file wimos_porta.ino
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
 * @brief Analog Port Initialization.
 *
 * This function initializes the Analog ports interface.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
extern void initPortA(void){
  #ifdef _EN_WIMOS_PORT_A1

    DEBUG_OK("Analog port 1 initialized.");
  #else

    DEBUG_INFO("Analog port 1 not initialized.");
  #endif
  #ifdef _EN_WIMOS_PORT_A2

    DEBUG_OK("Analog port 2 initialized.");
  #else

    DEBUG_INFO("Analog port 2 not initialized.");
  #endif
  #ifdef _EN_WIMOS_PORT_A3

    DEBUG_OK("Analog port 3 initialized.");
  #else

    DEBUG_INFO("Analog port 3 not initialized.");
  #endif
  #ifdef _EN_WIMOS_PORT_A4

    DEBUG_OK("Analog port 4 initialized.");
  #else

    DEBUG_INFO("Analog port 4 not initialized.");
  #endif
  #ifdef _EN_WIMOS_PORT_A5

    DEBUG_OK("Analog port 5 initialized.");
  #else

    DEBUG_INFO("Analog port 5 not initialized.");
  #endif
}
/**
 * @brief Analog ports updater.
 *
 * This function updates all Analog ports value.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return _stWimosInfo It returns Analog ports values updated into stWimosInfo struct.
 */
extern void readPortA(stWimosPortValues* stWimosPort){
  #ifdef _EN_WIMOS_PORT_A1
  
  #endif
  #ifdef _EN_WIMOS_PORT_A2
  
  #endif
  #ifdef _EN_WIMOS_PORT_A3
  
  #endif
  #ifdef _EN_WIMOS_PORT_A4
  
  #endif
  #ifdef _EN_WIMOS_PORT_A5
  
  #endif
}
