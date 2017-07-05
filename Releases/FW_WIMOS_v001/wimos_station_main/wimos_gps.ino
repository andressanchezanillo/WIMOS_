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
 * @file wimos_gps.ino
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main function for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */

 
#ifdef __SAM3X8E__
  
  #include "_setting.h"
  #include "main_config.h"
  
  
  #if defined(WIMOS_UNIT_TEST) or defined(WIMOS_VALIDATION_TEST)
    char ucUnitTestGPSInput[125];
    int8_t ucUnitTestGPSOutput;
  #endif
  
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
      stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus |= WIMOS_DEVICE_GPS_MASK;
      DEBUG_OK("GPS initialized.");
    #else  
      stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus &= ~WIMOS_DEVICE_GPS_MASK;
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
      #if defined(WIMOS_UNIT_TEST) or defined(WIMOS_VALIDATION_TEST)
        while(ucUnitTestGPSOutput < (strlen(ucUnitTestGPSInput)-1)){
          if(ucUnitTestGPSInput[ucUnitTestGPSOutput++] == '$')
      #else
            
        while(SERIAL_GPS.available()){
          if(SERIAL_GPS.read() == '$')
      #endif
          {
            machineStateGPS = findCMDGPS;
            return _NEXT_STEP;
          }    
        }
    #endif
    return _ERROR;
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
      char scRecv =0;
      uint8_t i =0;
      const char* pCommandRMC = "GPRMC,";
      DEBUG_INFO("GPS CMD Finder.");
      #if defined(WIMOS_UNIT_TEST) or defined(WIMOS_VALIDATION_TEST)
        if(strlen(ucUnitTestGPSInput) - ucUnitTestGPSOutput > strlen(pCommandRMC)){
          for(i=0; i<strlen(pCommandRMC) && bCmdFound; i++){
          scRecv = ucUnitTestGPSInput[ucUnitTestGPSOutput++];
      #else      
        if(SERIAL_GPS.available() >= strlen(pCommandRMC)){
          for(i=0; i<strlen(pCommandRMC) && bCmdFound; i++){
          scRecv = SERIAL_GPS.read();    
      #endif
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
  
  
      #if defined(WIMOS_UNIT_TEST) or defined(WIMOS_VALIDATION_TEST)
        strcpy(pcGPSBuffer, &ucUnitTestGPSInput[ucUnitTestGPSOutput]);
        SERIAL_DEBUG.println(pcGPSBuffer);
      #else
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
          }
        }
  
      #endif
          
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
        stAuxGpsPosition.stLatitude.slMinute = atoi(&pcGPSAuxBuffer[5]);
        pcGPSAuxBuffer[4] = '\0';
        stAuxGpsPosition.stLatitude.slMinute += atoi(&pcGPSAuxBuffer[2])*1000;
        pcGPSAuxBuffer[2] = '\0';
        stAuxGpsPosition.stLatitude.ucDegree = atoi(pcGPSAuxBuffer);
        
        DEBUG_DATA("GPS Latitude Degree = %d .", stAuxGpsPosition.stLatitude.ucDegree);
        DEBUG_DATA("GPS Latitude Minute = %d .", stAuxGpsPosition.stLatitude.slMinute);
  
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
          stAuxGpsPosition.stLatitude.slMinute = abs(stAuxGpsPosition.stLatitude.slMinute);
        }else if(strlen(pcGPSAuxBuffer) == 1 &&  pcGPSAuxBuffer[0] == 'S'){
          stAuxGpsPosition.stLatitude.slMinute *= -1;
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
        stAuxGpsPosition.stLongitude.slMinute = atoi(&pcGPSAuxBuffer[6]);
        pcGPSAuxBuffer[5] = '\0';
        stAuxGpsPosition.stLongitude.slMinute += atoi(&pcGPSAuxBuffer[3])*1000;
        pcGPSAuxBuffer[3] = '\0';
        stAuxGpsPosition.stLongitude.ucDegree = atoi(pcGPSAuxBuffer);
        
        DEBUG_DATA("GPS Longitude Degree = %d .", stAuxGpsPosition.stLongitude.ucDegree);
        DEBUG_DATA("GPS Latitude Minute = %d .", stAuxGpsPosition.stLongitude.slMinute);
  
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
          stAuxGpsPosition.stLongitude.slMinute = abs(stAuxGpsPosition.stLongitude.slMinute);
        }else if(strlen(pcGPSAuxBuffer) == 1 &&  pcGPSAuxBuffer[0] == 'W'){
          stAuxGpsPosition.stLongitude.slMinute *= -1;
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
      #ifdef DEBUG_GPS
        _stWimosInfo->stGpsPosition.stLatitude.ucDegree = 41;
        _stWimosInfo->stGpsPosition.stLatitude.slMinute = 653189;
        
        _stWimosInfo->stGpsPosition.stLongitude.ucDegree = 4;
        _stWimosInfo->stGpsPosition.stLongitude.slMinute = -731384;
        return;
      #else
        uint8_t ret;
        DEBUG_INFO("GPS update function.");
        ret = machineStateGPS(_stWimosInfo);
        if(ret == _OK){
          DEBUG_OK("GPS updated.");
        }  
        return;  
      #endif
    #endif
  }
  
  
  
  
  #ifdef WIMOS_DEBUG
    #ifdef WIMOS_UNIT_TEST
      
      /**
       * @brief Wimos test n3.UT10.
       *
       * Unit test n3.UT10 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT10 (void){
        const char* testName = "n3.UT10 = %d";
        ucUnitTestGPSOutput = 0;
        /*Body_TEST:*/
        
        #ifdef _EN_WIMOS_GPS
          stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus = 0;
          initGPS();
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_GPS_MASK), 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_GPS_MASK) == WIMOS_DEVICE_GPS_MASK);
        #else
          stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus = WIMOS_DEVICE_RF_MASK;
          initGPS();
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_GPS_MASK), 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_GPS_MASK) == 0x00));
        #endif
        
        /*End_Body_TEST:*/
      } 
  
      /**
       * @brief Wimos test n3.UT11.
       *
       * Unit test n3.UT11 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT11 (void){
        const char* testName = "n3.UT11 = %c";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "Test$toTest" );
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          findTokenGPS(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSInput[ucUnitTestGPSOutput]), 
                     (ucUnitTestGPSInput[ucUnitTestGPSOutput] == 't' && machineStateGPS == findCMDGPS));
        #else      
          machineStateGPS = NULL;
          
          findTokenGPS(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSInput[ucUnitTestGPSOutput]), 
                     (ucUnitTestGPSInput[ucUnitTestGPSOutput] == 'T' && machineStateGPS == NULL));
        #endif
        /*End_Body_TEST:*/
      } 
  
  
      /**
       * @brief Wimos test n3.UT12.
       *
       * Unit test n3.UT12 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT12 (void){
        const char* testName = "n3.UT12 = %c";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "Test@2Test_" );
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          findTokenGPS(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSInput[ucUnitTestGPSOutput]), 
                     (ucUnitTestGPSInput[ucUnitTestGPSOutput] == '_' && machineStateGPS == NULL));
        #else      
          machineStateGPS = NULL;
          
          findTokenGPS(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSInput[ucUnitTestGPSOutput]), 
                     (ucUnitTestGPSInput[ucUnitTestGPSOutput] == 'T' && machineStateGPS == NULL));
        #endif
        /*End_Body_TEST:*/
      } 
  
  
      /**
       * @brief Wimos test n3.UT13.
       *
       * Unit test n3.UT13 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT13 (void){
        const char* testName = "n3.UT13 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "GPRMC,Test" );
        
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = findCMDGPS(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _NEXT_STEP && machineStateGPS == processRMC));
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = findCMDGPS(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR && machineStateGPS == NULL));
        #endif
        /*End_Body_TEST:*/
      } 
  
  
      /**
       * @brief Wimos test n3.UT14.
       *
       * Unit test n3.UT14 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT14 (void){
        const char* testName = "n3.UT14 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "$GPRMC,Test" );
        
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = findCMDGPS(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR && machineStateGPS == findTokenGPS));
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = findCMDGPS(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _OK && machineStateGPS == NULL));
        #endif
        /*End_Body_TEST:*/
      } 
  
  
  
      /**
       * @brief Wimos test n3.UT15.
       *
       * Unit test n3.UT15 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT15 (void){
        const char* testName = "n3.UT15 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A" );
  
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _OK &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 12 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 35 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 19  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 48  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 7038  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 11  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 31000 &&
                      machineStateGPS == findTokenGPS)
                     );
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #endif
        /*End_Body_TEST:*/
      } 
  
  
  
      /**
       * @brief Wimos test n3.UT16.
       *
       * Unit test n3.UT16 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT16 (void){
        const char* testName = "n3.UT16 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "123519,V,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A" );
        
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute = 0;
  
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #endif
        /*End_Body_TEST:*/
      } 
  
  
  
      /**
       * @brief Wimos test n3.UT17.
       *
       * Unit test n3.UT17 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT17 (void){
        const char* testName = "n3.UT17 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "243519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A" );
        
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute = 0;
  
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #endif
        /*End_Body_TEST:*/
      } 
  
  
  
      /**
       * @brief Wimos test n3.UT18.
       *
       * Unit test n3.UT18 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT18 (void){
        const char* testName = "n3.UT18 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "126019,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A" );
        
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute = 0;
  
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #endif
        /*End_Body_TEST:*/
      } 
  
  
  
      /**
       * @brief Wimos test n3.UT19.
       *
       * Unit test n3.UT19 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT19 (void){
        const char* testName = "n3.UT19 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "123567,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A" );
        
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute = 0;
  
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #endif
        /*End_Body_TEST:*/
      } 
  
  
  
      /**
       * @brief Wimos test n3.UT07.
       *
       * Unit test n3.UT07 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT07 (void){
        const char* testName = "n3.UT07 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "123519,A,9107.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A" );
        
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute = 0;
  
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #endif
        /*End_Body_TEST:*/
      } 
  
  
  
      /**
       * @brief Wimos test n3.UT21.
       *
       * Unit test n3.UT21 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT21 (void){
        const char* testName = "n3.UT21 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "123519,A,4107.038,N,18131.000,E,022.4,084.4,230394,003.1,W*6A" );
        
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute = 0;
  
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #endif
        /*End_Body_TEST:*/
      } 
  
  
  
      /**
       * @brief Wimos test n3.UT22.
       *
       * Unit test n3.UT22 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT22 (void){
        const char* testName = "n3.UT22 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "123519,A,4107.038,R,01131.000,E,022.4,084.4,230394,003.1,W*6A" );
        
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute = 0;
  
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #endif
        /*End_Body_TEST:*/
      } 
  
  
  
      /**
       * @brief Wimos test n3.UT23.
       *
       * Unit test n3.UT23 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT23 (void){
        const char* testName = "n3.UT23 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "123519,A,4107.038,N,01131.000,R,022.4,084.4,230394,003.1,W*6A" );
        
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree = 0;
        stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute = 0;
  
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #endif
        /*End_Body_TEST:*/
      } 
  
  
  
      /**
       * @brief Wimos test n3.UT24.
       *
       * Unit test n3.UT24 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT24 (void){
        const char* testName = "n3.UT24 = %d";
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "123519,A,4807.038,S,01131.000,W,022.4,084.4,230394,003.1,W*6A" );
  
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree);
          DEBUG_DATA(testName, stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _OK &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 12 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 35 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 19  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 48  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == -7038  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 11  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == -31000 &&
                      machineStateGPS == findTokenGPS)
                     );
        #else      
  
          machineStateGPS = NULL;
          
          ucUnitTestGPSOutput = processRMC(&stGlobalWimosInfoMsg.stInfo);
          
          DEBUG_VALID(testName , 
                     (ucUnitTestGPSOutput), 
                     (ucUnitTestGPSOutput == _ERROR &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 0 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 0  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 0 &&
                      machineStateGPS == findTokenGPS)
                     );
        #endif
        /*End_Body_TEST:*/
      } 
  
  
  
  
      
    #endif
  
  
    
    
    #ifdef WIMOS_VALIDATION_TEST
    
      
      /**
       * @brief Wimos test n3.VT05.
       *
       * Unit test n3.VT05 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
      extern void _test_n3VT05 (void){
        const char* testName = "n3.VT05 = %s microseconds";
        char ucOutputBuffer[30];
        uint8_t ucSuccessStatus = -1;
        uint32_t ulMaxTimeMicroSeconds = 0;
        uint32_t ulCurrentTime = 0;
        ucUnitTestGPSOutput = 0;
        sprintf(ucUnitTestGPSInput, "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A" );
  
        /*Body_TEST:*/     
        
        #ifdef _EN_WIMOS_GPS
  
          machineStateGPS = findTokenGPS;
          
          ulCurrentTime = micros();
            ucSuccessStatus = machineStateGPS(&stGlobalWimosInfoMsg.stInfo);
          ulMaxTimeMicroSeconds = max(micros() - ulCurrentTime, ulMaxTimeMicroSeconds);
           
          ulCurrentTime = micros();
            ucSuccessStatus = machineStateGPS(&stGlobalWimosInfoMsg.stInfo);
          ulMaxTimeMicroSeconds = max(micros() - ulCurrentTime, ulMaxTimeMicroSeconds);
                      
          ulCurrentTime = micros();
            ucSuccessStatus = machineStateGPS(&stGlobalWimosInfoMsg.stInfo);
          ulMaxTimeMicroSeconds = max(micros() - ulCurrentTime, ulMaxTimeMicroSeconds);
                  
          sprintf(ucOutputBuffer,"%d",ulMaxTimeMicroSeconds);
          DEBUG_VALID(testName , 
                     (ucOutputBuffer), 
                     (ucSuccessStatus == _OK &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucHour == 12 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucMinute == 35 &&
                      stGlobalWimosInfoMsg.stInfo.stDateTime.stTime.ucSecond == 19  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.ucDegree == 48  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLatitude.slMinute == 7038  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.ucDegree == 11  &&
                      stGlobalWimosInfoMsg.stInfo.stGpsPosition.stLongitude.slMinute == 31000 &&
                      machineStateGPS == findTokenGPS && ulMaxTimeMicroSeconds < 10000)
                     );
        #else      
  
  
        #endif
        /*End_Body_TEST:*/
      } 
      
    #endif
  #endif
  
  /*
  int8_t findTokenGPS(stWimosInfo* _stWimosInfo);
  int8_t findCMDGPS(stWimosInfo* _stWimosInfo);
  int8_t processRMC(stWimosInfo* _stWimosInfo);
  */
#endif
