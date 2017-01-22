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

#ifdef __AVR_ATmega32U4__
  #include <SoftwareSerial.h>
  
  SoftwareSerial SerialRF(8,9);
  uint8_t ucIndexCommand = 0;                              
  stCommandMessage prtMessageArray[] = _WIMOS_COMMAND_LIST;    
  const uint8_t ucMessageArraySize = sizeof(prtMessageArray) / sizeof(stCommandMessage);
  
  void sendCommand(void);
#endif

#ifdef __SAM3X8E__
  void waitCommand(void);
#endif

stWimosACK stACK = { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameSize = ACK_SIZE_BYTE_CONST, .ucMessageFrom = 0x00, .ucMessageTo = 0x01, .ucACK = 0x00 };
stCommandMessage stCommand = (stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameSize = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = 0x00, .ucMessageTo = 0x01, .ucCommand = 0x00, .ucChecksum = 0x00 };
uint8_t ucLastChecksum = 0;

#define BUFFER_RF_SIZE  50
uint8_t ucBufferRF[50] = {0x00};
uint8_t ucBufferRFIndex = 0x00;


uint8_t getChecksum(void* ptrDataInput, uint8_t ucDataInputSize);
void clearBufferRF(void);
void updateBufferRF(void);
void sendACK(void);
void waitACK(void);
void sendResponse(void);
void clearBuffer(void);
void runFunction(void);
bool receiveFrame( void* pData, uint8_t ucSize, uint8_t ucFrameSize);
void sendFrame(const void* ptrData, uint8_t ucSize);
extern void (*communicationThread)(void);


#ifdef __AVR_ATmega32U4__
  #ifdef WIMOS_VALIDATION_TEST_CENTER
    uint32_t ucCommandsSent = 0;
    uint32_t ucCommandsResponseReceived = 0;
    uint32_t ulTimeRate = 0;
  #endif
#endif

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
  if( receiveFrame(&stCommand, sizeof(stCommand), COMMAND_SIZE_BYTE_CONST) ){   
    
    /** Process RF input **/
    if( stCommand.ucMessageTo == WIMOS_ID || stCommand.ucMessageTo == 0xFF ){
      
      /*The checksum matchs.*/   
      if( stCommand.ucChecksum == getChecksum(&stCommand, sizeof(stCommand)-1) ){
        
        if( stCommand.ucCommand == COMMAND_GET_STATION_ID || stCommand.ucCommand == COMMAND_GET_STATUS_ALERT ){
          ucLastChecksum = 0x00;
          communicationThread = runFunction;
        }else{
          ucLastChecksum = (uint8_t)(stCommand.ucChecksum << 1) & 0xFF;
          communicationThread = sendACK;
        }
        stCommand.ucChecksum = 0x03;
      }else{
        
        /*Build the ACK-NOK message.*/
        stACK.ucBegin = 0xFF;
        stACK.ucFrameSize = 0x03;
        stACK.ucMessageTo = stACK.ucMessageFrom;
        stACK.ucMessageFrom = WIMOS_ID;
        stACK.ucACK = 0x03;
        
        /*Clear RF Buffer.*/         
        clearBufferRF();
          
        /*Send ACK-NOK.*/
        sendFrame(&stACK,sizeof(stACK));
      
        DEBUG_ERROR("Checksum Error.");
      }
    }else{
        DEBUG_INFO("Command for another ID.");
    }  
  }
  return;
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
    
    /*Build the command message.*/
    stCommand = prtMessageArray[ucIndexCommand];
    stCommand.ucChecksum = getChecksum(&stCommand, (sizeof(stCommand)-1));

    /*Clear RF buffer.*/
    clearBufferRF();
    
    /*Send command*/
    sendFrame(&stCommand, sizeof(stCommand));

    /*Validation for Center.*/
    #ifdef __AVR_ATmega32U4__
      #ifdef WIMOS_VALIDATION_TEST_CENTER
        if(ucCommandsSent == 0){
          ucCommandsResponseReceived = 0;
          ulTimeRate = millis();
        }
        SERIAL_USB.print("Data Rate = ");
        SERIAL_USB.print(((float)((float)ucCommandsResponseReceived / (float)ucCommandsSent))*100);
        SERIAL_USB.println("%.");
        SERIAL_USB.print("Time Rate = ");
        SERIAL_USB.print(((float)((float)((millis()-ulTimeRate)/1000) / (float)ucCommandsResponseReceived)));
        SERIAL_USB.println("s/command.");
        
        if(ucCommandsResponseReceived < 100)
          ucCommandsSent++;
      #endif
    #endif
    
    /*Calculate the next command.*/
    ucIndexCommand = (ucIndexCommand + 1) % ucMessageArraySize;

    /*If the command is an ID request.*/
    if(stCommand.ucCommand == COMMAND_GET_STATION_ID || stCommand.ucCommand == COMMAND_GET_STATUS_ALERT ){
      
      /*Set not-checksum (they have not ACK confirmation).*/
      ucLastChecksum = 0x00;
      
      /*Go to run function state.*/
      communicationThread = runFunction;
      
      return;
    
    /*If the code is not an ID request.*/
    }else{
      
      /*Calculate the ACK value.*/
      ucLastChecksum = (uint8_t)(stCommand.ucChecksum << 1) & 0xFF;
      
      /**/
      stCommand.ucChecksum = 0x03;
      
      /*Go to wait ACK state.*/
      communicationThread = waitACK;
      
      return;          
    }
  }
