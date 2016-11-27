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
    stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus |= WIMOS_DEVICE_BATT_MASK;
    DEBUG_OK("Battery Reader initialized.");
  #else
    stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus &= ~WIMOS_DEVICE_BATT_MASK;
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
 
extern void updateStatusBattery(stWimosInfo* _stWimosInfo){
  #ifdef _EN_WIMOS_BAT
    int32_t slVoltage = 0;
    slVoltage  = (analogRead(WIMOS_BATT_PIN) * VCC_LOGIC)/ADC_MAX_VALUE;
    slVoltage  *= ADC_VOLTAGE_TO_BAT_VOLTAGE;
    slVoltage = ((VCC_MAX_BATTERY - VCC_MIN_BATTERY) / (slVoltage - VCC_MIN_BATTERY))*100;
    _stWimosInfo->ucPercentBattery = slVoltage;
    if(slVoltage < 0)
      _stWimosInfo->ucPercentBattery = 0;
    if(slVoltage > 100)
      _stWimosInfo->ucPercentBattery = 100;
  #endif
  return;
}
