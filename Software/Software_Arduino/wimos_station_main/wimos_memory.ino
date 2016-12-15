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



void writeFrameToFile(File* stFileToWrite, const void* ptrBuffer, uint16_t usBufferSize);
int8_t initSystemSD(void);
uint32_t getLastFile(const char* ptrDir, const char* prtFileName, const char* prtExtension );

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
      sprintf(ptrVolatileFileName, "%s/%s%d%s", WIMOS_VOLATILE_DIR, WIMOS_VOLATILE_FILE, usLastVolatileFile, WIMOS_VOLATILE_EXT);
      stVolatileFile = SD.open(ptrVolatileFileName, FILE_READ);  
      ulTimerSD= millis();
      
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

/**
 * @brief SD status updater.
 *
 * This function updates the SD status value.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return _stWimosInfo It returns GPS position and Datetime updated into stWimosInfo struct .
 */

 
int8_t initSystemSD(void){
  #ifdef _EN_WIMOS_SD
    if( errorSD == OK_INITIALIZATION ){
      File stAuxFile;
      uint32_t ulRetValue = 0;
      
      /*If the non volatile directory folder*/
      if(SD.exists(WIMOS_VOLATILE_DIR)){
        DEBUG_INFO(WIMOS_VOLATILE_DIR" exist.");  

        /*Clear the non volatile folder*/
        SD.rmdir(WIMOS_VOLATILE_DIR);
      }else{
        DEBUG_INFO(WIMOS_VOLATILE_DIR" does NOT exist.");    
      }
      /*Create the non volatile folder*/
      SD.mkdir(WIMOS_VOLATILE_DIR);

      
      /*If the non volatile directory folder*/
      if(!SD.exists(WIMOS_NON_VOLATILE_DIR)){
        DEBUG_INFO(WIMOS_NON_VOLATILE_DIR" does NOT exist.");  
        
        /*Create the non volatile folder*/
        SD.mkdir(WIMOS_NON_VOLATILE_DIR);
      }else{
        DEBUG_INFO(WIMOS_NON_VOLATILE_DIR" exist.");  
      }
      
      /*Set the current system signature.*/
      stWimosSignature _stCurrentSignature;
      _stCurrentSignature.ucWimosID = WIMOS_ID;
      _stCurrentSignature.stStatus = stGlobalWimosInfoMsg.stInfo.stStatus;
      
      /*If the WIMOS Signature in SD exists*/
      if(SD.exists(WIMOS_SIGN_FILE)){      
        DEBUG_INFO(WIMOS_SIGN_FILE" exist.");  
        stWimosSignature _stFileSignature;
        
        /*Open the current signature file*/
        stAuxFile = SD.open(WIMOS_SIGN_FILE,FILE_READ); 
           
        /*If the file size is equal with the current signature (see the last EOF char)*/
        if(stAuxFile.size() == sizeof(_stCurrentSignature)){
          DEBUG_INFO("Signature size match");            
          
          /*Read the current signature*/
          stAuxFile.read((uint8_t*) &_stFileSignature, sizeof(_stFileSignature));
          stAuxFile.close(); 
          
          DEBUG_DATA("Current Signature ucWimosID = %d",_stCurrentSignature.ucWimosID);   
          DEBUG_DATA("Current Signature DeviceStatus = %d", _stCurrentSignature.stStatus.ucDeviceStatus);   
          DEBUG_DATA("Current Signature DeviceStatus = %d", _stCurrentSignature.stStatus.usPortStatus);  
          
          DEBUG_DATA("File Signature ucWimosID = %d",_stFileSignature.ucWimosID);   
          DEBUG_DATA("File Signature DeviceStatus = %d", _stFileSignature.stStatus.ucDeviceStatus);   
          DEBUG_DATA("File Signature DeviceStatus = %d", _stFileSignature.stStatus.usPortStatus);   
          
          /*The signature is not equal*/
          if(_stCurrentSignature.ucWimosID != _stFileSignature.ucWimosID ||
             _stCurrentSignature.stStatus.ucDeviceStatus != _stFileSignature.stStatus.ucDeviceStatus ||
              _stCurrentSignature.stStatus.usPortStatus !=  _stFileSignature.stStatus.usPortStatus){
            
            DEBUG_INFO("Signature is not equal, restoring Storage...");   
            
            /*Remove the current signature*/
            SD.remove(WIMOS_SIGN_FILE);
            
            /*Write the current signature*/
            stAuxFile = SD.open(WIMOS_SIGN_FILE,FILE_WRITE); 
            stAuxFile.write((uint8_t*) &_stCurrentSignature, sizeof(_stCurrentSignature));
            stAuxFile.close(); 
          
          }else{
            /*The current signature and the signature stored in SD match*/
            DEBUG_INFO("Signature match");  
          }
          
        }else{
          
          /*Clear the non volatile folder*/
          SD.rmdir(WIMOS_NON_VOLATILE_DIR);
          
          /*Create the non volatile folder*/
          SD.mkdir(WIMOS_NON_VOLATILE_DIR);
          
          /*The Current Signature Size and File Signature Size match.*/
          DEBUG_INFO("Signature size does NOT match,  restoring Storage...");          
          stAuxFile.close(); 
          
          /*Remove the current signature*/
          SD.remove(WIMOS_SIGN_FILE);
          
          /*Write the current signature*/
          stAuxFile = SD.open(WIMOS_SIGN_FILE,FILE_WRITE); 
          stAuxFile.write((uint8_t*) &_stCurrentSignature, sizeof(_stCurrentSignature));
          stAuxFile.close(); 
        }
        
          
        
      }else{
        
        /*The File Signature does not exist.*/
        DEBUG_INFO(WIMOS_SIGN_FILE" does NOT exist.");        
        
        /*Write the current signature*/
        stAuxFile = SD.open(WIMOS_SIGN_FILE,FILE_WRITE); 
        stAuxFile.write((uint8_t*) &_stCurrentSignature, sizeof( _stCurrentSignature ));
        stAuxFile.close(); 
        
        /*Clear the non volatile folder*/
        SD.rmdir(WIMOS_NON_VOLATILE_DIR);
        
        /*Create the non volatile folder*/
        SD.mkdir(WIMOS_NON_VOLATILE_DIR);
      
      }
        
      return _OK;
      
    }else{
      /*SD updater error*/
      DEBUG_INFO("SD updater no available.");   
      return _ERROR;
    }
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

extern int8_t restoreVolatileStatus(uint32_t* prtStatusMemory, uint16_t usBufferSize){
  #ifdef _EN_WIMOS_SD
    uint8_t i=0;
    #ifdef WIMOS_UNIT_TEST
      for(i=0; i<ucUnitTestSDInput; i++){      
    #else
      for(i=0; i<60; i++){
    #endif
    sprintf(ptrVolatileFileName, "%s/%s%d%s", WIMOS_VOLATILE_DIR, WIMOS_VOLATILE_FILE, i, WIMOS_VOLATILE_EXT);
      if(SD.exists(ptrVolatileFileName)){
        stVolatileFile = SD.open(ptrVolatileFileName, FILE_WRITE);
        prtStatusMemory[i] = stVolatileFile.size();
      }else{
        prtStatusMemory[i] = 0;
      }
    }
    return _OK;
  #endif
  return _ERROR;
}


extern int8_t storeVolatile(void* ptrBuffer, uint8_t ucBufferSize){
  #ifdef _EN_WIMOS_SD
    stVolatileFile.write((uint8_t*)ptrBuffer,ucBufferSize);
    DEBUG_DATA("File size = %d", stVolatileFile.size());
    return _OK;
  #endif
  return _ERROR;
}

extern int8_t nextVolatileFile(void){
  #ifdef _EN_WIMOS_SD
      
    #ifdef WIMOS_UNIT_TEST
      if(bUnitTestSDInput || (millis() - ulTimerSD) > 60000){
    #else
      if((millis() - ulTimerSD) > 60000){
    #endif
      
    #ifdef WIMOS_UNIT_TEST
      if(ucUnitTestSDInput > 0){
        usLastVolatileFile = (usLastVolatileFile + 1) % ucUnitTestSDInput;
      }else{        
        usLastVolatileFile = (usLastVolatileFile + 1) % 60;
      }
    #else
      usLastVolatileFile = (usLastVolatileFile + 1) % 60;
    #endif
      stVolatileFile.close();
      
      sprintf(ptrVolatileFileName, "%s/%s%d%s", WIMOS_VOLATILE_DIR, WIMOS_VOLATILE_FILE, usLastVolatileFile, WIMOS_VOLATILE_EXT);
      
      if( SD.exists(ptrVolatileFileName) ){
        SD.remove(ptrVolatileFileName);
      }
      
      stVolatileFile = SD.open(ptrVolatileFileName, FILE_WRITE);
      ulTimerSD = millis();
      
    }else{    
      stVolatileFile.close();
      stVolatileFile = SD.open(ptrVolatileFileName, FILE_WRITE);
    }
    return _OK;
  #endif
  return _ERROR;
}

extern int8_t moveFileToDirectory(const char* ptrDirNameFrom, const char* ptrFileNameFrom, const char* ptrDirNameTo, const char* ptrFileNameTo){
  #ifdef _EN_WIMOS_SD
    char ptrBuffer[WIMOS_MEM_BUFFER_SIZE];
    File stFrom, stTo;
    sprintf(ptrBuffer, "%s/%s", ptrDirNameFrom, ptrFileNameFrom);
    stFrom = SD.open(ptrBuffer, FILE_READ);
    DEBUG_DATA("FROM = %s",ptrBuffer);
    sprintf(ptrBuffer, "%s/%s", ptrDirNameTo, ptrFileNameTo);
    SD.remove(ptrBuffer);
    stTo = SD.open(ptrBuffer, FILE_WRITE);
    DEBUG_DATA("To = %s",ptrBuffer);
    
    
    DEBUG_DATA("Numbers of iterations = %d",(stFrom.size()/WIMOS_MEM_BUFFER_SIZE));
    for(uint32_t i = 0; i < (stFrom.size()/WIMOS_MEM_BUFFER_SIZE) ; i+=WIMOS_MEM_BUFFER_SIZE){
      stFrom.read(ptrBuffer,WIMOS_MEM_BUFFER_SIZE);
      stTo.write(ptrBuffer,WIMOS_MEM_BUFFER_SIZE);
    }
    DEBUG_DATA("Numbers of iterations = %d",(stFrom.size() % WIMOS_MEM_BUFFER_SIZE));
    if((stFrom.size() % WIMOS_MEM_BUFFER_SIZE) > 0){
      stFrom.read(ptrBuffer,stFrom.size() % WIMOS_MEM_BUFFER_SIZE);
      stTo.write(ptrBuffer,stFrom.size() % WIMOS_MEM_BUFFER_SIZE);
    }
    
    DEBUG_DATA("FileTo size = %d",stTo.size());
    DEBUG_DATA("FileFrom size = %d",stFrom.size());
    
    stTo.close();
    stFrom.close();
    
    return _OK;
  #endif
  return _ERROR;
}




#ifdef WIMOS_DEBUG
  #ifdef WIMOS_UNIT_TEST
    
    /**
     * @brief Wimos test n1.UT29.
     *
     * Unit test n1.UT29 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n1UT29 (void){
      const char* testName = "n1.UT29 = %d";
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
     * @brief Wimos test n1.UT30.
     *
     * Unit test n1.UT30 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n1UT30 (void){
      const char* testName = "n1.UT30 = %d";
      uint8_t i=0, j=0, h=0;
      bool bTest = true;
      
      /*Body_TEST:*/     
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
                   (bTest));
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
     * @brief Wimos test n1.UT31.
     *
     * Unit test n1.UT31 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n1UT31 (void){
      
      const char* testName = "n1.UT31 = %d";
      uint8_t i=0, j=0, h=0;
      bool bTest = true;
      
      /*Body_TEST:*/     
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
                   (bTest));
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
     * @brief Wimos test n1.UT32.
     *
     * Unit test n1.UT32 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n1UT32 (void){
      const char* testName = "n1.UT32 = %d";
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
     * @brief Wimos test n1.VT07.
     *
     * Unit test n1.VT07 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
    extern void _test_n1VT07 (void){
      const char* testName = "n1.VT07 = %ld microseconds";
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
                   (ucUnitTestSDOutput == 10000));
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


