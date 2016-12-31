
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

#ifdef __SAM3X8E__
  void waitCommand(void);
#endif
#ifdef __AVR_ATmega32U4__
  void sendCommand(void);
  uint8_t ucIndexCommand = 0;                              
  stCommandMessage prtMessageArray[] = _WIMOS_COMMAND_LIST;  
  
  const uint8_t ucMessageArraySize = sizeof(prtMessageArray) / sizeof(stCommandMessage);
#endif

stWimosACK stACK = { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameSize = ACK_SIZE_BYTE_CONST, .ucMessageFrom = 0x00, .ucMessageTo = 0x01, .ucACK = 0x00 };
stCommandMessage stCommand = (stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameSize = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = 0x00, .ucMessageTo = 0x01, .ucCommand = 0x00, .ucChecksum = 0x00 };
uint8_t ucLastChecksum = 0;
uint8_t ucMessageTries = 0x00;

void sendACK(void);
void waitACK(void);
void sendResponse(void);
void clearBuffer(void);
void runFunction(void);
bool receiveFrame( void* pData, uint8_t ucSize, uint8_t ucFrameSize);
void sendFrame(const void* ptrData, uint8_t ucSize);
extern void (*communicationThread)(void);

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
    #ifdef __SAM3X8E__
      communicationThread = waitCommand;
      stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus |= WIMOS_DEVICE_RF_MASK;
    #endif
    #ifdef __AVR_ATmega32U4__
      communicationThread = sendCommand;
      SERIAL_USB.begin(BAUDRATE_USB);
    #endif    
      SERIAL_RF.begin(BAUDRATE_RF);
      DEBUG_OK("Communication RF initialized.");
  #else
    communicationThread = noOperation;
    stGlobalWimosInfoMsg.stInfo.stStatus.ucDeviceStatus &= ~WIMOS_DEVICE_RF_MASK;
    DEBUG_INFO("Communication RF NOT initialized.");
  #endif
}

uint8_t getChecksum(void* ptrDataInput, uint8_t ucDataInputSize){
  uint8_t ucRetValue = 0;
  if(ucDataInputSize > 0){
    ucRetValue = ((uint8_t*)ptrDataInput)[0];
    uint8_t i;
    for(i = 1; i < ucDataInputSize; i++){
      ucRetValue ^= ((uint8_t*)ptrDataInput)[i];
    }
  }
  return ucRetValue;
}


#ifdef __SAM3X8E__
/**
 * @brief Waiting a Correct Command State.
 *
 * This is a state of communication state machine, it waits until a correct command.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
void waitCommand(void){
  ucMessageTries = 0x00;     
  if( receiveFrame(&stCommand, sizeof(stCommand), COMMAND_SIZE_BYTE_CONST) ){   
    /** Process RF input **/
    if(stCommand.ucMessageTo == WIMOS_ID || stCommand.ucMessageTo == 0xFF ){
      if(stCommand.ucCommand == COMMAND_GET_STATION_ID || stCommand.ucCommand == COMMAND_GET_STATUS_ALERT ){
        communicationThread = runFunction;
        return;
      }else{
        communicationThread = sendACK;
        return;          
      }
    }else{
        DEBUG_INFO("Command for another ID.");
    }  
  }
}
#endif

