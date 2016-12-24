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
 * @file wimos_communication.ino
 * @author Andres Sanchez Anillo
 * @date 09 Jun 2015
 * @brief File containing main function for Wimos Framework.
 *
 * Wimos is a Framework for easy IoT development.
 * @see https://github.com/andressanchezanillo/WIMOS_
 */

#include "_setting.h"
#include "main_config.h"


void waitCommand(void);
void sendACK(void);
void waitACK(void);
void sendResponse(void);
void clearBuffer(void);
void runFunction(void);
void sendFrame(const void* ptrData, uint8_t ucSize);

extern void (*communicationThread)(void);
static uint8_t lastCMD = NONE;

uint8_t ucBufferRF[20] = {0};
stCommandMessage stCommand;
uint8_t ucLastChecksum = 0;

#ifdef WIMOS_UNIT_TEST
  uint8_t ucUnitTestOutput[70];
#endif
/**
 * @brief Communication initialization.
 *
 * This function initializes the communication interface.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
extern void initRF(void){
  #ifdef _EN_WIMOS_RF
    /*communicationThread = waitCommand;*/
    SERIAL_RF.begin(BAUDRATE_RF);
    stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus |= WIMOS_DEVICE_RF_MASK;
    DEBUG_OK("Communication RF initialized.");
  #else
    /*communicationThread = noOperation;*/
    stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus &= ~WIMOS_DEVICE_RF_MASK;
    DEBUG_INFO("Communication RF NOT initialized.");
  #endif
}

#define COMMAND true
#define IS_FOR_ME true
#define BUFFER_FULL true
#define QUERY_ID true
#define TIMEOUT true
bool ACK_SENT = true;
uint8_t ACK_ATTEMPTS = 0;
bool ACK_RECEIVED = true;

/**
 * @brief Waiting a Correct Command State.
 *
 * This is a state of communication state machine, it waits until a correct command.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
void waitCommand(void){
  uint8_t ucValueRF = 0x00;
  uint8_t i=0;
  DEBUG_INFO("Waiting Command over RF.");
  
  while(SERIAL_RF.available() >= sizeof(stCommand) && ucValueRF != 0xFF){
    ucValueRF = SERIAL_RF.read();
  }
  if(ucValueRF == 0xFF){
    DEBUG_INFO("Command token 0xFF found.");
    ucBufferRF[0] = 0xFF;
    for(i=1; i<sizeof(stCommand); i++)
      ucBufferRF[i] = SERIAL_RF.read();
    if(ucBufferRF[1] == 0x03){
      DEBUG_INFO("Command size match.");
      memcpy(&stCommand,ucBufferRF,sizeof(stCommand));
      
      /** Process RF input **/
      if(stCommand.ucMessageTo == WIMOS_ID || stCommand.ucMessageTo == 0xFF ){
        DEBUG_DATA("Command for me = %d.",stCommand.ucMessageTo);
        if(stCommand.ucCommand != 0x05){
          DEBUG_INFO("Command with ACK found");
          communicationThread = sendACK;
          return;
        }else{
          DEBUG_INFO("Command without ACK found");
          communicationThread = runFunction;
          return;          
        }
      }else{
          DEBUG_INFO("Command for another ID.");
      }  
    }else{
      DEBUG_INFO("Command size not match");  
    }
  }else{
    DEBUG_INFO("Command token not found");
  }  
}

