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
 * @file wimos_battery.ino
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
 * @brief Battery Reader Initialization.
 *
 * This function initializes the battery reader interface.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
extern void initBattery(void){
  #ifdef _EN_WIMOS_BAT
    DEBUG_OK("Battery Reader initialized.");
  #else
    DEBUG_INFO("Battery Reader not initialized.");
  #endif
}

/**
 * @brief Battery Reader updater.
 *
 * This function updates the battery status value.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return _stWimosInfo It returns battery status updated into stWimosInfo struct.
 */
 #define VCC_LOGIC        3300
 #define ADC_MAX_VALUE    1024
 #define ADC_VOLTAGE_TO_BAT_VOLTAGE    4
 #define VCC_MAX_BATTERY  13000       
 #define VCC_MIN_BATTERY  7150
extern void updateStatusBattery(stWimosInfo* _stWimosInfo){
  #ifdef _EN_WIMOS_BAT
    uint32_t ulVoltage = 0;
    ulVoltage  = (analogRead(WIMOS_BATT_PORT) * VCC_LOGIC)/ADC_MAX_VALUE;
    ulVoltage  *= ADC_VOLTAGE_TO_BAT_VOLTAGE;
    ulVoltage = ((VCC_MAX_BATTERY - VCC_MIN_BATTERY) / (ulVoltage - VCC_MIN_BATTERY))*100;
    _stWimosInfo->ucPercentBattery = (uint8_t) ulVoltage;
  #endif
  return;
}
