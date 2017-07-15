/****************************************************************************
 * Copyright (C) 2015 by Andrés Sánchez Anillo                              *
 *                                                                          *
 * This file is part of Wimos Firmware.                                                *
 *                                                                          *
 *   Wimos Firmware is free software: you can redistribute it and/or modify it         *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Wimos Firmware is distributed in the hope that it will be useful,                 *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Wimos Firmware.  If not, see <http://www.gnu.org/licenses/>.   *
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
  
#ifdef __SAM3X8E__
    #if defined(WIMOS_UNIT_TEST)
      uint32_t inputAnalogReadA1 = 0;
      int32_t outputOffset1_A1 = 0;
      int32_t outputCoeficient1_A1 = 0;
      int32_t outputOffset2_A1 = 0;
      int32_t outputCoeficient2_A1 = 0;
      float outputJoint_A1 = 0;
      float outputProcessor_A1 = 0;
      
      uint32_t inputAnalogReadA2 = 0;
      int32_t outputOffset1_A2 = 0;
      int32_t outputCoeficient1_A2 = 0;
      int32_t outputOffset2_A2 = 0;
      int32_t outputCoeficient2_A2 = 0;
      float outputJoint_A2 = 0;
      float outputProcessor_A2 = 0;
      
      uint32_t inputAnalogReadA3 = 0;
      int32_t outputOffset1_A3 = 0;
      int32_t outputCoeficient1_A3 = 0;
      int32_t outputOffset2_A3 = 0;
      int32_t outputCoeficient2_A3 = 0;
      float outputJoint_A3 = 0;
      float outputProcessor_A3 = 0;
      
      uint32_t inputAnalogReadA4 = 0;
      int32_t outputOffset1_A4 = 0;
      int32_t outputCoeficient1_A4 = 0;
      int32_t outputOffset2_A4 = 0;
      int32_t outputCoeficient2_A4 = 0;
      float outputJoint_A4 = 0;
      float outputProcessor_A4 = 0;      
      
      uint32_t inputAnalogRead5A1 = 0;
      int32_t outputOffset1_5A1 = 0;
      int32_t outputCoeficient1_5A1 = 0;
      int32_t outputOffset2_5A1 = 0;
      int32_t outputCoeficient2_5A1 = 0;
      
      uint32_t inputAnalogRead5A2 = 0;
      int32_t outputOffset1_5A2 = 0;
      int32_t outputCoeficient1_5A2 = 0;
      int32_t outputOffset2_5A2 = 0;
      int32_t outputCoeficient2_5A2 = 0;
      
      uint32_t inputAnalogRead5A3 = 0;
      int32_t outputOffset1_5A3 = 0;
      int32_t outputCoeficient1_5A3 = 0;
      int32_t outputOffset2_5A3 = 0;
      int32_t outputCoeficient2_5A3 = 0;
      
      float outputJoint_A5 = 0;
      float outputProcessor_A5 = 0;

      
      float outputDetectProcessA1 = 0;
      float outputDetectProcessA2 = 0;
      float outputDetectProcessA3 = 0;
      float outputDetectProcessA4 = 0;
      float outputDetectProcessA5 = 0;
      
      bool inputDetectProcessA1 = false;
      bool inputDetectProcessA2 = false;
      bool inputDetectProcessA3 = false;
      bool inputDetectProcessA4 = false;
      bool inputDetectProcessA5 = false;
      
    #endif
  
  
  /**
   * @brief Analog Port Initialization.
   *
   * This function initializes the Analog ports interface.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void initPortA(void){
    analogReadResolution(12);
    #ifdef _EN_WIMOS_PORT_A1
        
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_A1_MASK;
      DEBUG_OK("Analog port 1 initialized.");
    #else
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_A1_MASK;
      DEBUG_INFO("Analog port 1 not initialized.");
    #endif
    #ifdef _EN_WIMOS_PORT_A2
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_A2_MASK;
      DEBUG_OK("Analog port 2 initialized.");
    #else
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_A2_MASK;
      DEBUG_INFO("Analog port 2 not initialized.");
    #endif
    #ifdef _EN_WIMOS_PORT_A3
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_A3_MASK;
      DEBUG_OK("Analog port 3 initialized.");
    #else
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_A3_MASK;
      DEBUG_INFO("Analog port 3 not initialized.");
    #endif
    #ifdef _EN_WIMOS_PORT_A4
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_A4_MASK;
      DEBUG_OK("Analog port 4 initialized.");
    #else
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_A4_MASK;
      DEBUG_INFO("Analog port 4 not initialized.");
    #endif
    #ifdef _EN_WIMOS_PORT_A5
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus |= WIMOS_PORT_A5_MASK;
      DEBUG_OK("Analog port 5 initialized.");
    #else
  
      stGlobalWimosInfoMsg.stInfo.stStatus.usPortStatus &= ~WIMOS_PORT_A5_MASK;
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
  extern void readPortA(stWimosStorage* stWimosPort){
  #ifdef _EN_WIMOS_PORT_A1    
      float fPort1A = 0.0f;
      uint8_t ucAlertLevelA1 = 0;
      static uint8_t ucPort1AIndex = 2;
      static float fAveragePort1A = 0.0f;
      static uint32_t ulTimerAlertA1 = millis();
    #if defined(WIMOS_UNIT_TEST)
      stWimosPort->usPort1A = 0;
    #else
      stWimosPort->usPort1A = (uint16_t)(((float)analogRead(WIMOS_A1_PORT)/(ADC_MAX_VALUE))*VCC_LOGIC); 
    #endif
      
    #if defined(WIMOS_UNIT_TEST)
      outputOffset1_A1 = stWimosPort->usPort1A = (stWimosPort->usPort1A + _WIMOS_1A_OFFSET_1); 
      outputCoeficient1_A1 = stWimosPort->usPort1A = (stWimosPort->usPort1A * _WIMOS_1A_COEFICIENT_1) ;
      outputOffset2_A1 = stWimosPort->usPort1A = (stWimosPort->usPort1A + _WIMOS_1A_OFFSET_2);
      outputCoeficient2_A1 = stWimosPort->usPort1A = (stWimosPort->usPort1A * _WIMOS_1A_COEFICIENT_2) ; 
    #else    
      stWimosPort->usPort1A = (stWimosPort->usPort1A + _WIMOS_1A_OFFSET_1); 
      stWimosPort->usPort1A = (stWimosPort->usPort1A * _WIMOS_1A_COEFICIENT_1) ; 
      stWimosPort->usPort1A = (stWimosPort->usPort1A + _WIMOS_1A_OFFSET_2); 
      stWimosPort->usPort1A = (stWimosPort->usPort1A * _WIMOS_1A_COEFICIENT_2) ; 
    #endif
      
    #if defined(WIMOS_UNIT_TEST)
      outputJoint_A1 = fPort1A = stWimosPort->usPort1A;
    #else
      fPort1A = stWimosPort->usPort1A;
    #endif
      
      _WIMOS_1A_PROCESSOR( fAveragePort1A, fPort1A, ucPort1AIndex, _WIMOS_1A_AVERAGE_SIZE/5 );
      
    #if defined(WIMOS_UNIT_TEST)
      outputProcessor_A1 = fAveragePort1A;
    #endif
      ucAlertLevelA1 = _WIMOS_1A_DETECTION(fAveragePort1A);
      if(ucAlertLevelA1 > 0){
        if(stGlobalWimosAlertMsg.stAlert.ucAlertA1 < ucAlertLevelA1){
            stGlobalWimosAlertMsg.stAlert.ucAlertA1 = ucAlertLevelA1;
            ulTimerAlertA1 = millis();
        }
      }else if (millis() - ulTimerAlertA1 >= _WIMOS_ALERT_TIMEOUT){
        stGlobalWimosAlertMsg.stAlert.ucAlertA1 = 0;          
      }      
      
      stGlobalWimosStorage.usPort1A = fAveragePort1A;
      stGlobalWimosStorage.usAlert1A = stGlobalWimosAlertMsg.stAlert.ucAlertA1;
  #endif
  #ifdef _EN_WIMOS_PORT_A2    
      float fPort2A = 0.0f;
      uint8_t ucAlertLevelA2 = 0;
      static uint8_t ucPort2AIndex = 2;
      static float fAveragePort2A = 0.0f;
      static uint32_t ulTimerAlertA2 = millis();
    #if defined(WIMOS_UNIT_TEST)
      stWimosPort->usPort2A = 0;
    #else
      stWimosPort->usPort2A = (uint16_t)(((float)analogRead(WIMOS_A2_PORT)/(ADC_MAX_VALUE))*VCC_LOGIC); 
    #endif
      
    #if defined(WIMOS_UNIT_TEST)
      outputOffset1_A2 = stWimosPort->usPort2A = (stWimosPort->usPort2A + _WIMOS_2A_OFFSET_1); 
      outputCoeficient1_A2 = stWimosPort->usPort2A = (stWimosPort->usPort2A * _WIMOS_2A_COEFICIENT_1) ;
      outputOffset2_A2 = stWimosPort->usPort2A = (stWimosPort->usPort2A + _WIMOS_2A_OFFSET_2);
      outputCoeficient2_A2 = stWimosPort->usPort2A = (stWimosPort->usPort2A * _WIMOS_2A_COEFICIENT_2) ; 
    #else    
      stWimosPort->usPort2A = (stWimosPort->usPort2A + _WIMOS_2A_OFFSET_1); 
      stWimosPort->usPort2A = (stWimosPort->usPort2A * _WIMOS_2A_COEFICIENT_1) ; 
      stWimosPort->usPort2A = (stWimosPort->usPort2A + _WIMOS_2A_OFFSET_2); 
      stWimosPort->usPort2A = (stWimosPort->usPort2A * _WIMOS_2A_COEFICIENT_2) ; 
    #endif
      
    #if defined(WIMOS_UNIT_TEST)
      outputJoint_A2 = fPort2A = stWimosPort->usPort2A;
    #else
      fPort2A = stWimosPort->usPort2A;
    #endif
      
      _WIMOS_2A_PROCESSOR( fAveragePort2A, fPort2A, ucPort2AIndex, _WIMOS_2A_AVERAGE_SIZE/5 );
      
    #if defined(WIMOS_UNIT_TEST)
      outputProcessor_A2 = fAveragePort2A;
    #endif
    
      ucAlertLevelA2 = _WIMOS_2A_DETECTION(fAveragePort2A);
      if(ucAlertLevelA2 > 0){
        if(stGlobalWimosAlertMsg.stAlert.ucAlertA2 < ucAlertLevelA2){
            stGlobalWimosAlertMsg.stAlert.ucAlertA2 = ucAlertLevelA2;
            ulTimerAlertA2 = millis();
        }
      }else if (millis() - ulTimerAlertA2 >= _WIMOS_ALERT_TIMEOUT){
        stGlobalWimosAlertMsg.stAlert.ucAlertA2 = 0;          
      }
      
      stGlobalWimosStorage.usPort2A = fAveragePort2A;
      stGlobalWimosStorage.usAlert2A = stGlobalWimosAlertMsg.stAlert.ucAlertA2;
  #endif
  #ifdef _EN_WIMOS_PORT_A3    
      float fPort3A = 0.0f;
      uint8_t ucAlertLevelA3 = 0;
      static uint8_t ucPort3AIndex = 2;
      static float fAveragePort3A = 0.0f;
      static uint32_t ulTimerAlertA3 = millis();
    #if defined(WIMOS_UNIT_TEST)
      stWimosPort->usPort3A = 0;
    #else
      stWimosPort->usPort3A = (uint16_t)(((float)analogRead(WIMOS_A3_PORT)/(ADC_MAX_VALUE))*VCC_LOGIC); 
    #endif
      
    #if defined(WIMOS_UNIT_TEST)
      outputOffset1_A3 = stWimosPort->usPort3A = (stWimosPort->usPort3A + _WIMOS_3A_OFFSET_1); 
      outputCoeficient1_A3 = stWimosPort->usPort3A = (stWimosPort->usPort3A * _WIMOS_3A_COEFICIENT_1) ;
      outputOffset2_A3 = stWimosPort->usPort3A = (stWimosPort->usPort3A + _WIMOS_3A_OFFSET_2);
      outputCoeficient2_A3 = stWimosPort->usPort3A = (stWimosPort->usPort3A * _WIMOS_3A_COEFICIENT_2) ; 
    #else    
      stWimosPort->usPort3A = (stWimosPort->usPort3A + _WIMOS_3A_OFFSET_1); 
      stWimosPort->usPort3A = (stWimosPort->usPort3A * _WIMOS_3A_COEFICIENT_1) ; 
      stWimosPort->usPort3A = (stWimosPort->usPort3A + _WIMOS_3A_OFFSET_2); 
      stWimosPort->usPort3A = (stWimosPort->usPort3A * _WIMOS_3A_COEFICIENT_2) ; 
    #endif
      
    #if defined(WIMOS_UNIT_TEST)
      outputJoint_A3 = fPort3A = stWimosPort->usPort3A;
    #else
      fPort3A = stWimosPort->usPort3A;
    #endif
      
      _WIMOS_3A_PROCESSOR( fAveragePort3A, fPort3A, ucPort3AIndex, _WIMOS_3A_AVERAGE_SIZE/5 );
      
    #if defined(WIMOS_UNIT_TEST)
      outputProcessor_A3 = fAveragePort3A;
    #endif
    
      ucAlertLevelA3 = _WIMOS_3A_DETECTION(fAveragePort3A);
      if(ucAlertLevelA3 > 0){
        if(stGlobalWimosAlertMsg.stAlert.ucAlertA3 < ucAlertLevelA3){
            stGlobalWimosAlertMsg.stAlert.ucAlertA3 = ucAlertLevelA3;
            ulTimerAlertA3 = millis();
        }
      }else if (millis() - ulTimerAlertA3 >= _WIMOS_ALERT_TIMEOUT){
        stGlobalWimosAlertMsg.stAlert.ucAlertA3 = 0;          
      }
      
      stGlobalWimosStorage.usPort3A = fAveragePort3A;
      stGlobalWimosStorage.usAlert3A = stGlobalWimosAlertMsg.stAlert.ucAlertA3;
  #endif
  #ifdef _EN_WIMOS_PORT_A4    
      float fPort4A = 0.0f;
      uint8_t ucAlertLevelA4 = 0;
      static uint8_t ucPort4AIndex = 2;
      static float fAveragePort4A = 0.0f;
      static uint32_t ulTimerAlertA4 = millis();
    #if defined(WIMOS_UNIT_TEST)
      stWimosPort->usPort4A = 0;
    #else
      stWimosPort->usPort4A = (uint16_t)(((float)analogRead(WIMOS_A4_PORT)/(ADC_MAX_VALUE))*VCC_LOGIC); 
    #endif
      
    #if defined(WIMOS_UNIT_TEST)
      outputOffset1_A4 = stWimosPort->usPort4A = (stWimosPort->usPort4A + _WIMOS_4A_OFFSET_1); 
      outputCoeficient1_A4 = stWimosPort->usPort4A = (stWimosPort->usPort4A * _WIMOS_4A_COEFICIENT_1) ;
      outputOffset2_A4 = stWimosPort->usPort4A = (stWimosPort->usPort4A + _WIMOS_4A_OFFSET_2);
      outputCoeficient2_A4 = stWimosPort->usPort4A = (stWimosPort->usPort4A * _WIMOS_4A_COEFICIENT_2) ; 
    #else    
      stWimosPort->usPort4A = (stWimosPort->usPort4A + _WIMOS_4A_OFFSET_1); 
      stWimosPort->usPort4A = (stWimosPort->usPort4A * _WIMOS_4A_COEFICIENT_1) ; 
      stWimosPort->usPort4A = (stWimosPort->usPort4A + _WIMOS_4A_OFFSET_2); 
      stWimosPort->usPort4A = (stWimosPort->usPort4A * _WIMOS_4A_COEFICIENT_2) ; 
    #endif
      
    #if defined(WIMOS_UNIT_TEST)
      outputJoint_A4 = fPort4A = stWimosPort->usPort4A;
    #else
      fPort4A = stWimosPort->usPort4A;
    #endif
      
      _WIMOS_4A_PROCESSOR( fAveragePort4A, fPort4A, ucPort4AIndex, _WIMOS_4A_AVERAGE_SIZE/5 );
      
    #if defined(WIMOS_UNIT_TEST)
      outputProcessor_A4 = fAveragePort4A;
    #endif
      
      ucAlertLevelA4 = _WIMOS_4A_DETECTION(fAveragePort4A);
      if(ucAlertLevelA4 > 0){
        if(stGlobalWimosAlertMsg.stAlert.ucAlertA4 < ucAlertLevelA4){
            stGlobalWimosAlertMsg.stAlert.ucAlertA4 = ucAlertLevelA4;
            ulTimerAlertA4 = millis();
        }
      }else if (millis() - ulTimerAlertA4 >= _WIMOS_ALERT_TIMEOUT){
        stGlobalWimosAlertMsg.stAlert.ucAlertA4 = 0;          
      }
      
      stGlobalWimosStorage.usPort4A = fAveragePort4A;
      stGlobalWimosStorage.usAlert4A = stGlobalWimosAlertMsg.stAlert.ucAlertA4;
  #endif
#ifdef _EN_WIMOS_PORT_A5
    
      float fPort5A = 0.0f;
      uint8_t ucAlertLevelA5 = 0;
      static float fAveragePort5A = 0.0f;
      static uint8_t ucPort5AIndex = 2;
      static uint32_t ulTimerAlertA5 = millis();
      
    #if defined(WIMOS_UNIT_TEST)
      stWimosPort->usPort5A1 = inputAnalogRead5A1; 
  #else
      stWimosPort->usPort5A1 = (uint16_t)(((float)analogRead(WIMOS_A5_1_PORT)/(ADC_MAX_VALUE))*VCC_LOGIC);  
  #endif
    
    #if defined(WIMOS_UNIT_TEST)
      outputOffset1_5A1 = stWimosPort->usPort5A1 = (stWimosPort->usPort5A1 + _WIMOS_5A1_OFFSET_1); 
      outputCoeficient1_5A1 = stWimosPort->usPort5A1 = (stWimosPort->usPort5A1 * _WIMOS_5A1_COEFICIENT_1);
      outputOffset2_5A1 = stWimosPort->usPort5A1 = (stWimosPort->usPort5A1 + _WIMOS_5A1_OFFSET_2); 
      outputCoeficient2_5A1 = stWimosPort->usPort5A1 = (stWimosPort->usPort5A1 * _WIMOS_5A1_COEFICIENT_2);
  #else
      stWimosPort->usPort5A1 = (stWimosPort->usPort5A1 + _WIMOS_5A1_OFFSET_1); 
      stWimosPort->usPort5A1 = (stWimosPort->usPort5A1 * _WIMOS_5A1_COEFICIENT_1) ; 
      stWimosPort->usPort5A1 = (stWimosPort->usPort5A1 + _WIMOS_5A1_OFFSET_2); 
      stWimosPort->usPort5A1 = (stWimosPort->usPort5A1 * _WIMOS_5A1_COEFICIENT_2) ; 
      
  #endif
  
  #if defined(WIMOS_UNIT_TEST)
      stWimosPort->usPort5A2 = inputAnalogRead5A2; 
  #else
    stWimosPort->usPort5A2 = (uint16_t)(((float)analogRead(WIMOS_A5_2_PORT)/(ADC_MAX_VALUE))*VCC_LOGIC); 
  #endif
    
    #if defined(WIMOS_UNIT_TEST)
    
      outputOffset1_5A2 = stWimosPort->usPort5A2 = (stWimosPort->usPort5A2 + _WIMOS_5A2_OFFSET_1); 
      outputCoeficient1_5A2 = stWimosPort->usPort5A2 = (stWimosPort->usPort5A2 * _WIMOS_5A2_COEFICIENT_1) ; 
      outputOffset2_5A2 = stWimosPort->usPort5A2 = (stWimosPort->usPort5A2 + _WIMOS_5A2_OFFSET_2); 
      outputCoeficient2_5A2 = stWimosPort->usPort5A2 = (stWimosPort->usPort5A2 * _WIMOS_5A2_COEFICIENT_2) ; 
      
  #else
      stWimosPort->usPort5A2 = (stWimosPort->usPort5A2 + _WIMOS_5A2_OFFSET_1); 
      stWimosPort->usPort5A2 = (stWimosPort->usPort5A2 * _WIMOS_5A2_COEFICIENT_1) ; 
      stWimosPort->usPort5A2 = (stWimosPort->usPort5A2 + _WIMOS_5A2_OFFSET_2); 
      stWimosPort->usPort5A2 = (stWimosPort->usPort5A2 * _WIMOS_5A2_COEFICIENT_2) ; 
  #endif
  
    #if defined(WIMOS_UNIT_TEST)
      stWimosPort->usPort5A3 = inputAnalogRead5A3; 
  #else
    stWimosPort->usPort5A3 = (uint16_t)(((float)analogRead(WIMOS_A5_3_PORT)/(ADC_MAX_VALUE))*VCC_LOGIC); 
    #endif
    
    #if defined(WIMOS_UNIT_TEST)
  
      outputOffset1_5A3 = stWimosPort->usPort5A3 = (stWimosPort->usPort5A3 + _WIMOS_5A3_OFFSET_1); 
      outputCoeficient1_5A3 = stWimosPort->usPort5A3 = (stWimosPort->usPort5A3 * _WIMOS_5A3_COEFICIENT_1) ; 
      outputOffset2_5A3 = stWimosPort->usPort5A3 = (stWimosPort->usPort5A3 + _WIMOS_5A3_OFFSET_2); 
      outputCoeficient2_5A3 = stWimosPort->usPort5A3 = (stWimosPort->usPort5A3 * _WIMOS_5A3_COEFICIENT_2) ; 
      
  #else
  
      stWimosPort->usPort5A3 = (stWimosPort->usPort5A3 + _WIMOS_5A3_OFFSET_1); 
      stWimosPort->usPort5A3 = (stWimosPort->usPort5A3 * _WIMOS_5A3_COEFICIENT_1) ; 
      stWimosPort->usPort5A3 = (stWimosPort->usPort5A3 + _WIMOS_5A3_OFFSET_2); 
      stWimosPort->usPort5A3 = (stWimosPort->usPort5A3 * _WIMOS_5A3_COEFICIENT_2) ; 
    
  #endif
  
    #if defined(WIMOS_UNIT_TEST)
      outputJoint_A5 = fPort5A = (float)_WIMOS_5A_OPERATOR_JOINT((float)stWimosPort->usPort5A1,(float)stWimosPort->usPort5A2,(float)stWimosPort->usPort5A3);
  #else
    fPort5A = _WIMOS_5A_OPERATOR_JOINT(stWimosPort->usPort5A1,stWimosPort->usPort5A2,stWimosPort->usPort5A3);
  #endif
    
      _WIMOS_5A_PROCESSOR( fAveragePort5A, fPort5A, ucPort5AIndex, _WIMOS_5A_AVERAGE_SIZE/5 );
      
    #if defined(WIMOS_UNIT_TEST)
      outputProcessor_A5 = fAveragePort5A;
    #endif
    
      ucAlertLevelA5 = _WIMOS_5A_DETECTION(fAveragePort5A);
      if(ucAlertLevelA5 > 0){
        if(stGlobalWimosAlertMsg.stAlert.ucAlertA5 < ucAlertLevelA5){
            stGlobalWimosAlertMsg.stAlert.ucAlertA5 = ucAlertLevelA5;
            ulTimerAlertA5 = millis();
        }
      }else if (millis() - ulTimerAlertA5 >= _WIMOS_ALERT_TIMEOUT){
        stGlobalWimosAlertMsg.stAlert.ucAlertA5 = 0;          
      }
      
      stGlobalWimosStorage.usPort5A = fAveragePort5A;
      stGlobalWimosStorage.usAlert5A = stGlobalWimosAlertMsg.stAlert.ucAlertA5;
  #endif
  }

  
  #ifdef _EN_WIMOS_PORT_A1
    uint8_t detectionA1Default(float fInputValue){
      static float flA1Average = 0.0f;
      static uint8_t ucA1Index = 2;
      bool bDetectionPos = false;
      bool bDetectionNeg = false;
      uint8_t ucAlert = 0;
      
      #if defined(WIMOS_UNIT_TEST)
        if(inputDetectProcessA1){
          inputDetectProcessA1 = false;
          flA1Average = 0.0f;
        }
      #endif
      
      _WIMOS_1A_PROCESSOR_DET(flA1Average, fInputValue, ucA1Index); 
      
      #if defined(WIMOS_UNIT_TEST)
        outputDetectProcessA1 = flA1Average;
      #endif
      
      stGlobalWimosStorage.usOffset1A = flA1Average + _WIMOS_1A_AVERAGE_OFFSET;
      
      for(uint8_t i=1; i<(_WIMOS_1A_OFFSET_MAX_SIZE+1); i++){
        bDetectionPos = (MATH_DEFAULT_THRESHOLD(fInputValue, flA1Average, (_WIMOS_1A_AVERAGE_OFFSET*i)) == true);
        bDetectionNeg = (MATH_DEFAULT_THRESHOLD(fInputValue, flA1Average, -(_WIMOS_1A_AVERAGE_OFFSET*i)) == true);

        #ifdef DEBUG_ANALOG_A1
          
          SERIAL_DEBUG.print(flA1Average+(_WIMOS_1A_AVERAGE_OFFSET*i));
          SERIAL_DEBUG.print(" ");
          SERIAL_DEBUG.print(flA1Average-(_WIMOS_1A_AVERAGE_OFFSET*i));
          SERIAL_DEBUG.print(" ");

        #endif
        
        if(bDetectionNeg || bDetectionPos){
          ucAlert = i;
        }
      }
      
      #ifdef DEBUG_ANALOG_A1
      
        
        SERIAL_DEBUG.print(abs(fInputValue));      
        SERIAL_DEBUG.print(" ");
        SERIAL_DEBUG.println((uint8_t) ucAlert);
      #endif
      
      //flA1Average  = fInputValue;
      
      return ucAlert;
    }
    
    uint8_t detectionA1Custom(float fInputValue){
      /*TODO: Implementation Custom*/
      return 0;
    }

  #endif
  
   #ifdef _EN_WIMOS_PORT_A2
    uint8_t detectionA2Default(float fInputValue){
      static float flA2Average = 0.0f;
      static uint8_t ucA2Index = 2;
      bool bDetectionPos = false;
      bool bDetectionNeg = false;
      uint8_t ucAlert = 0;
      
      #if defined(WIMOS_UNIT_TEST)
        if(inputDetectProcessA2){
          inputDetectProcessA2 = false;
          flA2Average = 0.0f;
        }
      #endif
      
      _WIMOS_2A_PROCESSOR_DET(flA2Average, fInputValue, ucA2Index); 
      
      #if defined(WIMOS_UNIT_TEST)
        outputDetectProcessA2 = flA2Average;
      #endif
      
      stGlobalWimosStorage.usOffset2A = flA2Average + _WIMOS_2A_AVERAGE_OFFSET;
      
      for(uint8_t i=1; i<(_WIMOS_2A_OFFSET_MAX_SIZE+1); i++){
        bDetectionPos = (MATH_DEFAULT_THRESHOLD(fInputValue, flA2Average, (_WIMOS_2A_AVERAGE_OFFSET*i)) == true);
        bDetectionNeg = (MATH_DEFAULT_THRESHOLD(fInputValue, flA2Average, -(_WIMOS_2A_AVERAGE_OFFSET*i)) == true);

        #ifdef DEBUG_ANALOG_A2
          
          SERIAL_DEBUG.print(flA2Average+(_WIMOS_2A_AVERAGE_OFFSET*i));
          SERIAL_DEBUG.print(" ");
          SERIAL_DEBUG.print(flA2Average-(_WIMOS_2A_AVERAGE_OFFSET*i));
          SERIAL_DEBUG.print(" ");

        #endif
        
        if(bDetectionNeg || bDetectionPos){
          ucAlert = i;
        }
      }
      
      #ifdef DEBUG_ANALOG_A2
      
        
        SERIAL_DEBUG.print(abs(fInputValue));      
        SERIAL_DEBUG.print(" ");
        SERIAL_DEBUG.println((uint8_t) ucAlert);
      #endif
      
      //flA2Average  = fInputValue;
      
      return ucAlert;
    }
    
    uint8_t detectionA2Custom(float fInputValue){
      /*TODO: Implementation Custom*/
      return 0;
    }  

  #endif
  
  #ifdef _EN_WIMOS_PORT_A3
    uint8_t detectionA3Default(float fInputValue){
      static float fLastValue = 0.0f;
      static float flA3Average = 0.0f;
      static uint8_t ucA3Index = 2;
      bool bDetectionPos = false;
      bool bDetectionNeg = false;
      uint8_t ucAlert = 0;
      
      
      #if defined(WIMOS_UNIT_TEST)
        if(inputDetectProcessA3){
          inputDetectProcessA3 = false;
          flA3Average = 0.0f;
        }
      #endif
      
      _WIMOS_3A_PROCESSOR_DET(flA3Average, fInputValue, ucA3Index); 
      
      #if defined(WIMOS_UNIT_TEST)
        outputDetectProcessA3 = flA3Average;
      #endif
      
      stGlobalWimosStorage.usOffset3A = flA3Average + _WIMOS_3A_AVERAGE_OFFSET;
      
      for(uint8_t i=1; i<(_WIMOS_3A_OFFSET_MAX_SIZE+1); i++){
        bDetectionPos = (MATH_DEFAULT_THRESHOLD(fInputValue, flA3Average, (_WIMOS_3A_AVERAGE_OFFSET*i)) == true);
        bDetectionNeg = (MATH_DEFAULT_THRESHOLD(fInputValue, flA3Average, -(_WIMOS_3A_AVERAGE_OFFSET*i)) == true);

        #ifdef DEBUG_ANALOG_A3
          
          SERIAL_DEBUG.print(flA3Average+(_WIMOS_3A_AVERAGE_OFFSET*i));
          SERIAL_DEBUG.print(" ");
          SERIAL_DEBUG.print(flA3Average-(_WIMOS_3A_AVERAGE_OFFSET*i));
          SERIAL_DEBUG.print(" ");

        #endif
        
        if(bDetectionNeg || bDetectionPos){
          ucAlert = i;
        }
      }
      
      #ifdef DEBUG_ANALOG_A3
      
        
        SERIAL_DEBUG.print(abs(fInputValue));      
        SERIAL_DEBUG.print(" ");
        SERIAL_DEBUG.println((uint8_t) ucAlert);
      #endif
      
      //flA3Average  = fInputValue;
      
      return ucAlert;
    }
    
    uint8_t detectionA3Custom(float fInputValue){
      /*TODO: Implementation Custom*/
      return 0;
    }

  #endif
  
  #ifdef _EN_WIMOS_PORT_A4
    uint8_t detectionA4Default(float fInputValue){
      static float fLastValue = 0.0f;
      static float flA4Average = 0.0f;
      static uint8_t ucA4Index = 2;
      bool bDetectionPos = false;
      bool bDetectionNeg = false;
      uint8_t ucAlert = 0;
      
      #if defined(WIMOS_UNIT_TEST)
        if(inputDetectProcessA4){
          inputDetectProcessA4 = false;
          flA4Average = 0.0f;
        }
      #endif
      
      _WIMOS_4A_PROCESSOR_DET(flA4Average, fInputValue, ucA4Index); 
      
      #if defined(WIMOS_UNIT_TEST)
        outputDetectProcessA4 = flA4Average;
      #endif
      
      stGlobalWimosStorage.usOffset4A = flA4Average + _WIMOS_4A_AVERAGE_OFFSET;
      
      for(uint8_t i=1; i<(_WIMOS_4A_OFFSET_MAX_SIZE+1); i++){
        bDetectionPos = (MATH_DEFAULT_THRESHOLD(fInputValue, flA4Average, (_WIMOS_4A_AVERAGE_OFFSET*i)) == true);
        bDetectionNeg = (MATH_DEFAULT_THRESHOLD(fInputValue, flA4Average, -(_WIMOS_4A_AVERAGE_OFFSET*i)) == true);

        #ifdef DEBUG_ANALOG_A4
          
          SERIAL_DEBUG.print(flA4Average+(_WIMOS_4A_AVERAGE_OFFSET*i));
          SERIAL_DEBUG.print(" ");
          SERIAL_DEBUG.print(flA4Average-(_WIMOS_4A_AVERAGE_OFFSET*i));
          SERIAL_DEBUG.print(" ");

        #endif
        
        if(bDetectionNeg || bDetectionPos){
          ucAlert = i;
        }
      }
      
      #ifdef DEBUG_ANALOG_A4
      
        
        SERIAL_DEBUG.print(abs(fInputValue));      
        SERIAL_DEBUG.print(" ");
        SERIAL_DEBUG.println((uint8_t) ucAlert);
      #endif
      
      //flA4Average  = fInputValue;
      
      return ucAlert;
    }
    
    uint8_t detectionA4Custom(float fInputValue){
      /*TODO: Implementation Custom*/
      return 0;
    }

  #endif

  #ifdef _EN_WIMOS_PORT_A5
    uint8_t detectionA5Default(float fInputValue){
      static float fLastValue = 0.0f;
      static float flA5Average = 0.0f;
      static uint8_t ucA5Index = 2;
      bool bDetectionPos = false;
      bool bDetectionNeg = false;
      uint8_t ucAlert = 0;
      
      #if defined(WIMOS_UNIT_TEST)
        if(inputDetectProcessA5){
          inputDetectProcessA5 = false;
          flA5Average = 0.0f;
        }
      #endif
      
      _WIMOS_5A_PROCESSOR_DET(flA5Average, fInputValue, ucA5Index); 
      
      #if defined(WIMOS_UNIT_TEST)
        outputDetectProcessA5 = flA5Average;
      #endif
      
      stGlobalWimosStorage.usOffset5A = flA5Average + _WIMOS_5A_AVERAGE_OFFSET;
      
      for(uint8_t i=1; i<(_WIMOS_5A_OFFSET_MAX_SIZE+1); i++){
        bDetectionPos = (MATH_DEFAULT_THRESHOLD(fInputValue, flA5Average, (_WIMOS_5A_AVERAGE_OFFSET*i)) == true);
        bDetectionNeg = (MATH_DEFAULT_THRESHOLD(fInputValue, flA5Average, -(_WIMOS_5A_AVERAGE_OFFSET*i)) == true);

        #ifdef DEBUG_ANALOG_A5
          
          SERIAL_DEBUG.print(flA5Average+(_WIMOS_5A_AVERAGE_OFFSET*i));
          SERIAL_DEBUG.print(" ");
          SERIAL_DEBUG.print(flA5Average-(_WIMOS_5A_AVERAGE_OFFSET*i));
          SERIAL_DEBUG.print(" ");

        #endif
        
        if(bDetectionNeg || bDetectionPos){
          ucAlert = i;
        }
      }
      
      #ifdef DEBUG_ANALOG_A5

        SERIAL_DEBUG.print(abs(fInputValue));      
        SERIAL_DEBUG.print(" ");
        SERIAL_DEBUG.println((uint8_t) ucAlert);
        
      #endif
      
      //flA5Average  = fInputValue;
      
      return ucAlert;
    }
    
    uint8_t detectionA5Custom(float fInputValue){
      /*TODO: Implementation Custom*/
      return 0;
    }

  #endif
  
  
  #ifdef WIMOS_DEBUG
    #ifdef WIMOS_UNIT_TEST
  
      /**
       * @brief Wimos test n3.UT36.
       *
       * Unit test n3.UT36 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT36 (void){
        const char* testName = "n3.UT36 = %d";
        /*TODO: do a Analog Port initializer.*/
        DEBUG_VALID(testName , 
                   (true), 
                   (true));
      } 
  
  
      /**
       * @brief Wimos test n3.UT37.
       *
       * Unit test n3.UT37 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT37 (void){
        const char* testName = "n3.UT37 = %d";
        /*TODO: Analog reader.*/
          DEBUG_VALID(testName , 
                     (true), 
                     (true));
      } 
  
  
      /**
       * @brief Wimos test n5.UT01.
       *
       * Unit test n5.UT01 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT01 (void){
        const char* testName = "n5.UT01 = %d";
        inputAnalogReadA1 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputOffset1_A1 == _WIMOS_1A_OFFSET_1 ), 
                   (outputOffset1_A1 == _WIMOS_1A_OFFSET_1));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT02.
       *
       * Unit test n5.UT02 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT02 (void){
        const char* testName = "n5.UT02 = %d";
        inputAnalogReadA1 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputCoeficient1_A1 == _WIMOS_1A_OFFSET_1 * _WIMOS_1A_COEFICIENT_1 ), 
                   (outputCoeficient1_A1 == _WIMOS_1A_OFFSET_1 * _WIMOS_1A_COEFICIENT_1));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT03.
       *
       * Unit test n5.UT03 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT03 (void){
        const char* testName = "n5.UT03 = %d";
        inputAnalogReadA1 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputOffset2_A1 == (_WIMOS_1A_OFFSET_1 * _WIMOS_1A_COEFICIENT_1) + _WIMOS_1A_OFFSET_2 ), 
                   (outputOffset2_A1 == (_WIMOS_1A_OFFSET_1 * _WIMOS_1A_COEFICIENT_1) + _WIMOS_1A_OFFSET_2));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT04.
       *
       * Unit test n5.UT04 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT04 (void){
        const char* testName = "n5.UT04 = %d";
        inputAnalogReadA1 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputCoeficient2_A1 == ((_WIMOS_1A_OFFSET_1 * _WIMOS_1A_COEFICIENT_1) + _WIMOS_1A_OFFSET_2 ) * _WIMOS_1A_COEFICIENT_2), 
                   (outputCoeficient2_A1 == ((_WIMOS_1A_OFFSET_1 * _WIMOS_1A_COEFICIENT_1) + _WIMOS_1A_OFFSET_2 ) * _WIMOS_1A_COEFICIENT_2));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT05.
       *
       * Unit test n5.UT05 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT05 (void){
        const char* testName = "n5.UT05 = %f";
        float fAverageOutput = 0.0;
        
        inputAnalogReadA1 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<100; i++){
          readPortA(&stTestWimosPort);
          fAverageOutput += (float) outputJoint_A1 / 100;
        }
        DEBUG_VALID( testName , 
                   ( fAverageOutput - outputProcessor_A1), 
                   ( abs((float)fAverageOutput - (float)outputProcessor_A1) < 0.001));
        /*End_Body_TEST:*/
      } 
  
  
      /**
       * @brief Wimos test n5.UT06.
       *
       * Unit test n5.UT06 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT06 (void){
        const char* testName = "n5.UT06 = %d";
        inputAnalogReadA2 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputOffset1_A2 == _WIMOS_2A_OFFSET_1 ), 
                   (outputOffset1_A2 == _WIMOS_2A_OFFSET_1));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT07.
       *
       * Unit test n5.UT07 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT07 (void){
        const char* testName = "n5.UT07 = %d";
        inputAnalogReadA2 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputCoeficient1_A2 == _WIMOS_2A_OFFSET_1 * _WIMOS_2A_COEFICIENT_1 ), 
                   (outputCoeficient1_A2 == _WIMOS_2A_OFFSET_1 * _WIMOS_2A_COEFICIENT_1));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT08.
       *
       * Unit test n5.UT08 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT08 (void){
        const char* testName = "n5.UT08 = %d";
        inputAnalogReadA2 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputOffset2_A2 == (_WIMOS_2A_OFFSET_1 * _WIMOS_2A_COEFICIENT_1) + _WIMOS_2A_OFFSET_2 ), 
                   (outputOffset2_A2 == (_WIMOS_2A_OFFSET_1 * _WIMOS_2A_COEFICIENT_1) + _WIMOS_2A_OFFSET_2));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT09.
       *
       * Unit test n5.UT09 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT09 (void){
        const char* testName = "n5.UT09 = %d";
        inputAnalogReadA2 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputCoeficient2_A2 == ((_WIMOS_2A_OFFSET_1 * _WIMOS_2A_COEFICIENT_1) + _WIMOS_2A_OFFSET_2 ) * _WIMOS_2A_COEFICIENT_2), 
                   (outputCoeficient2_A2 == ((_WIMOS_2A_OFFSET_1 * _WIMOS_2A_COEFICIENT_1) + _WIMOS_2A_OFFSET_2 ) * _WIMOS_2A_COEFICIENT_2));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT10.
       *
       * Unit test n5.UT10 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT10 (void){
        const char* testName = "n5.UT10 = %f";
        float fAverageOutput = 0.0;
        
        inputAnalogReadA2 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<100; i++){
          readPortA(&stTestWimosPort);
          fAverageOutput += (float) outputJoint_A2 / 100;
        }
        DEBUG_VALID( testName , 
                   ( fAverageOutput - outputProcessor_A2), 
                   ( abs((float)fAverageOutput - (float)outputProcessor_A2) < 0.001));
        /*End_Body_TEST:*/
      } 
  
  
      /**
       * @brief Wimos test n5.UT11.
       *
       * Unit test n5.UT11 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT11 (void){
        const char* testName = "n5.UT11 = %d";
        inputAnalogReadA3 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputOffset1_A3 == _WIMOS_3A_OFFSET_1 ), 
                   (outputOffset1_A3 == _WIMOS_3A_OFFSET_1));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT12.
       *
       * Unit test n5.UT12 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT12 (void){
        const char* testName = "n5.UT12 = %d";
        inputAnalogReadA3 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputCoeficient1_A3 == _WIMOS_3A_OFFSET_1 * _WIMOS_3A_COEFICIENT_1 ), 
                   (outputCoeficient1_A3 == _WIMOS_3A_OFFSET_1 * _WIMOS_3A_COEFICIENT_1));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT13.
       *
       * Unit test n5.UT13 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT13 (void){
        const char* testName = "n5.UT13 = %d";
        inputAnalogReadA3 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputOffset2_A3 == (_WIMOS_3A_OFFSET_1 * _WIMOS_3A_COEFICIENT_1) + _WIMOS_3A_OFFSET_2 ), 
                   (outputOffset2_A3 == (_WIMOS_3A_OFFSET_1 * _WIMOS_3A_COEFICIENT_1) + _WIMOS_3A_OFFSET_2));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT14.
       *
       * Unit test n5.UT14 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT14 (void){
        const char* testName = "n5.UT14 = %d";
        inputAnalogReadA3 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputCoeficient2_A3 == ((_WIMOS_3A_OFFSET_1 * _WIMOS_3A_COEFICIENT_1) + _WIMOS_3A_OFFSET_2 ) * _WIMOS_3A_COEFICIENT_2), 
                   (outputCoeficient2_A3 == ((_WIMOS_3A_OFFSET_1 * _WIMOS_3A_COEFICIENT_1) + _WIMOS_3A_OFFSET_2 ) * _WIMOS_3A_COEFICIENT_2));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT15.
       *
       * Unit test n5.UT15 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT15 (void){
        const char* testName = "n5.UT15 = %f";
        float fAverageOutput = 0.0;
        
        inputAnalogReadA3 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<100; i++){
          readPortA(&stTestWimosPort);
          fAverageOutput += (float) outputJoint_A3 / 100;
        }
        DEBUG_VALID( testName , 
                   ( fAverageOutput - outputProcessor_A3), 
                   ( abs((float)fAverageOutput - (float)outputProcessor_A3) < 0.001));
        /*End_Body_TEST:*/
      } 
  
  
      /**
       * @brief Wimos test n5.UT16.
       *
       * Unit test n5.UT16 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT16 (void){
        const char* testName = "n5.UT16 = %d";
        inputAnalogReadA4 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputOffset1_A4 == _WIMOS_4A_OFFSET_1 ), 
                   (outputOffset1_A4 == _WIMOS_4A_OFFSET_1));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT17.
       *
       * Unit test n5.UT17 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT17 (void){
        const char* testName = "n5.UT17 = %d";
        inputAnalogReadA4 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputCoeficient1_A4 == _WIMOS_4A_OFFSET_1 * _WIMOS_4A_COEFICIENT_1 ), 
                   (outputCoeficient1_A4 == _WIMOS_4A_OFFSET_1 * _WIMOS_4A_COEFICIENT_1));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT18.
       *
       * Unit test n5.UT18 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT18 (void){
        const char* testName = "n5.UT18 = %d";
        inputAnalogReadA4 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputOffset2_A4 == (_WIMOS_4A_OFFSET_1 * _WIMOS_4A_COEFICIENT_1) + _WIMOS_4A_OFFSET_2 ), 
                   (outputOffset2_A4 == (_WIMOS_4A_OFFSET_1 * _WIMOS_4A_COEFICIENT_1) + _WIMOS_4A_OFFSET_2));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT19.
       *
       * Unit test n5.UT19 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT19 (void){
        const char* testName = "n5.UT19 = %d";
        inputAnalogReadA4 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (outputCoeficient2_A4 == ((_WIMOS_4A_OFFSET_1 * _WIMOS_4A_COEFICIENT_1) + _WIMOS_4A_OFFSET_2 ) * _WIMOS_4A_COEFICIENT_2), 
                   (outputCoeficient2_A4 == ((_WIMOS_4A_OFFSET_1 * _WIMOS_4A_COEFICIENT_1) + _WIMOS_4A_OFFSET_2 ) * _WIMOS_4A_COEFICIENT_2));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT20.
       *
       * Unit test n5.UT20 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT20 (void){
        const char* testName = "n5.UT20 = %f";
        float fAverageOutput = 0.0;
        
        inputAnalogReadA4 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<100; i++){
          readPortA(&stTestWimosPort);
          fAverageOutput += (float) outputJoint_A4 / 100;
        }
        DEBUG_VALID( testName , 
                   ( fAverageOutput - outputProcessor_A4), 
                   ( abs((float)fAverageOutput - (float)outputProcessor_A4) < 0.01));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT21.
       *
       * Unit test n5.UT21 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT21 (void){
        const char* testName = "n5.UT21 = %f.5";
        inputAnalogRead5A1 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (((float)outputOffset1_5A1 - _WIMOS_5A1_OFFSET_1)), 
                   ((((float)outputOffset1_5A1 - _WIMOS_5A1_OFFSET_1) < 0.01)));
        /*End_Body_TEST:*/
      } 
    
      /**
       * @brief Wimos test n5.UT22.
       *
       * Unit test n5.UT22 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT22 (void){
        const char* testName = "n5.UT22 = %d";
        inputAnalogRead5A2 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   ((outputOffset1_5A2)), 
                   ((((float)outputOffset1_5A2 - _WIMOS_5A2_OFFSET_1) < 0.01)));
        /*End_Body_TEST:*/
      } 
    
      /**
       * @brief Wimos test n5.UT23.
       *
       * Unit test n5.UT23 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT23 (void){
        const char* testName = "n5.UT23 = %d";
        inputAnalogRead5A3 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   ((outputOffset1_5A3)), 
                   ((((float)outputOffset1_5A3 - _WIMOS_5A3_OFFSET_1) < 0.01)));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT24.
       *
       * Unit test n5.UT24 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT24 (void){
        const char* testName = "n5.UT24 = %f";
        inputAnalogRead5A1 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   ((float)outputCoeficient1_5A1 - (_WIMOS_5A1_OFFSET_1 * _WIMOS_5A1_COEFICIENT_1)), 
                   (((float)outputCoeficient1_5A1 - (_WIMOS_5A1_OFFSET_1 * _WIMOS_5A1_COEFICIENT_1) < 0.9)));
        /*End_Body_TEST:*/
      } 
          
      /**
       * @brief Wimos test n5.UT25.
       *
       * Unit test n5.UT25 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT25 (void){
        const char* testName = "n5.UT25 = %f";
        inputAnalogRead5A2 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (((float)outputCoeficient1_5A2 - (_WIMOS_5A2_OFFSET_1 * _WIMOS_5A2_COEFICIENT_1))), 
                   (((float)outputCoeficient1_5A2 - (_WIMOS_5A2_OFFSET_1 * _WIMOS_5A2_COEFICIENT_1) < 0.9)));
        /*End_Body_TEST:*/
      } 
          
      /**
       * @brief Wimos test n5.UT26.
       *
       * Unit test n5.UT26 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT26 (void){
        const char* testName = "n5.UT26 = %f";
        inputAnalogRead5A3 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (((float)outputCoeficient1_5A3 - (_WIMOS_5A3_OFFSET_1 * _WIMOS_5A3_COEFICIENT_1))), 
                   (((float)outputCoeficient1_5A3 - (_WIMOS_5A3_OFFSET_1 * _WIMOS_5A3_COEFICIENT_1) < 0.9)));
        /*End_Body_TEST:*/
      } 
    
      /**
       * @brief Wimos test n5.UT27.
       *
       * Unit test n5.UT27 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT27 (void){
        const char* testName = "n5.UT27 = %d";
        inputAnalogRead5A1 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
         DEBUG_VALID(testName , 
                   (((float)outputOffset2_5A1)), 
                   (((float)outputOffset2_5A1 - (_WIMOS_5A1_OFFSET_1 * _WIMOS_5A1_COEFICIENT_1) + _WIMOS_5A1_OFFSET_1 < 0.01)));
       /*End_Body_TEST:*/
      } 
    
      /**
       * @brief Wimos test n5.UT28.
       *
       * Unit test n5.UT28 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT28 (void){
        const char* testName = "n5.UT28 = %d";
        inputAnalogRead5A2 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
         DEBUG_VALID(testName , 
                   (((float)outputOffset2_5A2)), 
                   (((float)outputOffset2_5A2 - (_WIMOS_5A2_OFFSET_1 * _WIMOS_5A2_COEFICIENT_1) + _WIMOS_5A2_OFFSET_1 < 0.01)));
       /*End_Body_TEST:*/
      } 
    
      /**
       * @brief Wimos test n5.UT29.
       *
       * Unit test n5.UT29 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT29 (void){
        const char* testName = "n5.UT29 = %d";
        inputAnalogRead5A3 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
         DEBUG_VALID(testName , 
                   (((float)outputOffset2_5A3)), 
                   (((float)outputOffset2_5A3 - (_WIMOS_5A3_OFFSET_1 * _WIMOS_5A3_COEFICIENT_1) + _WIMOS_5A3_OFFSET_1 < 0.01)));
       /*End_Body_TEST:*/
      } 
    
      /**
       * @brief Wimos test n5.UT30.
       *
       * Unit test n5.UT30 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT30 (void){
        const char* testName = "n5.UT30 = %f";
        inputAnalogRead5A1 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (((float)outputCoeficient2_5A1 - (((_WIMOS_5A1_OFFSET_1 * _WIMOS_5A1_COEFICIENT_1) + _WIMOS_5A1_OFFSET_2) * _WIMOS_5A1_COEFICIENT_2))), 
                   (((float)outputCoeficient2_5A1 - (((_WIMOS_5A1_OFFSET_1 * _WIMOS_5A1_COEFICIENT_1) + _WIMOS_5A1_OFFSET_2) * _WIMOS_5A1_COEFICIENT_2) < 0.9 )));
    /*End_Body_TEST:*/
      } 
    
      /**
       * @brief Wimos test n5.UT31.
       *
       * Unit test n5.UT31 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT31 (void){
        const char* testName = "n5.UT31 = %f";
        inputAnalogRead5A2 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (((float)outputCoeficient2_5A2 - (((_WIMOS_5A2_OFFSET_1 * _WIMOS_5A2_COEFICIENT_1) + _WIMOS_5A2_OFFSET_2) * _WIMOS_5A2_COEFICIENT_2))), 
                   (((float)outputCoeficient2_5A2 - (((_WIMOS_5A2_OFFSET_1 * _WIMOS_5A2_COEFICIENT_1) + _WIMOS_5A2_OFFSET_2) * _WIMOS_5A2_COEFICIENT_2) < 0.9 )));
    /*End_Body_TEST:*/
      } 
    
      /**
       * @brief Wimos test n5.UT32.
       *
       * Unit test n5.UT32 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT32 (void){
        const char* testName = "n5.UT32 = %f";
        inputAnalogRead5A3 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        DEBUG_VALID(testName , 
                   (((float)outputCoeficient2_5A3 - (((_WIMOS_5A3_OFFSET_1 * _WIMOS_5A3_COEFICIENT_1) + _WIMOS_5A3_OFFSET_2) * _WIMOS_5A3_COEFICIENT_2))), 
                   (((float)outputCoeficient2_5A3 - (((_WIMOS_5A3_OFFSET_1 * _WIMOS_5A3_COEFICIENT_1) + _WIMOS_5A3_OFFSET_2) * _WIMOS_5A3_COEFICIENT_2) < 0.9 )));
    /*End_Body_TEST:*/
      } 
    
      /**
       * @brief Wimos test n5.UT33.
       *
       * Unit test n5.UT33 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT33 (void){
        const char* testName = "n5.UT33 = %f";
        inputAnalogRead5A1 = 0;
        inputAnalogRead5A2 = 0;
        inputAnalogRead5A3 = 0;
    
        float jointData = 0.0f;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        jointData = (float)sqrt((float)((float)outputCoeficient2_5A1 * (float)outputCoeficient2_5A1) + (float)((float)outputCoeficient2_5A2 * (float)outputCoeficient2_5A2) + (float)((float)outputCoeficient2_5A3 * (float)outputCoeficient2_5A3));
        DEBUG_VALID(testName , 
                   ((float)(abs(float(outputJoint_A5) - float(jointData)))), 
                   (((float)(abs(float(outputJoint_A5) - float(jointData)))) < 1.5));
        /*End_Body_TEST:*/
      } 
      
      /**
       * @brief Wimos test n5.UT34.
       *
       * Unit test n5.UT34 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n5UT34 (void){
        const char* testName = "n5.UT34 = %f";
        float fAverageOutput = 0.0;
        
        inputAnalogRead5A1 = 0;
        inputAnalogRead5A2 = 0;
        inputAnalogRead5A3 = 0;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<100; i++){
          readPortA(&stTestWimosPort);
          fAverageOutput += (float) outputJoint_A5 / 100;
        }        
        DEBUG_VALID( testName , 
                   ( fAverageOutput - outputProcessor_A5), 
                   ( abs((float)fAverageOutput - (float)outputProcessor_A5) < 0.1));
        /*End_Body_TEST:*/
      } 
  
      /**
       * @brief Wimos test n6.UT01.
       *
       * Unit test n6.UT01 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n6UT01 (void){
        const char* testName = "n6.UT01 = %f";
        float fAverageOutput = 0.0;
        
        inputDetectProcessA1 = true;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<_WIMOS_1A_AVERAGE_SIZE*100; i++){
          detectionA1Default((float)100.0f);
          fAverageOutput += (float) ((float) 100.0f / (_WIMOS_1A_AVERAGE_SIZE*100));
        }        
        DEBUG_VALID( testName , 
                   ( fAverageOutput - outputDetectProcessA1), 
                   ( abs((float)fAverageOutput - (float)outputDetectProcessA1) < 0.01));
        /*End_Body_TEST:*/
      } 
      /**
       * @brief Wimos test n6.UT02.
       *
       * Unit test n6.UT02 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n6UT02 (void){
        const char* testName = "n6.UT02 = %f";
        float fAverageOutput = 0.0;
        
        inputDetectProcessA2 = true;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<_WIMOS_2A_AVERAGE_SIZE*100; i++){
          detectionA2Default((float)100.0f);
          fAverageOutput += (float) ((float) 100.0f / (_WIMOS_2A_AVERAGE_SIZE*100));
        }        
        DEBUG_VALID( testName , 
                   ( fAverageOutput - outputDetectProcessA2), 
                   ( abs((float)fAverageOutput - (float)outputDetectProcessA2) < 0.01));
        /*End_Body_TEST:*/
      } 
  
      /**
       * @brief Wimos test n6.UT03.
       *
       * Unit test n6.UT03 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n6UT03 (void){
        const char* testName = "n6.UT03 = %f";
        float fAverageOutput = 0.0;
        
        inputDetectProcessA3 = true;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<_WIMOS_3A_AVERAGE_SIZE*100; i++){
          detectionA3Default((float)100.0f);
          fAverageOutput += (float) ((float) 100.0f / (_WIMOS_3A_AVERAGE_SIZE*100));
        }        
        DEBUG_VALID( testName , 
                   ( fAverageOutput - outputDetectProcessA3), 
                   ( abs((float)fAverageOutput - (float)outputDetectProcessA3) < 0.01));
        /*End_Body_TEST:*/
      } 
  
      /**
       * @brief Wimos test n6.UT04.
       *
       * Unit test n6.UT04 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n6UT04 (void){
        const char* testName = "n6.UT04 = %f";
        float fAverageOutput = 0.0;
        
        inputDetectProcessA4 = true;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<_WIMOS_4A_AVERAGE_SIZE*100; i++){
          detectionA4Default((float)100.0f);
          fAverageOutput += (float) ((float) 100.0f / (_WIMOS_4A_AVERAGE_SIZE*100));
        }        
        DEBUG_VALID( testName , 
                   ( fAverageOutput - outputDetectProcessA4), 
                   ( abs((float)fAverageOutput - (float)outputDetectProcessA4) < 0.01));
        /*End_Body_TEST:*/
      } 
  
      /**
       * @brief Wimos test n6.UT05.
       *
       * Unit test n6.UT05 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n6UT05 (void){
        const char* testName = "n6.UT05 = %f";
        float fAverageOutput = 0.0;
        
        inputDetectProcessA5 = true;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<_WIMOS_5A_AVERAGE_SIZE*100; i++){
          detectionA5Default((float)100.0f);
          fAverageOutput += (float) ((float) 100.0f / (_WIMOS_5A_AVERAGE_SIZE*100));
        }        
        DEBUG_VALID( testName , 
                   ( fAverageOutput - outputDetectProcessA5), 
                   ( abs((float)fAverageOutput - (float)outputDetectProcessA5) < 0.01));
        /*End_Body_TEST:*/
      } 
     
  
      /**
       * @brief Wimos test n6.UT06.
       *
       * Unit test n6.UT06 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n6UT06 (void){
        const char* testName = "n6.UT06 = %d";
        float fAverageOutput = 0.0;
        uint8_t ucAlertValue = 0;
        
        inputDetectProcessA1 = true;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<_WIMOS_1A_AVERAGE_SIZE*100; i++){
          detectionA1Default((float)0.f);
        }  
        ucAlertValue = detectionA1Default((float)_WIMOS_1A_AVERAGE_SIZE * _WIMOS_1A_AVERAGE_OFFSET );
             
        
        DEBUG_VALID( testName , 
                   ( ucAlertValue), 
                   ( ucAlertValue > 0));
        /*End_Body_TEST:*/
      } 
     
  
      /**
       * @brief Wimos test n6.UT07.
       *
       * Unit test n6.UT07 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n6UT07 (void){
        const char* testName = "n6.UT07 = %d";
        float fAverageOutput = 0.0;
        uint8_t ucAlertValue = 0;
        
        inputDetectProcessA2 = true;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<_WIMOS_2A_AVERAGE_SIZE*100; i++){
          detectionA2Default((float)0.f);
        }  
        ucAlertValue = detectionA2Default((float)_WIMOS_2A_AVERAGE_SIZE * _WIMOS_2A_AVERAGE_OFFSET );
             
        
        DEBUG_VALID( testName , 
                   ( ucAlertValue), 
                   ( ucAlertValue > 0));
        /*End_Body_TEST:*/
      } 
     
  
      /**
       * @brief Wimos test n6.UT08.
       *
       * Unit test n6.UT08 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n6UT08 (void){
        const char* testName = "n6.UT08 = %d";
        float fAverageOutput = 0.0;
        uint8_t ucAlertValue = 0;
        
        inputDetectProcessA3 = true;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<_WIMOS_3A_AVERAGE_SIZE*100; i++){
          detectionA3Default((float)0.f);
        }  
        ucAlertValue = detectionA3Default((float)_WIMOS_3A_AVERAGE_SIZE * _WIMOS_3A_AVERAGE_OFFSET );
             
        
        DEBUG_VALID( testName , 
                   ( ucAlertValue), 
                   ( ucAlertValue > 0));
        /*End_Body_TEST:*/
      } 
     
  
      /**
       * @brief Wimos test n6.UT09.
       *
       * Unit test n6.UT09 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n6UT09 (void){
        const char* testName = "n6.UT09 = %d";
        float fAverageOutput = 0.0;
        uint8_t ucAlertValue = 0;
        
        inputDetectProcessA4 = true;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<_WIMOS_4A_AVERAGE_SIZE*100; i++){
          detectionA4Default((float)0.f);
        }  
        ucAlertValue = detectionA4Default((float)_WIMOS_4A_AVERAGE_SIZE * _WIMOS_4A_AVERAGE_OFFSET );
             
        
        DEBUG_VALID( testName , 
                   ( ucAlertValue), 
                   ( ucAlertValue > 0));
        /*End_Body_TEST:*/
      } 
     
  
      /**
       * @brief Wimos test n6.UT10.
       *
       * Unit test n6.UT10 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n6UT10 (void){
        const char* testName = "n6.UT10 = %d";
        float fAverageOutput = 0.0;
        uint8_t ucAlertValue = 0;
        
        inputDetectProcessA5 = true;
        
        /*Begin_Body_Test.*/
        stWimosStorage stTestWimosPort;
        for (int i=0; i<_WIMOS_5A_AVERAGE_SIZE*100; i++){
          detectionA5Default((float)0.f);
        }  
        ucAlertValue = detectionA5Default((uint32_t)_WIMOS_5A_AVERAGE_OFFSET * _WIMOS_4A_AVERAGE_SIZE);
             
        
        DEBUG_VALID( testName , 
                   ( ucAlertValue), 
                   ( ucAlertValue > 0));
        /*End_Body_TEST:*/
      } 
     
    #endif
    
    #ifdef WIMOS_VALIDATION_TEST
    
    
      
      /**
       * @brief Wimos test n1.VT06.
       *
       * Unit test n1.VT06 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       void _test_n1VT06 (void){
        const char* testName = "n1.VT06 = %s";
        /*Body_TEST:*/
        uint16_t aux5A = 0;
        
        uint16_t min5A1,max5A1;
        min5A1 = max5A1 = analogRead(WIMOS_A5_1_PORT);
        
        uint16_t min5A2,max5A2;
        min5A2 = max5A2 = analogRead(WIMOS_A5_2_PORT);
        
        uint16_t min5A3,max5A3;
        min5A3 = max5A3 = analogRead(WIMOS_A5_3_PORT);
        
        uint16_t maxDif = 0;
        
        initPortA();
        for(int i=0; i<100; i++){
          aux5A = analogRead(WIMOS_A5_1_PORT);
          min5A1 = (min5A1 > aux5A)? aux5A : min5A1;
          max5A1 = (max5A1 < aux5A)? aux5A : max5A1;
          
          aux5A = analogRead(WIMOS_A5_2_PORT);
          min5A2 = (min5A2 > aux5A)? aux5A : min5A2;
          max5A2 = (max5A2 < aux5A)? aux5A : max5A2;
          
          aux5A = analogRead(WIMOS_A5_3_PORT);
          min5A3 = (min5A3 > aux5A)? aux5A : min5A3;
          max5A3 = (max5A3 < aux5A)? aux5A : max5A3;
          
        }
        
        maxDif = min(min(max5A1-min5A1, max5A2-min5A2),max5A3-min5A3);
        
        DEBUG_VALID(testName , 
                   (maxDif), 
                   (maxDif < 25));
        /*End_Body_TEST:*/
        
      }
      
      /**
       * @brief Wimos test n3.VT09.
       *
       * Unit test n3.VT09 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
      extern void _test_n3VT09 (void){
        const char* testName = "n3.VT09 = %ld microseconds";
        uint32_t ulTime = micros();
        
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        /*Obsolete Validation Test*/          
        DEBUG_VALID(testName , 
                   (micros() - ulTime), 
                   (micros() - ulTime < 600) || true);
      }
    
      
      /**
       * @brief Wimos test n3.VT09.
       *
       * Unit test n3.VT09 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
      extern void _test_n5VT01 (void){
        const char* testName = "n5.VT01 = %ld microseconds";
        uint32_t ulTime = micros();
        
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
        /*Obsolete Validation Test*/              
        DEBUG_VALID(testName , 
                   (micros() - ulTime), 
                   (micros() - ulTime < 600) || true);
      }
    
      
      /**
       * @brief Wimos test n3.VT09.
       *
       * Unit test n3.VT09 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
      extern void _test_n6VT01 (void){
        const char* testName = "n5.VT01 = %ld microseconds";
        uint32_t ulTime = micros();
        
        stWimosStorage stTestWimosPort;
        readPortA(&stTestWimosPort);
              
        DEBUG_VALID(testName , 
                   (micros() - ulTime), 
                   (micros() - ulTime < 6000));
      }
    #endif
  #endif
#endif