/**
 * @brief Waiting ACK State.
 *
 * This is a state of communication state machine, it waits until ACK message received.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
void waitACK(void){
  uint8_t i = 0;
  uint8_t ucValueRF = 0;  
  stWimosACK stACK;
  
  static bool bACKReceived = true;
  static uint32_t ulTimeoutACK = 0;
  
  DEBUG_INFO("Waiting ACK.");

  if(bACKReceived){
    DEBUG_INFO("The last ACK was received.");
    DEBUG_INFO("Waiting a new ACK.");
    bACKReceived =  false;
    ulTimeoutACK = millis();    
  }else{
    DEBUG_INFO("The last ACK was not received.");
    if(millis() - ulTimeoutACK > TIMEOUT_ACK){
        DEBUG_INFO("The timeout was excedded.");
        communicationThread = runFunction; 
        bACKReceived =  true;
        ulTimeoutACK = 0;  
    }else{
      while(SERIAL_RF.available() >= sizeof(stACK) && ucValueRF != 0xFF){
        ucValueRF = SERIAL_RF.read();
      }
      if(ucValueRF == 0xFF){
        DEBUG_INFO("The ACK token was found.");
        ucBufferRF[0] = 0xFF;
        for(i=1; i<sizeof(stWimosACK); i++)
          ucBufferRF[i] = SERIAL_RF.read();
        if(ucBufferRF[1] == 0x03){
          DEBUG_INFO("The ACK size match.");
          memcpy(&stACK,ucBufferRF,sizeof(stACK));
          
          /** Process RF input **/
          if(stACK.ucMessageTo == WIMOS_ID && stACK.ucMessageFrom == stCommand.ucMessageFrom){
            DEBUG_DATA("The ACK is for me = %d.",stACK.ucMessageTo);
            DEBUG_DATA("The ACK is from = %d.",stACK.ucMessageFrom);
            if(stACK.ucACK == (~ucLastChecksum)){
              DEBUG_INFO("The ACK checksum matched.");
              bACKReceived =  true;
              ulTimeoutACK = 0;
              communicationThread = waitCommand; 
              return;
            }else{
              DEBUG_INFO("The ACK checksum not matched.");
              communicationThread = runFunction; 
              bACKReceived =  true;
              ulTimeoutACK = 0;
              return;
            }
          }else{
            DEBUG_INFO("The message is not for me.");
          }  
        }else{
          DEBUG_INFO("The ACK checksum not matched.");
        }
      }else{
        DEBUG_INFO("The ACK token not found.");
      } 
    }
  }
}

/**
 * @brief Sending ACK State.
 *
 * This is a state of communication state machine, it sends a ACK messsage.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
void sendACK(void){
  uint8_t i = 0;
  uint8_t ucValueRF = 0;  
  stWimosACK stACK;
  
  static bool bACKSent = false;
  static uint32_t ulTimeoutACK = 0;
  static uint8_t ucACKTries = 0;
  DEBUG_INFO("Sending the ACK.");
  
  if(bACKSent == false){
    DEBUG_INFO("The ACK was not sent.");
    
    stACK.ucMessageFrom = WIMOS_ID;
    stACK.ucMessageTo = stCommand.ucMessageFrom;
    stACK.ucACK = ~(stCommand.ucChecksum);

    sendFrame(&stACK, sizeof(stACK));
    ulTimeoutACK = millis();
    ucACKTries++;
    bACKSent = true;
    return;
  }else{
    DEBUG_INFO("The ACK was sent.");
    if(ucACKTries >= 3){
      DEBUG_INFO("The number of tries was excedded.");        
      communicationThread = waitCommand; 
      return;
    }else{
      if( millis() - ulTimeoutACK > TIMEOUT_ACK ){
        DEBUG_INFO("The timeout was excedded.");     
        ulTimeoutACK = 0;
        bACKSent = false;
        return;
      }else{
        while(SERIAL_RF.available() >= sizeof(stACK) && ucValueRF != 0xFF){
          ucValueRF = SERIAL_RF.read();
        }
        if(ucValueRF == 0xFF){
          DEBUG_INFO("The ACK response token was found.");     
          ucBufferRF[0] = 0xFF;
          for(i=1; i<sizeof(stWimosACK); i++)
            ucBufferRF[i] = SERIAL_RF.read();
          if(ucBufferRF[1] == 0x03){
            DEBUG_INFO("The ACK response size match.");     
            memcpy(&stACK,ucBufferRF,sizeof(stACK));
            
            /** Process RF input **/
            if(stACK.ucMessageTo == WIMOS_ID && stACK.ucMessageFrom == stCommand.ucMessageFrom){
              DEBUG_DATA("The ACK response is for me = %d.", stACK.ucMessageTo); 
              DEBUG_DATA("The ACK response is from = %d.", stACK.ucMessageFrom);                   
              if(stACK.ucACK == stCommand.ucChecksum){
                DEBUG_INFO("The ACK response checksum match.");     
                bACKSent = false;
                ucACKTries = 0;
                ulTimeoutACK = 0;
                communicationThread = runFunction; 
                return;
              }else{
                DEBUG_INFO("The ACK response checksum not match.");     
                bACKSent = false;
                ucACKTries = 0;
                ulTimeoutACK = 0;
                return;
              }
            }  
          }
        }
      }
    }
  }
}

/**
 * @brief Waiting a Correct Command State.
 *
 * This is a state of communication state machine, it waits until a correct command.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
void runFunction(void){
  switch(stCommand.ucCommand){
    case 0x01:
      sendFrame("Hola",5);
    break;
    default:
    
    break; 
  }
}

/**
 * @brief Send Frame function.
 *
 * This function sends the frame byte to byte.
 * @verbatim like this@endverbatim 
 * @param pData Pointer to data for sending.
 * @param ucSize Data length for sending.
 * @return none.
 */
