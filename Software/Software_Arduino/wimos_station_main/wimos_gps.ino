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

int8_t findTokenGPS(stWimosInfo* _stWimosInfo);
int8_t findCMDGPS(stWimosInfo* _stWimosInfo);
int8_t processRMC(stWimosInfo* _stWimosInfo);

int8_t splitByChar(const char* pcBufferIn, char* pcBufferOut, uint8_t* ucBufferInBegin, uint8_t index);

int8_t (*machineStateGPS)(stWimosInfo* _stWimosInfo) = findTokenGPS;

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
    SERIAL_GPS.begin(BAUDRATE_GPS);  
    DEBUG_OK("GPS initialized.");
  #else
    DEBUG_INFO("GPS not initialized.");
  #endif
}

/**
 * @brief GPS Token "$" finder.
 *
 * This function finds the GPS "$" token.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
int8_t findTokenGPS(stWimosInfo* _stWimosInfo){
  #ifdef _EN_WIMOS_GPS
    DEBUG_INFO("GPS $ Finder.");
    while(SERIAL_GPS.available()){
      if(SERIAL_GPS.read() == '$'){
        machineStateGPS = findCMDGPS;
        return _NEXT_STEP;
      }
    }
    return _ERROR;
  #endif
}

/**
 * @brief GPS CMD finder.
 *
 * This function finds the GPS "$" token.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
int8_t findCMDGPS(stWimosInfo* _stWimosInfo){
  #ifdef _EN_WIMOS_GPS
    bool bCmdFound = true;
    uint8_t i =0;
    char scRecv =0;
    const char* pCommandRMC = "GPRMC,";
    DEBUG_INFO("GPS CMD Finder.");
    if(SERIAL_GPS.available() >= strlen(pCommandRMC)){
      for(i=0; i<strlen(pCommandRMC); i++){
        scRecv = SERIAL_GPS.read();
        DEBUG_DATA("GPS CMD Finder = %c.",scRecv);
        bCmdFound &= (scRecv == pCommandRMC[i]);
      }
        
      if(bCmdFound == true){
        DEBUG_INFO("GPS RMC Found.");
        machineStateGPS = processRMC;
        return _NEXT_STEP;
      }else{
        machineStateGPS = findTokenGPS;
        return _ERROR;
      }
    }
    return _ERROR;
  #endif
}

/**
 * @brief GPS Token "$" finder.
 *
 * This function finds the GPS "$" token.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
int8_t processRMC(stWimosInfo* _stWimosInfo){
  #ifdef _EN_WIMOS_GPS
  
    static char pcGPSBuffer[125] = {0};
    static uint8_t ucGPSBufferIndex = 0;
    uint8_t ucBufferIndex = 0;
    char scRecv = 0;
    char pcGPSAuxBuffer[20] = {0};
    uint8_t ucAuxBufferIndex = 0;
    uint32_t ulData = 0;
    
    stWimosDateTime stAuxDateTime; 
    stWimosGpsPosition stAuxGpsPosition; 
    uint8_t ucRecv = 0;
    
    DEBUG_INFO("RMC Processor.");
    while(SERIAL_GPS.available()){
      scRecv = SERIAL_GPS.read();
      if( scRecv != '\n'){    
        /*Build Frame. */
        pcGPSBuffer[ucGPSBufferIndex] = scRecv;
        ucGPSBufferIndex++;
        
      }else{
        /* Finalize Frame */
        pcGPSBuffer[ucGPSBufferIndex] = '\0';
        
        /** Clear buffer **/
        DEBUG_INFO("Frame Received and Buffer cleared."); 
        ucGPSBufferIndex=0;
        while(SERIAL_GPS.available()){
          SERIAL_GPS.read();
        }
        
        /** Split Fix Code **/
        ucAuxBufferIndex = 0;
        ucRecv = splitByChar(pcGPSBuffer,pcGPSAuxBuffer,&ucAuxBufferIndex,1);
        
        if(ucRecv == _ERROR){
          DEBUG_ERROR("GPS SPLITTER ERROR.");
          machineStateGPS = findTokenGPS;
          return _ERROR;
        }
        /* IS FIXED GPS*/
        /*************************************************************************************TODO Cambiar V por A******************************************************/
        if(strlen(pcGPSAuxBuffer) == 1 &&  pcGPSAuxBuffer[0] == 'A'){
          DEBUG_INFO("GPS Fix.");
          /* Process Time Values */
          ucAuxBufferIndex = 0;
          ucRecv = splitByChar(pcGPSBuffer,pcGPSAuxBuffer,&ucAuxBufferIndex,0);
          
          if(ucRecv == _ERROR){
            DEBUG_ERROR("GPS SPLITTER ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }
          
          ulData = atoi(pcGPSAuxBuffer);
          stAuxDateTime.stTime.ucSecond = atoi(&pcGPSAuxBuffer[4]);
          pcGPSAuxBuffer[4] = '\0';
          stAuxDateTime.stTime.ucMinute = atoi(&pcGPSAuxBuffer[2]);
          pcGPSAuxBuffer[2] = '\0';
          stAuxDateTime.stTime.ucHour = atoi(pcGPSAuxBuffer);
          DEBUG_DATA("GPS Hour = %d .",stAuxDateTime.stTime.ucHour);
          DEBUG_DATA("GPS Minute = %d .",stAuxDateTime.stTime.ucMinute);
          DEBUG_DATA("GPS Second = %d .",stAuxDateTime.stTime.ucSecond);

          if(stAuxDateTime.stTime.ucSecond >= 60 || stAuxDateTime.stTime.ucMinute >= 60 || stAuxDateTime.stTime.ucHour >= 24 ){
            DEBUG_ERROR("GPS Time ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }
          
          /* Process Latitude Values */
          ucRecv = splitByChar(pcGPSBuffer,pcGPSAuxBuffer,&ucAuxBufferIndex,1);
          
          if(ucRecv == _ERROR){
            DEBUG_ERROR("GPS SPLITTER ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }
          
          ulData = atoi(pcGPSAuxBuffer);
          stAuxGpsPosition.stLatitude.ucMinute = atoi(&pcGPSAuxBuffer[5]);
          pcGPSAuxBuffer[4] = '\0';
          stAuxGpsPosition.stLatitude.ucMinute += atoi(&pcGPSAuxBuffer[2])*1000;
          pcGPSAuxBuffer[2] = '\0';
          stAuxGpsPosition.stLatitude.ucDegree = atoi(pcGPSAuxBuffer);
          
          DEBUG_DATA("GPS Latitude Degree = %d .", stAuxGpsPosition.stLatitude.ucDegree);
          DEBUG_DATA("GPS Latitude Minute = %d .", stAuxGpsPosition.stLatitude.ucMinute);

          if(stAuxGpsPosition.stLatitude.ucDegree > 90){
            DEBUG_ERROR("GPS Latitude ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }

          ucRecv = splitByChar(pcGPSBuffer,pcGPSAuxBuffer,&ucAuxBufferIndex,0);
          
          if(ucRecv == _ERROR){
            DEBUG_ERROR("GPS SPLITTER ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }
          if(strlen(pcGPSAuxBuffer) == 1 &&  pcGPSAuxBuffer[0] == 'N'){
            stAuxGpsPosition.stLatitude.ucMinute = abs(stAuxGpsPosition.stLatitude.ucMinute);
          }else if(strlen(pcGPSAuxBuffer) == 1 &&  pcGPSAuxBuffer[0] == 'S'){
            stAuxGpsPosition.stLatitude.ucMinute *= -1;
          }else{
            DEBUG_ERROR("GPS LATITUDE N/S ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }

          
          /* Process Longitude Values */
          ucRecv = splitByChar(pcGPSBuffer,pcGPSAuxBuffer,&ucAuxBufferIndex,0);
          
          if(ucRecv == _ERROR){
            DEBUG_ERROR("GPS SPLITTER ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }
          
          ulData = atoi(pcGPSAuxBuffer);
          stAuxGpsPosition.stLongitude.ucMinute = atoi(&pcGPSAuxBuffer[6]);
          pcGPSAuxBuffer[5] = '\0';
          stAuxGpsPosition.stLongitude.ucMinute += atoi(&pcGPSAuxBuffer[3])*1000;
          pcGPSAuxBuffer[3] = '\0';
          stAuxGpsPosition.stLongitude.ucDegree = atoi(pcGPSAuxBuffer);
          
          DEBUG_DATA("GPS Longitude Degree = %d .", stAuxGpsPosition.stLongitude.ucDegree);
          DEBUG_DATA("GPS Latitude Minute = %d .", stAuxGpsPosition.stLongitude.ucMinute);

          if(stAuxGpsPosition.stLongitude.ucDegree > 180){
            DEBUG_ERROR("GPS Longitude ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }

          ucRecv = splitByChar(pcGPSBuffer,pcGPSAuxBuffer,&ucAuxBufferIndex,0);
          
          if(ucRecv == _ERROR){
            DEBUG_ERROR("GPS SPLITTER ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }
          if(strlen(pcGPSAuxBuffer) == 1 &&  pcGPSAuxBuffer[0] == 'E'){
            stAuxGpsPosition.stLongitude.ucMinute = abs(stAuxGpsPosition.stLongitude.ucMinute);
          }else if(strlen(pcGPSAuxBuffer) == 1 &&  pcGPSAuxBuffer[0] == 'W'){
            stAuxGpsPosition.stLongitude.ucMinute *= -1;
          }else{
            DEBUG_ERROR("GPS LONGITUDE N/S ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }
          /*Process Date value*/
          ucRecv = splitByChar(pcGPSBuffer,pcGPSAuxBuffer,&ucAuxBufferIndex,2);
          
          if(ucRecv == _ERROR){
            DEBUG_ERROR("GPS SPLITTER ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }
          
          ulData = atoi(pcGPSAuxBuffer);
          stAuxDateTime.stDate.ucYear = atoi(&pcGPSAuxBuffer[4]);
          pcGPSAuxBuffer[4] = '\0';
          stAuxDateTime.stDate.ucMonth = atoi(&pcGPSAuxBuffer[2]);
          pcGPSAuxBuffer[2] = '\0';
          stAuxDateTime.stDate.ucDay = atoi(pcGPSAuxBuffer);
          DEBUG_DATA("GPS Year = %d .",stAuxDateTime.stDate.ucYear);
          DEBUG_DATA("GPS Month = %d .",stAuxDateTime.stDate.ucMonth);
          DEBUG_DATA("GPS Day = %d .",stAuxDateTime.stDate.ucDay);

          if(stAuxDateTime.stDate.ucYear < 16 || stAuxDateTime.stDate.ucMonth > 12 || stAuxDateTime.stDate.ucDay > 31 ){
            DEBUG_ERROR("GPS Date ERROR.");
            machineStateGPS = findTokenGPS;
            return _ERROR;
          }
          
          /*Update values*/
          _stWimosInfo->stDateTime = stAuxDateTime;
          _stWimosInfo->stGpsPosition = stAuxGpsPosition;
                    
          machineStateGPS = findTokenGPS;
          return _OK;
          
        /* IS NOT FIXED GPS*/
        }else if (strlen(pcGPSAuxBuffer) == 1 &&  pcGPSAuxBuffer[0] == 'V'){
          DEBUG_INFO("GPS no Fix [ERROR].");
          machineStateGPS = findTokenGPS;
          return _ERROR;
          
        /* IT IS A UNKNOW VALUE.*/
        }else{
          DEBUG_ERROR("GPS UKNOWN ERROR.");
          DEBUG_DATA("GPS FIX = %c .",pcGPSBuffer[ucBufferIndex]);
          DEBUG_DATA("GPS FIX Index = %d .",ucBufferIndex);
          machineStateGPS = findTokenGPS;
          return _ERROR;
        }
      }
    }
    return _ERROR;
  #endif
}


/**
 * @brief GPS frame splitter.
 *
 * This function splits the GPS parameters.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
int8_t splitByChar(const char* pcBufferIn, char* pcBufferOut, uint8_t* ucBufferInBegin, uint8_t index){
  uint8_t ucBufferIndex = 0;
  uint8_t i=0; 
  
  DEBUG_DATA("GPS Splitter BufferIN = %s .",pcBufferIn);
  DEBUG_DATA("GPS Splitter BeginIndex = %d .",*ucBufferInBegin);
  DEBUG_DATA("GPS Splitter Index = %d .",index);
  
  for(i=0; i<index; i++){
    while(pcBufferIn[(*ucBufferInBegin)] != ','){
      if(pcBufferIn[(*ucBufferInBegin)] == '\0'){
        DEBUG_ERROR("Split overflow ERROR.");        
        return _ERROR;
      }else{
        (*ucBufferInBegin)++;
        DEBUG_DATA("GPS Splitter Index = %d .",(*ucBufferInBegin));
      }
    }
    (*ucBufferInBegin)++;
  }
    
  DEBUG_DATA("GPS Splitter Buffer Begin= %s .",&pcBufferIn[*ucBufferInBegin]);
  while(pcBufferIn[(*ucBufferInBegin)] != ',' && pcBufferIn[(*ucBufferInBegin)] != '\0'){
    pcBufferOut[ucBufferIndex++] = pcBufferIn[(*ucBufferInBegin)++];
  }
  (*ucBufferInBegin)++;
  pcBufferOut[ucBufferIndex] = '\0';
  DEBUG_DATA("GPS Splitter BufferOUT = %s .",pcBufferOut);      
  return _OK;
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
    uint8_t ret;
    DEBUG_INFO("GPS update function.");
    ret = machineStateGPS(_stWimosInfo);
    if(ret == _OK){
      DEBUG_OK("GPS updated.");
    }  
    return;  
  #endif
}
