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
 * @file wimos_memory.ino
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main function for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */

#include <SPI.h>
#include <SD.h>
#include "_setting.h"
#include "main_config.h"


Sd2Card card;
SdVolume volume;
SdFile root;

PeriphericErrors errorSD = UKNOWN_ERROR;



/**
 * @brief SD Initialization.
 *
 * This function initializes the SD interface.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
extern void initSD(void){
  #ifdef _EN_WIMOS_SD
    if (card.init(SPI_HALF_SPEED, 10)) {
      if (volume.init(card)) {
        errorSD = OK_INITIALIZATION;
        DEBUG_OK("SD Memory initialized.");
        root.openRoot(volume);
        updateStatusSD(&stWimosInfoMsg.stInfo);
        return;
      }
    }    
    errorSD = ERROR_INITIALIZATION;
    DEBUG_ERROR("SD Memory initialization ERROR.");
  #else
    errorSD = ERROR_INITIALIZATION;
    DEBUG_INFO("SD Memory not initialized.");
  #endif
  return;
}

/**
 * @brief SD status updater.
 *
 * This function updates the SD status value.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return _stWimosInfo It returns GPS position and Datetime updated into stWimosInfo struct .
 */
extern void updateStatusSD(stWimosInfo* _stWimosInfo){
  #ifdef _EN_WIMOS_SD
    if( errorSD == OK_INITIALIZATION ){
      uint32_t memoryUsed = 0;
      
      _stWimosInfo->ucPercentMemory = memoryUsed / ((uint32_t)volume.blocksPerCluster() * volume.clusterCount() * 512);
      
    }  
  #endif
}
