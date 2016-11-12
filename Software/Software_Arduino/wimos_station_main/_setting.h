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
 * @file _settinh.h
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing variable setting for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */

#ifndef _SETTING_H_
#define _SETTING_H_

/**
 * @brief Label Debug traces activation.
 */
#define WIMOS_DEBUG
/**
 * @brief Label Debug for colour terminal activation.
 */
#define DEBUG_COLOR
/**
 * @brief Label for GPS activation.
 */
#define _EN_WIMOS_GPS
/**
 * @brief Label for SD activation.
 */
#define _EN_WIMOS_SD
/**
 * @brief Label for IMU activation.
 */
#define _EN_WIMOS_IMU
/**
 * @brief Label for RF activation.
 */
#define _EN_WIMOS_RF
/**
 * @brief Label for Battery Reader activation.
 */
#define _EN_WIMOS_BAT
/**
 * @brief Label for TV activation.
 */
#define _EN_WIMOS_TV
/**
 * @brief Time for TV display.
 */
#define _EN_WIMOS_TV_TIME_SEC ((uint16_t) 300)

/**
 * @brief Label for Analog Port A1 activation.
 */
#define _EN_WIMOS_PORT_A1
/**
 * @brief Label for Analog Port A2 activation.
 */
#define _EN_WIMOS_PORT_A2
/**
 * @brief Label for Analog Port A3 activation.
 */
#define _EN_WIMOS_PORT_A3
/**
 * @brief Label for Analog Port A4 activation.
 */
#define _EN_WIMOS_PORT_A4
/**
 * @brief Label for Analog Port A5 activation.
 */
#define _EN_WIMOS_PORT_A5

/**
 * @brief Label for Digital Port D1 activation.
 */
#define _EN_WIMOS_PORT_D1
/**
 * @brief Label for Digital Port D2 activation.
 */
#define _EN_WIMOS_PORT_D2
/**
 * @brief Label for Digital Port D3 activation.
 */
#define _EN_WIMOS_PORT_D3
/**
 * @brief Label for Digital Port D4 activation.
 */
#define _EN_WIMOS_PORT_D4
/**
 * @brief Label for Digital Port D5 activation.
 */
#define _EN_WIMOS_PORT_D5

/**
 * @brief Label for UART Port activation.
 */
#define _EN_WIMOS_PORT_UART
/**
 * @brief Label for I2C Port activation.
 */
#define _EN_WIMOS_PORT_I2C
/**
 * @brief Size of I2C list devices.
 */
#define _EN_WIMOS_PORT_I2C_SIZE 0x01

/**
 * @brief Label for none elements.
 */
#define NONE 0x00

#ifdef _EN_WIMOS_TV
  /**
   * @brief Define to element 1 to display over TV signal.
   */
  #define _WIMOS_TV_D1  &stWimosInformation.stDateTime.ucDate
  /**
   * @brief Define to element 2 to display over TV signal.
   */
  #define _WIMOS_TV_D2  &stWimosInformation.stDateTime.ucDate
  /**
   * @brief Define to element 3 to display over TV signal.
   */
  #define _WIMOS_TV_D3  &stWimosInformation.stDateTime.ucDate
#endif




#endif