#ifdef __AVR_ATmega32U4__
  /**
   * @brief Waiting a Correct Command State.
   *
   * This is a state of communication state machine, it waits until a correct command.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  void sendCommand(void){    
    ucMessageTries = 0x00;
    DEBUG_INFO("Sending the Command.");
    memcpy(&stCommand,&prtMessageArray[ucIndexCommand],sizeof(stCommand));
    //stCommand.ucChecksum = getChecksum(&stCommand, (sizeof(stCommand)-1));
    ucLastChecksum = stCommand.ucChecksum;
    SERIAL_USB.println("Command sent.");
    sendFrame(&stCommand, sizeof(stCommand));
    ucIndexCommand = (ucIndexCommand + 1) % ucMessageArraySize;
             
    if(stCommand.ucCommand == COMMAND_GET_STATION_ID || stCommand.ucCommand == COMMAND_GET_STATUS_ALERT ){
      communicationThread = runFunction;
      return;
    }else{
      communicationThread = waitACK;
      return;          
    }
  }
#endif

uint8_t getByteRF(void){
  uint8_t ucReadValue = SERIAL_RF.read();
  char ucDebugRF[8];
  sprintf(ucDebugRF, "%d ",ucReadValue);
  DEBUG_OK(ucDebugRF);
  return ucReadValue;
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
  static bool bACKReceived = true;
  static uint32_t ulTimeoutACK = 0;
  
  if(bACKReceived){
    DEBUG_OK("Into");
    /*Init the ACK received value.*/
    bACKReceived =  false;
    /*Init the timer for timeout.*/
    ulTimeoutACK = millis();    
  }else{
    /*If the timeour was excedded.*/
    if(millis() - ulTimeoutACK > (TIMEOUT_ACK)){
      /*Go to the previous State.*/
      #ifdef __AVR_ATmega32U4__
        communicationThread = sendCommand;
      #endif
      #ifdef __SAM3X8E__
        communicationThread = waitCommand;
      #endif 
      /*Restart ACK received status.*/
      bACKReceived =  true;
      /*Restart ACK timeout value.*/
      ulTimeoutACK = 0;  
    }else{
      if( receiveFrame(&stACK, sizeof(stACK), ACK_SIZE_BYTE_CONST) ){
        /*If the message is for me, and from the message is a response for command.*/
    #ifdef __SAM3X8E__
        if(stACK.ucMessageTo == WIMOS_ID && stACK.ucMessageFrom == stCommand.ucMessageFrom)
    #endif
    #ifdef __AVR_ATmega32U4__
        if(stACK.ucMessageTo == WIMOS_ID && stACK.ucMessageFrom == stCommand.ucMessageTo)
    #endif
        {
          /*If the ACK matchs with the last Checksum sent.*/
          if(true){         
            /*Restart the Received state.*/
            bACKReceived =  true;
            /*Restart the timeout counter.*/
            ulTimeoutACK = 0;
            Serial.println("ACK received.");
            /*Go to the next state.*/
            #ifdef __AVR_ATmega32U4__
              communicationThread = runFunction;
            #endif
            #ifdef __SAM3X8E__
              communicationThread = waitCommand;
            #endif 
            
            stACK.ucMessageTo = stACK.ucMessageFrom;
            /*Update the ACK response.*/
            stACK.ucMessageFrom = WIMOS_ID;
            //stACK.ucACK = ~stACK.ucACK;
            /*Send the ACK response.*/
            sendFrame(&stACK,sizeof(stACK));
            return;
          /*The ACK received does not match.*/
          }else{
            DEBUG_INFO("The ACK checksum not matched.");
            /*Go to the last state.*/
            #ifdef __AVR_ATmega32U4__
              communicationThread = sendCommand;
            #endif
            #ifdef __SAM3X8E__
              communicationThread = runFunction;
            #endif 
            /*Restart the status*/
            bACKReceived =  true;
            ulTimeoutACK = 0;
            return;
          }
        }else{
          DEBUG_INFO("The message is not for me.");
        }  
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
  stWimosACK stACK = {.ucBegin = 0xFF, .ucFrameSize = 0x03};
  static bool bACKSent = false;
  static uint32_t ulTimeoutACK = 0;
  static uint8_t ucACKTries = 0;
  
  if(bACKSent == false){
    
    /*Build the message.*/
    stACK.ucBegin = 0xFF;
    stACK.ucFrameSize= 0x03;
    
    #ifdef __SAM3X8E__
      stACK.ucMessageTo = stCommand.ucMessageFrom;
    #endif
    #ifdef __AVR_ATmega32U4__
      stACK.ucMessageTo = stCommand.ucMessageTo;    
    #endif 
    
    stACK.ucMessageFrom = WIMOS_ID;
    stACK.ucACK = ~(stCommand.ucChecksum);
    /*Send the message*/
    DEBUG_INFO("The ACK sent.");
    Serial.println("ACK sent");
    sendFrame(&stACK, sizeof(stACK));
    /*Set the current time.*/
    ulTimeoutACK = millis();
    /*Set the ACK as sent.*/
    bACKSent = true;
    delay(1000);
    return;
  }
  DEBUG_INFO("The ACK was sent.");
  if( millis() - ulTimeoutACK > TIMEOUT_ACK ){
    /*Reset the timeout counter.*/   
    ulTimeoutACK = 0;
    /*Set the state to not sent.*/
    bACKSent = false;
    
    #ifdef __SAM3X8E__
      communicationThread = waitCommand;
    #endif
    #ifdef __AVR_ATmega32U4__
      communicationThread = sendCommand;      
    #endif 
    
    return;
  }else{
    
    if( receiveFrame(&stACK, sizeof(stACK), ACK_SIZE_BYTE_CONST) ){ 
      /*If the ACK message is for me and from the Command.*/
  #ifdef __SAM3X8E__
      if(stACK.ucMessageTo == WIMOS_ID && stACK.ucMessageFrom == stCommand.ucMessageFrom)
  #endif
  #ifdef __AVR_ATmega32U4__
      if(stACK.ucMessageTo == WIMOS_ID && stACK.ucMessageFrom == stCommand.ucMessageTo)
  #endif
      {
        DEBUG_DATA("The ACK response is for me = %d.", stACK.ucMessageTo); 
        DEBUG_DATA("The ACK response is from = %d.", stACK.ucMessageFrom);    
        /*The checksum matchs.*/              
        if(true){     
          /*Reset the ACK to not sent.*/     
          bACKSent = false;
          /*Reset the number of tries.*/
          ucACKTries = 0;
          /*Reset the timeout counter.*/
          ulTimeoutACK = 0;
          /*Go to next state.*/      
          Serial.println("ACK-OK received");     
          DEBUG_OK("ACK-OK is received");        
          #ifdef __AVR_ATmega32U4__
            communicationThread = sendCommand;
          #endif
          #ifdef __SAM3X8E__
            communicationThread = runFunction;
          #endif 
          return;
        }else{
          DEBUG_INFO("The ACK response checksum not match."); 
          /*Reset the ACK sent state.*/    
          bACKSent = false;
          /*Reset the ACK tries.*/
          ucACKTries = 0;
          /*Reset the ACK timeout counter.*/
          ulTimeoutACK = 0;
          return;
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
  uint8_t ucValueRF = 0x00;
  static uint32_t ulTimeout = 0x00;
  static bool bFirstExec = true;
  
  #ifdef __SAM3X8E__
  
    if(bFirstExec == true){
      DEBUG_OK("Run function execution.");
      bFirstExec = false;
    }else{
      switch(stCommand.ucCommand){
        case COMMAND_GET_STATION_ID:
          DEBUG_INFO("GET STATION ID COMMAND received.");
          ucValueRF = WIMOS_ID;
          sendFrame(&ucValueRF,1);
          communicationThread = waitCommand;
        break;
        case COMMAND_GET_GENERAL_INFO:
          DEBUG_OK("GET GENERAL INFO COMMAND received.");
          stGlobalWimosInfoMsg.ucChecksum = 0x00; /*TODO: Checksum.*/
          stGlobalWimosInfoMsg.ucBegin = 0xFF; /*TODO: Checksum.*/
          stGlobalWimosInfoMsg.ucFrameSize = 0x05; /*TODO: Checksum.*/
          stGlobalWimosInfoMsg.ucMessageTo = 0x00; /*TODO: Checksum.*/
          stGlobalWimosInfoMsg.ucMessageFrom = 0x10; /*TODO: Checksum.*/
          sendFrame(&stGlobalWimosInfoMsg,sizeof(stGlobalWimosInfoMsg));
          communicationThread = waitACK;
        break;
        case COMMAND_GET_QUEUE_ALERT:
          DEBUG_INFO("GET QUEUE ALERT COMMAND received.");
          /*TODO: make a Alert Queue.*/
          communicationThread = waitACK;
        
        break;
        case COMMAND_GET_STATUS_ALERT:
          DEBUG_INFO("GET STATUS ALERT COMMAND received.");
          /*TODO: check the alert Queue.*/
          ucValueRF = WIMOS_ID;
          sendFrame(&ucValueRF,1);
          communicationThread = waitCommand;
        break;
        case COMMAND_GET_SENSOR_INFO:
          DEBUG_INFO("GET SENSOR INFO COMMAND received.");
          stGlobalWimosPortMsg.ucChecksum = 0x00; /*TODO: Checksum.*/
          sendFrame(&stGlobalWimosPortMsg,sizeof(stGlobalWimosPortMsg));
          communicationThread = waitACK;      
        break;
        default:
          DEBUG_ERROR("Unknow Command received.");
          communicationThread = waitCommand;
        break; 
      }
    }
  #endif
  #ifdef __AVR_ATmega32U4__
    if(bFirstExec == true){
      SERIAL_USB.print("Run Command = ");
      SERIAL_USB.println(stCommand.ucCommand, HEX);
      ulTimeout = millis();
      bFirstExec = false;
    }
    if(millis() - ulTimeout >= (TIMEOUT_ACK)){
      SERIAL_USB.println("Run timeout.");
      communicationThread = sendCommand;
      ulTimeout = 0;
      bFirstExec = true;
      return;
    }
    switch(stCommand.ucCommand){
      case COMMAND_GET_STATION_ID:
        delay(1875);
        SERIAL_USB.print("FRAME512(");
        SERIAL_USB.print(SERIAL_RF.available());
        SERIAL_USB.print("): ");
        while(SERIAL_RF.available()){
          SERIAL_USB.print(SERIAL_RF.read());
          SERIAL_USB.print(" ");
        }
        SERIAL_USB.println("");
        communicationThread = sendCommand;
      break;
      case COMMAND_GET_GENERAL_INFO:
        if( receiveFrame(&stGlobalWimosInfoMsg, sizeof(stGlobalWimosInfoMsg),  5 ) ){ 
          /*If the ACK message is for me and from the Command.*/
        SERIAL_USB.print(stGlobalWimosInfoMsg.ucMessageTo);
        SERIAL_USB.print(" ");
        SERIAL_USB.println(WIMOS_ID);
        SERIAL_USB.print(stGlobalWimosInfoMsg.ucMessageFrom);
        SERIAL_USB.print(" ");
        SERIAL_USB.println(stCommand.ucMessageTo);
          if(stGlobalWimosInfoMsg.ucMessageTo == WIMOS_ID && stGlobalWimosInfoMsg.ucMessageFrom == stCommand.ucMessageTo){
            /*The checksum matchs.*/              
            if(true){ /*TODO: make Checksum.*/       
              SERIAL_USB.println("FRAME515:");
              communicationThread = sendACK;
              bFirstExec = true;
              return;
            }else{
              return;
            }
          }
        }
      break;
      case COMMAND_GET_QUEUE_ALERT:
        /*TODO: make a Alert Queue.*/
        communicationThread = waitACK;
      
      break;
      case COMMAND_GET_STATUS_ALERT:
        /*TODO: check the alert Queue.*/
        ucValueRF = WIMOS_ID;
        sendFrame(&ucValueRF,1);
        communicationThread = sendCommand;
      break;
      case COMMAND_GET_SENSOR_INFO:
        stGlobalWimosPortMsg.ucChecksum = 0x00; /*TODO: Checksum.*/
        sendFrame(&stGlobalWimosPortMsg,sizeof(stGlobalWimosPortMsg));
        communicationThread = waitACK;      
      break;
      default:
        DEBUG_ERROR("Unknow Command received.");
        communicationThread = sendCommand;
      break; 
    }
  #endif
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
bool receiveFrame( void* pData, uint8_t ucSize, uint8_t ucFrameSize){
  uint8_t i = 0x00;
  
  #ifdef __AVR_ATmega32U4__
    delay(5);
  #endif
  
  #ifdef WIMOS_UNIT_TEST
    if(true)
  #else
    if(SERIAL_RF.available() >= ucSize)
  #endif
  {
    do{
      ((uint8_t*)pData)[0] = getByteRF();
    }while(SERIAL_RF.available() >= ucSize && ((uint8_t*)pData)[0] != COMMAND_BEGIN_BYTE_CONST);
    
    if(((uint8_t*)pData)[0] == COMMAND_BEGIN_BYTE_CONST){
      //SERIAL_USB.println("Token received.");
      ((uint8_t*)pData)[1] = getByteRF();
      if(((uint8_t*)pData)[1] == ucFrameSize){
      //SERIAL_USB.println("Size matchs.");
        for(i=2; i < ucSize; i++){
          ((uint8_t*)pData)[i] = getByteRF();
        }
        for(i=0; i < ucSize; i++){
          Serial.print(((uint8_t*)pData)[i]);
          Serial.print(" ");
        }
        Serial.println();
        return true;
      }else{
        //SERIAL_USB.println("Size does not match.");
        //SERIAL_USB.println(((uint8_t*)pData)[1] );
        //SERIAL_USB.println(ucFrameSize);
        return false;
      }
    }else{
      return false; 
    }
  }else{
    return false;
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
      DEBUG_DATA("Send Byte = %d.", ((uint8_t*)pData)[i]);
      #ifdef WIMOS_UNIT_TEST
        ucUnitTestOutput[i] = ((uint8_t*)pData)[i];
      #else
        SERIAL_RF.write(((uint8_t*)pData)[i]);
        delayMicroseconds(100);
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




#ifdef __SAM3X8E__
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
#endif