#endif


/**
 * @brief Waiting ACK State.
 *
 * This is a state of communication state machine, it waits until ACK message received.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
void waitACK(void){
  static bool bACKReceived = false;
  static uint32_t ulTimeoutACK = 0;

  if(bACKReceived == false){
    
    /*Clear RF buffer.*/
    //clearBufferRF();
    
    /*Init the ACK received value.*/
    bACKReceived =  true;
    
    /*Init the timer for timeout.*/
    ulTimeoutACK = millis();
  
  }
  
  {
    /*If the timeour was excedded.*/
    if(millis() - ulTimeoutACK > (TIMEOUT_ACK)){
      
      /*Restart ACK received status.*/
      bACKReceived =  false;
      DEBUG_OK("Timeout.");
      /*Restart ACK timeout value.*/
      ulTimeoutACK = 0;  
      
      /*Build the ACK-NOK message.*/
      stACK.ucBegin = 0xFF;
      stACK.ucFrameSize = 0x03;
      
      #ifdef __AVR_ATmega32U4__
        stACK.ucMessageTo = stCommand.ucMessageTo;
      #endif
      #ifdef __SAM3X8E__
        stACK.ucMessageTo = stCommand.ucMessageFrom;
      #endif 
      stACK.ucMessageFrom = WIMOS_ID;
      stACK.ucACK = 0x03;

      Serial.print(__FUNCTION__);
      Serial.print(":");
      Serial.println("timeout");

      /*Clear Buffer RF.*/
      clearBufferRF();
      
      /*Send ACK-NOK.*/
      sendFrame(&stACK,sizeof(stACK));
      
      /*Go to the previous State.*/
      #ifdef __AVR_ATmega32U4__
        communicationThread = sendCommand;
      #endif
      #ifdef __SAM3X8E__
        communicationThread = waitCommand;
      #endif 
      return;
    /*The timeout was not excedded.*/
    }else{
  
      /*If we have received an ACK message.*/
      if( receiveFrame(&stACK, sizeof(stACK), ACK_SIZE_BYTE_CONST) ){
        #ifdef __SAM3X8E__
          /*If the message is for me, and from the ID command received from.*/
          if(stACK.ucMessageTo == WIMOS_ID && stACK.ucMessageFrom == stCommand.ucMessageFrom)
        #endif
        #ifdef __AVR_ATmega32U4__
          
          /*If the message is for me, and from the ID command sent to.*/
          if(stACK.ucMessageTo == WIMOS_ID && stACK.ucMessageFrom == stCommand.ucMessageTo)
        #endif
        {
          DEBUG_OK("ACK for my ID.");
                   
          /*Go to not received state.*/
          bACKReceived =  false;
          
          /*Reset the timeout.*/
          ulTimeoutACK = 0x00;
          
          char ucDebugRF[12];
//          #ifdef __SAM3X8E__
//          sprintf(ucDebugRF, "%d - %d", stACK.ucACK, ucLastChecksum);
//          Serial.println(ucDebugRF);
//          #endif
          /*ACK-OK*/          
          if( stACK.ucACK == ucLastChecksum){
            DEBUG_OK("ACK checksum matched.");
            
            /*Build the ACK-OK message.*/
            stACK.ucBegin = 0xFF;
            stACK.ucFrameSize = 0x03;
            stACK.ucMessageTo = stACK.ucMessageFrom;
            stACK.ucMessageFrom = WIMOS_ID;
            stACK.ucACK = (uint8_t)( stACK.ucACK << 1) & 0xFF;
//            #ifdef __SAM3X8E__
//            sprintf(ucDebugRF, "%d ", stACK.ucACK);
//            Serial.println(ucDebugRF);
//            #endif
            /*Go to the next state.*/
            #ifdef __AVR_ATmega32U4__
              communicationThread = runFunction;
            #endif
            #ifdef __SAM3X8E__
              communicationThread = waitCommand;
            #endif 
            
          /*ACK-NOK*/
          }else{
            DEBUG_OK("ACK checksum not matched.");
            
            Serial.print(__FUNCTION__);
            Serial.print(":");
            Serial.println("NO-ACK");
            /*Build the ACK-NOK message.*/
            stACK.ucBegin = 0xFF;
            stACK.ucFrameSize = 0x03;
            stACK.ucMessageTo = stACK.ucMessageFrom;
            stACK.ucMessageFrom = WIMOS_ID;
            stACK.ucACK = 0x03;
            
            /*Restart state machine.*/
            #ifdef __AVR_ATmega32U4__
              communicationThread = sendCommand;
            #endif
            #ifdef __SAM3X8E__
              communicationThread = waitCommand;
            #endif 
            
          }
          
          /*Clear RF Buffer.*/         
          clearBufferRF();
          
          /*Send the ACK(OK or NOK) message.*/
          sendFrame(&stACK,sizeof(stACK));
          
          stACK.ucACK = 0x03;
          
          return;
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
  static bool bACKSent = false;
  static uint32_t ulTimeoutACK = 0;

  /*ACK was sent.*/
  if(bACKSent == false){
    
    /*Build the message.*/
    stACK.ucBegin = 0xFF;
    stACK.ucFrameSize= 0x03;
    stACK.ucMessageFrom = WIMOS_ID;
    
    #ifdef __SAM3X8E__
      stACK.ucMessageTo = stCommand.ucMessageFrom;
    #endif
    #ifdef __AVR_ATmega32U4__
      stACK.ucMessageTo = stCommand.ucMessageTo;    
    #endif 
    
    stACK.ucACK = ucLastChecksum;
    
    /*Clear RF Buffer.*/         
    clearBufferRF();
    
    /*Send the message*/
    sendFrame(&stACK, sizeof(stACK));

    /*Calculate the last Checksum.*/
    ucLastChecksum = ((uint8_t)stACK.ucACK << 1) & 0xFF;
    
    /*Set the current time.*/
    ulTimeoutACK = millis();
    
    /*Set the ACK as sent.*/
    bACKSent = true;
    return;
  
  }

  {
    
    /*The ACK was sent.*/
    /*The timeout was excedded.*/
    if( millis() - ulTimeoutACK >= TIMEOUT_ACK ){
      
      /*Reset the timeout counter.*/   
      ulTimeoutACK = 0;
      
      /*Set the state to not sent.*/
      bACKSent = false;
  
      /*Go to first state.*/
      #ifdef __SAM3X8E__
        communicationThread = waitCommand;
      #endif
      #ifdef __AVR_ATmega32U4__
//        Serial.println("sendACK: Timeout.");
        communicationThread = sendCommand;      
      #endif 
      
      Serial.print(__FUNCTION__);
      Serial.print(":");
      Serial.println("timeout");
      
      return;
      
    /*The timeout was not excedded.*/
    }else{
  
      /*It has received a command.*/
      if( receiveFrame(&stACK, sizeof(stACK), ACK_SIZE_BYTE_CONST) ){ 
      
      /*If the ACK message is for me and from the Command.*/
    #ifdef __SAM3X8E__
        if(stACK.ucMessageTo == WIMOS_ID && stACK.ucMessageFrom == stCommand.ucMessageFrom)
    #endif
    #ifdef __AVR_ATmega32U4__
        if(stACK.ucMessageTo == WIMOS_ID && stACK.ucMessageFrom == stCommand.ucMessageTo)
    #endif
        {
                    
          /*Reset the ACK to not sent.*/     
          bACKSent = false;
          
          /*Reset the timeout counter.*/
          ulTimeoutACK = 0;   
          
          /*Clear RF Buffer.*/         
          clearBufferRF();
          
          #ifdef __AVR_ATmega32U4__
//            Serial.print(stACK.ucACK);
//            Serial.print(" .-. ");
//            Serial.println(ucLastChecksum);
          #endif 
          /*The checksum matchs.*/              
          if(stACK.ucACK == ucLastChecksum ){
          
            #ifdef __AVR_ATmega32U4__
                    
              /*Validation for Center.*/
              #ifdef WIMOS_VALIDATION_TEST_CENTER
                if(ucCommandsResponseReceived < 100)
                  ucCommandsResponseReceived++;
              #endif
              
              /*Go to next state.*/
              communicationThread = sendCommand;
              
            #endif
            #ifdef __SAM3X8E__
              communicationThread = runFunction;
            #endif 
            
            return;
          
          /*The checksum does not match.*/
          }else{
            
            /*Reset the ACK sent state.*/    
            bACKSent = false;
            
            /*Reset the ACK timeout counter.*/
            ulTimeoutACK = 0;
            
            /*Go to the first state.*/
            #ifdef __AVR_ATmega32U4__
              communicationThread = sendCommand;
            #endif
            #ifdef __SAM3X8E__
              communicationThread = waitCommand;
            #endif 
            
            Serial.print(__FUNCTION__);
            Serial.print(":");
            Serial.println("NO-ACK");
            
            return;
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
  uint8_t ucValueRF = 0x00;
  static uint32_t ulTimeoutACK = 0x00;
  static bool bFirstExec = true;

  #ifdef __SAM3X8E__
    
    /*It is the first time of execution.*/
    if(bFirstExec == true){

      /*Clear first time variable.*/
      bFirstExec = false;

      /*Clear the Buffer RF.*/
      clearBufferRF();

    /*It is not the first time of execution.*/
    }
    
    {
      
      /*It is not the first execution.*/
      /*Process the command.*/
      switch(stCommand.ucCommand){
  
        /*The command is a ID request.*/
        case COMMAND_GET_STATION_ID:
          
          /*Set the ID.*/
          ucValueRF = WIMOS_ID;
  
          /*Send the ID.*/
          sendFrame(&ucValueRF,1);
  
          /*Go to the next state.*/
          communicationThread = waitCommand;
          
          /*Set the ACK (ACK is not needed for this command).*/
          stACK.ucACK = 0x00;
        break;
  
        /*The command is a general info request.*/
        case COMMAND_GET_GENERAL_INFO:
          
          /*Build a general info message.*/
          memset(&stGlobalWimosInfoMsg, 0x00, sizeof(stGlobalWimosInfoMsg));
          stGlobalWimosInfoMsg.ucBegin = 0xFF;
          stGlobalWimosInfoMsg.ucFrameSize = 0x05;
          stGlobalWimosInfoMsg.ucMessageTo = 0x00;
          stGlobalWimosInfoMsg.ucMessageFrom = 0x10; 
          stGlobalWimosInfoMsg.ucChecksum  = getChecksum(&stGlobalWimosInfoMsg, (sizeof(stGlobalWimosInfoMsg)-1));
  
          /*Calculate the checksum.*/
          ucLastChecksum = ((uint8_t)stGlobalWimosInfoMsg.ucChecksum << 1) & 0xFF ;  
  
          /*Clear buffer RF.*/
          clearBufferRF();
  
          /*Send the general info response.*/
          sendFrame(&stGlobalWimosInfoMsg,sizeof(stGlobalWimosInfoMsg));
  
          /*Go to next state.*/
          communicationThread = waitACK;
        
        break;
  
        /*The command is an alert request.*/
        case COMMAND_GET_QUEUE_ALERT:
          /*TODO: make a Alert Queue.*/
          communicationThread = waitACK;
          stACK.ucACK = 0x00;
        
        break;
        
        /*The command is an ID alert request.*/
        case COMMAND_GET_STATUS_ALERT:
          DEBUG_INFO("GET STATUS ALERT COMMAND received.");
          /*TODO: check the alert Queue.*/
          ucValueRF = WIMOS_ID;
          sendFrame(&ucValueRF,1);
          communicationThread = waitCommand;
          stACK.ucACK = 0x00;
          
        break;
  
        /*The command is a sensor info request.*/
        case COMMAND_GET_SENSOR_INFO:
          DEBUG_INFO("GET SENSOR INFO COMMAND received."); 
          /*TODO: Checksum.*/
          stGlobalWimosPortMsg.ucChecksum = 0x00;
          sendFrame(&stGlobalWimosPortMsg,sizeof(stGlobalWimosPortMsg));
          communicationThread = waitACK;    
          stACK.ucACK = 0x00;  
        break;
  
        /*The command received is unknown.*/
        default:
          DEBUG_ERROR("Unknown Command received.");
          communicationThread = waitCommand;
          stACK.ucACK = 0x00;
        break; 
      }
    }
  #endif

  #ifdef __AVR_ATmega32U4__

    /*If the first time execution.*/
    if(bFirstExec == true){
      
      /*Set the timeout timer.*/
      ulTimeoutACK = millis();
      
      /*Clear the first execution.*/
      bFirstExec = false;
    }
    
    {
    
      /*It is not the first execution.*/
      /*If the timeout was excedded.*/
      if(millis() - ulTimeoutACK >= (TIMEOUT_ACK)){
        
        /*Clear the ACK value.*/
        ulTimeoutACK = 0;
        /*Clear the first execution.*/
        bFirstExec = true;
        
        /*Build the ACK-NOK message.*/
        stACK.ucBegin = 0xFF;
        stACK.ucFrameSize= 0x03;      
        stACK.ucMessageFrom = WIMOS_ID;
        stACK.ucMessageTo = stCommand.ucMessageTo;
        stACK.ucACK = 0x01;
  
        /*Clear the Buffer RF.*/
        clearBufferRF();
  
        Serial.print(__FUNCTION__);
        Serial.print(":");
        Serial.println("timeout");
        /*Send the ACK-NOK message.*/
        sendFrame(&stACK, sizeof(stACK));     
        
        /*Go to first state.*/
        communicationThread = sendCommand;
        
        return;
      
      }else{
        
        /*Process the command.*/
        switch(stCommand.ucCommand){
          
          /*The command is a ID station request.*/
          case COMMAND_GET_STATION_ID:
          
            /*Wait a minimal delay.*/
            delay(1875);
            
            /*Print all IDs received.*/
            while(SERIAL_RF.available()){
              SERIAL_USB.print(SERIAL_RF.read());
              SERIAL_USB.print(" ");
            }
            SERIAL_USB.println("");
            
            /*Go to first State.*/
            communicationThread = sendCommand;
            
          break;
          
          /*The command is a General Info request.*/
          case COMMAND_GET_GENERAL_INFO:
            
            /*If it receives a General info message.*/
            if( receiveFrame(&stGlobalWimosInfoMsg, sizeof(stGlobalWimosInfoMsg),  5 ) ){ 
              
              /*If the ACK message is for me and from the Command.*/
              if(stGlobalWimosInfoMsg.ucMessageTo == WIMOS_ID && stGlobalWimosInfoMsg.ucMessageFrom == stCommand.ucMessageTo){
                
                /*Clear buffer RF.*/
                clearBufferRF();
                
                /* The checksum matched.*/
                if( stGlobalWimosInfoMsg.ucChecksum  == getChecksum(&stGlobalWimosInfoMsg, (sizeof(stGlobalWimosInfoMsg)-1))){    
  
                  /*Calculate the ACK.*/
                  ucLastChecksum = ((uint8_t)stGlobalWimosInfoMsg.ucChecksum << 1) & 0xFF ;  
                  
                  /*Frame to USB.*/
                  SERIAL_USB.println("FRAME515:");
                  
                  /*Go to next state.*/
                  communicationThread = sendACK;
                  
                  /*Clear first execution value.*/
                  bFirstExec = true;
                  
                  return;
                /*The checksum not matched.*/
                }else{
                                  
                  /*Build the message.*/
                  stACK.ucBegin = 0xFF;
                  stACK.ucFrameSize= 0x03;                
                  stACK.ucMessageFrom = WIMOS_ID;
                  stACK.ucMessageTo = stCommand.ucMessageTo;
                  stACK.ucACK = 0x01;
  
                  /*Clear Buffer RF.*/                
                  clearBufferRF();
  
                  /*Send ACK-NOK.*/
                  sendFrame(&stACK, sizeof(stACK));              
  
                  /*Go to the first state.*/
                  communicationThread = sendCommand;          
                  
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
      }
    }
  #endif
}







uint8_t getChecksum(void* ptrDataInput, uint8_t ucDataInputSize){
  uint8_t ucRetValue = 0;
  //Serial.println("Checksum Begin:");
  if(ucDataInputSize > 0){
    ucRetValue = ((uint8_t*)ptrDataInput)[0];
    //Serial.print(((uint8_t*)ptrDataInput)[0]);
    //Serial.print(" ");
    uint8_t i;
    for(i = 1; i < ucDataInputSize; i++){
      ucRetValue ^= ((uint8_t*)ptrDataInput)[i];
      //Serial.print(((uint8_t*)ptrDataInput)[i]);
      //Serial.print(" ");
    }
    //Serial.println(ucRetValue);
  }
  //Serial.println("Checksum End:");
  return ucRetValue;
}


void clearBufferRF(void){
  uint16_t usCurrentBufferStatus = 0x00;  
  do{
    usCurrentBufferStatus = SERIAL_RF.available();
    delay(5);
  }while(usCurrentBufferStatus != SERIAL_RF.available());
  
  for(uint8_t i = 0; i < BUFFER_RF_SIZE; i++){
    ucBufferRF[ucBufferRFIndex] = 0;
  }
  ucBufferRFIndex = 0;
  return;  
}


extern bool updateBufferRF(uint8_t ucSize){
  static bool bFound = false;
  uint8_t ucValueRF = 0x00;
  uint16_t usCurrentBufferStatus = 0x00;
  
  if(bFound == false){
    
    /*Wait the data entering.*/
    do{
      usCurrentBufferStatus = SERIAL_RF.available();
      delay(5);
    }while(usCurrentBufferStatus != SERIAL_RF.available());

    /*Read RF buffer until buffer empty or receive the 0xFF token.*/
    while(SERIAL_RF.available() &&  ucValueRF != 0xFF ){
      ucValueRF = SERIAL_RF.read();
    }
    
    /*The token was not received.*/
    if(ucValueRF != 0xFF){
      bFound = false;
      
    /*The token was received.*/
    }else{
      bFound = true;      
    }    
  }
  
  /*If the token was received.*/
  if(bFound == true){    
    /*If Serial buffer available equal than ucSize.*/
    /* (ucSize - 1) =  Data without token.*/
    if( SERIAL_RF.available() >= (ucSize - 1) ){
      /* Set as not found, for the next iteration. */
      bFound = false;
      /* Returns found.*/
      return true;
      
    /*If Serial buffer available lower than ucSize.*/
    }else{
      /*Set as found.*/
      bFound = true;
      /*Returns not found.*/
      return false;        
    }    
  }else{
    /*Returns not found.*/
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

bool receiveFrame( void* pData, uint8_t ucSize, uint8_t ucFrameSize){
  uint8_t i=0;
  bool bFound = false;
  
  bFound = updateBufferRF(ucSize);
  
  if(bFound){
    
    ((uint8_t*)pData)[0] =  0xFF;
    ((uint8_t*)pData)[1] =   SERIAL_RF.read();
    
    if( ((uint8_t*)pData)[1] != ucFrameSize ){
      return false;
    }else{
      for(i=2; i < ucSize; i++){
        ((uint8_t*)pData)[i] =  SERIAL_RF.read();
      }
    }
  }
  return bFound;
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
    char ucDebug[10];
    
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