void sendFrame(const void* pData, uint8_t ucSize){
  #ifdef _EN_WIMOS_RF
    /** Send the frame over RF **/
    uint8_t i = 0;
    for(i=0; i<ucSize; i++){
      #ifdef WIMOS_UNIT_TEST
        ucUnitTestOutput[i] = ((uint8_t*)pData)[i];
      #else
        SERIAL_RF.write(((uint8_t*)pData)[i]);
      #endif    
    }
  #endif
  
  return;
}

/**
 * @brief Example showing how to document a function with Doxygen.
 *
 * Description of what the function does. This part may refer to the parameters
 * of the function, like @p param1 or @p param2.
 * @verbatim like this@endverbatim 
 * @param param1 Description of the first parameter of the function.
 * @param param2 The second one, which follows @p param1.
 * @return Describe what the function returns.
 * @see http://website/
 */
extern void noOperation(void){
  return;
}




#ifdef WIMOS_DEBUG
  #ifdef WIMOS_UNIT_TEST
    
    /**
     * @brief Wimos test n3.UT10.
     *
     * Unit test n3.UT07 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n3UT07 (void){
      const char* testName = "n3.UT07 = %d";
      /*Body_TEST:*/
      
      #ifdef _EN_WIMOS_RF
        stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus = 0;
        initRF();
        DEBUG_VALID(testName , 
                   (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_RF_MASK), 
                   (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_RF_MASK) == WIMOS_DEVICE_RF_MASK);
      #else
        stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus = WIMOS_DEVICE_RF_MASK;
        initRF();
        DEBUG_VALID(testName , 
                   (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_RF_MASK), 
                   (stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus & WIMOS_DEVICE_RF_MASK) == 0x00);
      #endif
      
      /*End_Body_TEST:*/
    } 

    /**
     * @brief Wimos test n3.UT08.
     *
     * Unit test n3.UT08 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n3UT08 (void){
      const char* testName = "n3.UT08 = %d";
      /*Body_TEST:*/     
      uint8_t i = 0;
      for(i = 0; i<70; i++)
        ucUnitTestOutput[i] = 0;
        
      stGlobalWimosPortMsg.ucMessageFrom = 1;
      stGlobalWimosPortMsg.ucMessageTo = 2;
      stGlobalWimosPortMsg.checksum = 3;
      
      #ifdef _EN_WIMOS_RF
        
        sendFrame(&stGlobalWimosPortMsg, sizeof(stGlobalWimosPortMsg));
        
        stWimosPortValuesMessage* stUTest = (stWimosPortValuesMessage*) ucUnitTestOutput ;
        
        DEBUG_VALID(testName , 
                   (memcmp(&stGlobalWimosPortMsg, stUTest, sizeof(stGlobalWimosPortMsg))), 
                   (memcmp(&stGlobalWimosPortMsg, stUTest, sizeof(stGlobalWimosPortMsg)) == 0));
      #else      
        
        sendFrame(&stGlobalWimosPortMsg, sizeof(stGlobalWimosPortMsg));
        
        stWimosPortValuesMessage* stUTest = (stWimosPortValuesMessage*) ucUnitTestOutput ;
        
        DEBUG_VALID(testName , 
                   (stUTest->ucBegin), 
                   (stUTest->ucBegin == 0));
      #endif
      /*End_Body_TEST:*/
    } 


    /**
     * @brief Wimos test n3.UT09.
     *
     * Unit test n3.UT09 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
     extern void _test_n3UT09 (void){
      const char* testName = "n3.UT09 = %d";
      /*Body_TEST:*/     
      /*TODO: make a reader testing.*/
      /*End_Body_TEST:*/
    } 
  #endif
  
  #ifdef WIMOS_VALIDATION_TEST
  
    
    /**
     * @brief Wimos test n3.VT04.
     *
     * Unit test n3.VT04 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
    extern void _test_n3VT04 (void){
      const char* testName = "n3.VT04 = %ld microseconds";
      
      /*Body_TEST:*/     
      initRF();
      uint32_t ulTimerVT = micros();
      sendFrame(&stGlobalWimosPortMsg, sizeof(stGlobalWimosPortMsg));
      DEBUG_VALID(testName , 
                 (micros() - ulTimerVT), 
                 (micros() - ulTimerVT < 100));
      /*End_Body_TEST:*/
    } 
    
  #endif
#endif
