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
 * @file wimos_memory.ino
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main function for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */


#ifdef __SAM3X8E__
  #include <SPI.h>
  #include <SD.h>
  #include "_setting.h"
  #include "main_config.h"
  
  uint16_t usLastNonVolatileFile = 0;
  uint16_t usLastVolatileFile = 1;
  uint32_t ulTimerSD = 0;
  PeriphericErrors errorSD = UKNOWN_ERROR;
  File stVolatileFile;
  char ptrVolatileFileName[22];
  char ptrNonVolatileFileName[22];
  File stNonVolatileFile;
  
  
  #if defined(WIMOS_UNIT_TEST) or defined(WIMOS_VALIDATION_TEST)
    char ptrUnitTestSDInput[25];
    uint8_t ucUnitTestSDInput;
    uint8_t ucUnitTestSDOutput;
    uint32_t ulUnitTestSDOutput;
    bool bUnitTestSDInput;
    int8_t scUnitTestSDOutput;
  #endif
  
  
  int8_t initSystemSD(void);
  uint32_t getLastFile(const char* ptrDir, const char* prtFileName, const char* prtExtension );
  uint32_t writeLineSD(const char * str);
  
  /**
   * @brief SD Initialization.
   *
   * This function initializes the SD interface.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern int32_t initSD(void){
    #ifdef _EN_WIMOS_SD
      int32_t slRetValue = 0;
      if(SD.begin(WIMOS_SD_CS_PIN)){
        
        /* Initialization is properly*/
        errorSD = OK_INITIALIZATION;
        
        /* Set the status devices SD as enabled*/
        stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus |= WIMOS_DEVICE_SD_MASK;
        
        /* Set SystemFile */
        initSystemSD();
        
        /* Get the last non volatile file wroten*/
        usLastNonVolatileFile =  getLastFile(WIMOS_NON_VOLATILE_DIR, WIMOS_NON_VOLATILE_FILE, WIMOS_NON_VOLATILE_EXT );
        DEBUG_DATA("First non volatil file = %d ",usLastNonVolatileFile);
        
        /* Get percent of memory used*/
        slRetValue = ( (usLastNonVolatileFile-1) / WIMOS_NON_VOLATILE_FILE_MAX) * 100;
        DEBUG_DATA("Memory status = %d /100",slRetValue);
        
        /* Set the non volatile file*/
        sprintf(ptrNonVolatileFileName, "%s/%s%d%s", WIMOS_NON_VOLATILE_DIR, WIMOS_NON_VOLATILE_FILE, usLastNonVolatileFile, WIMOS_NON_VOLATILE_EXT);
        stNonVolatileFile = SD.open(ptrNonVolatileFileName, FILE_WRITE);  
        ulTimerSD= millis();
        stNonVolatileFile.println("\"Date\";\"Time\";\"PortA1(RAW)\";\"PortA1(Offset)\";\"PortA1(Alert Level)\";\"PortA2(RAW)\";\"PortA2(Offset)\";\"PortA2(Alert Level)\";\"PortA3(RAW)\";\"PortA3(Offset)\";\"PortA3(Alert Level)\";\"PortA4(RAW)\";\"PortA4(Offset)\";\"PortA4(Alert Level)\";\"PortA5(RAW)\";\"PortA5(Offset)\";\"PortA5(Alert Level)\";\"IMU (ACC)\";\"IMU (GyX)\";\"IMU (GyY)\";");

        
        DEBUG_OK("SD Memory initialized.");
      }else{
        slRetValue = -1;
        errorSD = ERROR_INITIALIZATION;
        stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus &= ~WIMOS_DEVICE_SD_MASK;
        DEBUG_ERROR("SD Memory initialization ERROR.");
      }
      
    #else
      slRetValue = -1;
      /* Initialization is wrong*/
      errorSD = ERROR_INITIALIZATION;
      DEBUG_INFO("SD Memory not available.");
    
    #endif
    return slRetValue;
  }

   
  int8_t initSystemSD(void){
    #ifdef _EN_WIMOS_SD
        File stAuxFile;
        uint32_t ulRetValue = 0;  
        
        /*If the non volatile directory folder*/
        if(!SD.exists(WIMOS_NON_VOLATILE_DIR)){
          DEBUG_INFO(WIMOS_NON_VOLATILE_DIR" does NOT exist.");  
          
          /*Create the non volatile folder*/
          SD.mkdir(WIMOS_NON_VOLATILE_DIR);
        }else{
          DEBUG_INFO(WIMOS_NON_VOLATILE_DIR" exist.");  
        }
          
        return _OK;
    #endif
    return _ERROR;
  }
  
  uint32_t getLastFile(const char* ptrDir, const char* prtFileName, const char* prtExtension ){
    #ifdef _EN_WIMOS_SD
        uint32_t ulRetValue = 0;
        char ptrNameBuffer[22];
        bool bFound = false;
        uint16_t usIndexMax = WIMOS_NON_VOLATILE_FILE_MAX;
        uint16_t usIndexMin = 1;
        /*Logarism seach for find the last file wrotten (it is sorted)*/
        while(!bFound){
               
          DEBUG_DATA("Index = %d",usIndexMin+((usIndexMax-usIndexMin)/2));
          if(usIndexMax - usIndexMin < 2){
            bFound = true; 
            sprintf(ptrNameBuffer, "%s/%s%d%s",ptrDir,prtFileName,usIndexMin,prtExtension);
            if(SD.exists(ptrNameBuffer))
              ulRetValue = usIndexMax;
            else
              ulRetValue = usIndexMin;
            
          }else{
            sprintf(ptrNameBuffer, "%s/%s%d%s",ptrDir,prtFileName,usIndexMin+((usIndexMax-usIndexMin)/2),prtExtension);
            if(SD.exists(ptrNameBuffer)){
               usIndexMin = usIndexMin+((usIndexMax-usIndexMin)/2);
            }else{
               usIndexMax = usIndexMin+((usIndexMax-usIndexMin)/2);
            }
          }  
        }
        /*Return the last file wrotten.*/
        return ulRetValue;
    #endif
    return 0;
  }
  
 extern uint32_t writeFrameSD (const stWimosStorage* stWimosPort){
  char ucFrame[400];
  
  sprintf(ucFrame,"\"%d/%d/%d\";\"%d:%d:%d\";%f.6;%f.6;%d;%f.6;%f.6;%d;%f.6;%f.6;%d;%f.6;%f.6;%d;%f.6;%f.6;%d;%d;%d;%d",
  stWimosPort->stDateTime.stDate.ucDay,stWimosPort->stDateTime.stDate.ucMonth,stWimosPort->stDateTime.stDate.ucYear,
  stWimosPort->stDateTime.stTime.ucHour,stWimosPort->stDateTime.stTime.ucMinute,stWimosPort->stDateTime.stTime.ucSecond,
  stWimosPort->usPort1A,stWimosPort->usOffset1A,stWimosPort->usAlert1A,
  stWimosPort->usPort2A,stWimosPort->usOffset2A,stWimosPort->usAlert2A,
  stWimosPort->usPort3A,stWimosPort->usOffset3A,stWimosPort->usAlert3A,
  stWimosPort->usPort4A,stWimosPort->usOffset4A,stWimosPort->usAlert4A,
  stWimosPort->usPort5A,stWimosPort->usOffset5A,stWimosPort->usAlert5A,
  stWimosPort->ulImuAcc,stWimosPort->ulImuGx,stWimosPort->ulImuGy);
  
  return writeLineSD(ucFrame);
 }
  

 uint32_t writeLineSD (const char* str){
  int32_t slRetValue = stGlobalWimosInfoMsg.stInfo.ucPercentMemory;
  #ifdef _EN_WIMOS_SD
    if (millis() -  ulTimerSD >= 60000){
      stNonVolatileFile.close();
      
      /* Get the last non volatile file wroten*/
      usLastNonVolatileFile =  getLastFile(WIMOS_NON_VOLATILE_DIR, WIMOS_NON_VOLATILE_FILE, WIMOS_NON_VOLATILE_EXT );
      DEBUG_DATA("First non volatil file = %d ",usLastNonVolatileFile);
      
      /* Get percent of memory used*/
      slRetValue = ( (usLastNonVolatileFile-1) / WIMOS_NON_VOLATILE_FILE_MAX) * 100;
      DEBUG_DATA("Memory status = %d /100",slRetValue);
        
      /* Set the non volatile file*/
      sprintf(ptrNonVolatileFileName, "%s/%s%d%s", WIMOS_NON_VOLATILE_DIR, WIMOS_NON_VOLATILE_FILE, usLastNonVolatileFile, WIMOS_NON_VOLATILE_EXT);
      stNonVolatileFile = SD.open(ptrNonVolatileFileName, FILE_WRITE);  
      ulTimerSD= millis();
      stNonVolatileFile.println("\"Date\";\"Time\";\"PortA1(RAW)\";\"PortA1(Offset)\";\"PortA1(Alert Level)\";\"PortA2(RAW)\";\"PortA2(Offset)\";\"PortA2(Alert Level)\";\"PortA3(RAW)\";\"PortA3(Offset)\";\"PortA3(Alert Level)\";\"PortA4(RAW)\";\"PortA4(Offset)\";\"PortA4(Alert Level)\";\"PortA5(RAW)\";\"PortA5(Offset)\";\"PortA5(Alert Level)\";\"IMU (ACC)\";\"IMU (GyX)\";\"IMU (GyY)\";");
    }
    stNonVolatileFile.println(str);
    return slRetValue;
  #endif
  return slRetValue;
  }
  
  
  #ifdef WIMOS_DEBUG
    #ifdef WIMOS_UNIT_TEST
      
      /**
       * @brief Wimos test n3.UT29.
       *
       * Unit test n3.UT29 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT29 (void){
        const char* testName = "n3.UT29 = %d";
        /*Body_TEST:*/
        
        #ifdef _EN_WIMOS_SD
          stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus = 0;
          initSD();
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_SD_MASK), 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_SD_MASK) == WIMOS_DEVICE_SD_MASK);
        #else
          stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus = WIMOS_DEVICE_SD_MASK;
          initSD();
          DEBUG_VALID(testName , 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_SD_MASK), 
                     (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_SD_MASK) == 0x00);
        #endif
        /*End_Body_TEST:*/
      } 
  
      /**
       * @brief Wimos test n3.UT30.
       *
       * Unit test n3.UT30 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT30 (void){
        const char* testName = "n3.UT30 = %d";
        uint8_t i=0, j=0, h=0;
        bool bTest = true;
        
        /*Body_TEST:*/     
        /*TODO: test*/ 
        initSystemSD();
        bUnitTestSDInput = true;
        ucUnitTestSDInput = 5;
        uint32_t ulOutput[10] = {0};
        
        for(i=0; i<10; i++){
          nextVolatileFile();
          for(j=0; j<5; j++){
            storeVolatile(&h, 1);
            h = (h+1) % 256;
          }
        }
        ucUnitTestSDInput = 10;
        restoreVolatileStatus(ulOutput, 10);
  
        #ifdef _EN_WIMOS_SD
        
          for(i=0; i<5; i++)
            bTest &= (ulOutput[i] == 5);
            
          for(i=5; i<10; i++)
            bTest &= (ulOutput[i] == 0);
          
          DEBUG_VALID(testName , 
                     (bTest), 
                     (true));
        #else      
        
          for(i=0; i<10; i++)
            bTest &= (ulOutput[i] == 0);
                  
          DEBUG_VALID(testName , 
                     (bTest), 
                     (bTest));
        #endif
        delay(10);
        /*End_Body_TEST:*/
      } 
  
      /**
       * @brief Wimos test n3.UT31.
       *
       * Unit test n3.UT31 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT31 (void){
        
        const char* testName = "n3.UT31 = %d";
        uint8_t i=0, j=0, h=0;
        bool bTest = true;
        
        /*Body_TEST:*/     
        /*TODO: test*/ 
        initSystemSD();
        bUnitTestSDInput = true;
        ucUnitTestSDInput = 10;
        uint32_t ulOutput[10] = {0};
        
        for(i=0; i<10; i++){
          nextVolatileFile();
          for(j=0; j<5; j++){
            storeVolatile(&h, 1);
            h = (h+1) % 256;
          }
        }
        restoreVolatileStatus(ulOutput, 10);
        #ifdef _EN_WIMOS_SD
        
          for(i=0; i<10; i++)
            bTest &= (ulOutput[i] == 5);
          
          DEBUG_VALID(testName , 
                     (bTest), 
                     (true));
        #else      
        
          for(i=0; i<10; i++)
            bTest &= (ulOutput[i] == 0);
                  
          DEBUG_VALID(testName , 
                     (bTest), 
                     (bTest));
        #endif
        delay(10);
        /*End_Body_TEST:*/
      } 
  
      /**
       * @brief Wimos test n3.UT32.
       *
       * Unit test n3.UT32 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
       extern void _test_n3UT32 (void){
        const char* testName = "n3.UT32 = %d";
        uint8_t i, j, h;
        /*Body_TEST:*/    
        /*TODO: test*/     
        initSystemSD();
        bUnitTestSDInput = true;
        ucUnitTestSDInput = 5;
        ucUnitTestSDOutput = 0;
        
        for(i=0; i<5; i++){
          nextVolatileFile();
          for(j=0; j<5; j++){
            storeVolatile(&h, 1);
            h = (h+1) % 256;
          }
        }
        
        for(i=0; i<5; i++){ 
          sprintf(ptrVolatileFileName, "%s/%s%d%s", WIMOS_VOLATILE_DIR, WIMOS_VOLATILE_FILE, i, WIMOS_VOLATILE_EXT);
          if( SD.exists(ptrVolatileFileName) ){
            sprintf(ptrNonVolatileFileName, "%s%d%s", WIMOS_NON_VOLATILE_FILE, i, WIMOS_NON_VOLATILE_EXT);
            sprintf(ptrVolatileFileName, "%s%d%s", WIMOS_VOLATILE_FILE, i, WIMOS_VOLATILE_EXT);          
            moveFileToDirectory(WIMOS_VOLATILE_DIR,ptrVolatileFileName,WIMOS_NON_VOLATILE_DIR,ptrNonVolatileFileName);
          }
        }
        
        ucUnitTestSDOutput = getLastFile(WIMOS_NON_VOLATILE_DIR, WIMOS_NON_VOLATILE_FILE, WIMOS_NON_VOLATILE_EXT);
        
        #ifdef _EN_WIMOS_SD
        
          
          DEBUG_VALID(testName , 
                     (ucUnitTestSDOutput), 
                     (ucUnitTestSDOutput == 5));
        #else      
        
          DEBUG_VALID(testName , 
                     (ucUnitTestSDOutput), 
                     (ucUnitTestSDOutput == 0));
        #endif
        delay(10);
        /*End_Body_TEST:*/
      } 
  
      
    #endif
    
    #ifdef WIMOS_VALIDATION_TEST
    
      
      /**
       * @brief Wimos test n3.VT07.
       *
       * Unit test n3.VT07 function.
       * @verbatim like this@endverbatim 
       * @param none.
       * @return none.
       */
      extern void _test_n3VT07 (void){
        const char* testName = "n3.VT07 = %ld microseconds";
        uint8_t i, j, h;
        /*Body_TEST:*/         
        initSystemSD();
        bUnitTestSDInput = true;
        ucUnitTestSDInput = 5;
        ucUnitTestSDOutput = 0;
        
        for(i=0; i<5; i++){
          nextVolatileFile();
          for(j=0; j<5; j++){
            storeVolatile(&h, 1);
            h = (h+1) % 256;
          }
        }
        
        ulUnitTestSDOutput = micros();
        for(i=0; i<5; i++){ 
          sprintf(ptrVolatileFileName, "%s/%s%d%s", WIMOS_VOLATILE_DIR, WIMOS_VOLATILE_FILE, i, WIMOS_VOLATILE_EXT);
          if( SD.exists(ptrVolatileFileName) ){
            sprintf(ptrNonVolatileFileName, "%s%d%s", WIMOS_NON_VOLATILE_FILE, i, WIMOS_NON_VOLATILE_EXT);
            sprintf(ptrVolatileFileName, "%s%d%s", WIMOS_VOLATILE_FILE, i, WIMOS_VOLATILE_EXT);          
            moveFileToDirectory(WIMOS_VOLATILE_DIR,ptrVolatileFileName,WIMOS_NON_VOLATILE_DIR,ptrNonVolatileFileName);
          }
        }      
        ulUnitTestSDOutput = micros() - ulUnitTestSDOutput;
        ulUnitTestSDOutput /= 5;
        
        #ifdef _EN_WIMOS_SD
        
          
          DEBUG_VALID(testName , 
                     (ulUnitTestSDOutput), 
                     (ucUnitTestSDOutput < 10000));
        #else      
        
          DEBUG_VALID(testName , 
                     (ulUnitTestSDOutput), 
                     (ucUnitTestSDOutput < 10000));
        #endif
        delay(10);
        /*End_Body_TEST:*/
      } 
      
    #endif
  #endif
#endif  

