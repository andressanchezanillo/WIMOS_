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
  const uint8_t ucMessageArraySize = _WIMOS_COMMAND_LIST_SIZE;
  
  void sendCommand(void);
#endif

#ifdef __SAM3X8E__
  void waitCommand(void);
#endif

stWimosACK stACK = { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = ACK_SIZE_BYTE_CONST, .ucMessageFrom = 0x00, .ucMessageTo = 0x01, .ucACK = 0x00 };
stCommandMessage stCommand = (stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = 0x00, .ucMessageTo = 0x01, .ucCommand = 0x00, .ucChecksum = 0x00 };
uint8_t ucLastChecksum = 0;

#define BUFFER_RF_SIZE  50
uint8_t ucBufferRF[50] = {0x00};
uint8_t ucBufferRFIndex = 0x00;

uint8_t getChecksum(void* ptrDataInput, uint8_t ucDataInputSize);
void clearBufferRF(void);
bool updateBufferRF(uint8_t);
void sendACK(void);
void waitACK(void);
void sendResponse(void);
void clearBuffer(void);
void runFunction(void);
void noOperation(void);
bool receiveFrame( void* pData, uint8_t ucSize, uint8_t ucFrameID);
void sendFrame(const void* ptrData, uint8_t ucSize);
extern void (*communicationThread)(void);


#ifdef __AVR_ATmega32U4__
    uint32_t ucCommandsSent = 0;
    uint32_t ucCommandsResponseReceived = 0;
    uint32_t ulTimeRate = 0;
    uint32_t ulTimeCommandsRate = 0;
#endif

#ifdef WIMOS_UNIT_TEST
  uint8_t ucUnitTestOutput[70];
  uint8_t ucUnitTestInput[70];
  uint8_t ucUnitTestInputIndex;
  uint8_t ucUnitTestInputSize;
#endif

#ifdef WIMOS_VALIDATION_TEST
  bool bValidationOnGoing = true;
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
      ulTimeCommandsRate = millis();
      SERIAL_USB.begin(BAUDRATE_USB);
    #endif    
      SERIAL_RF.begin(BAUDRATE_RF);
      clearBufferRF();
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

    static bool bFirstTime = true;

    if( receiveFrame(&stCommand, sizeof(stCommand), COMMAND_SIZE_BYTE_CONST) ){   
    
    /** Process RF input **/
    if( stCommand.ucMessageTo == WIMOS_ID || stCommand.ucMessageTo == 0xFF ){
      
      /*The checksum matchs.*/   
      if( stCommand.ucChecksum == getChecksum(&stCommand, sizeof(stCommand)-1) ){
          
          #ifdef DEBUG_COMM_STATUS
            SERIAL_DEBUG.print(__FUNCTION__);
            SERIAL_DEBUG.println(": Go to SendACK.");
          #endif
        
          ucLastChecksum = (uint8_t)(stCommand.ucChecksum << 1) & 0xFF;
          communicationThread = sendACK;
          bFirstTime = true;
          
      }else{
        
        #ifdef DEBUG_COMM_STATUS
          SERIAL_DEBUG.print(__FUNCTION__);
          SERIAL_DEBUG.println(": Checksum Error");
        #endif
        
        /*Build the ACK-NOK message.*/
        stACK.ucBegin = 0xFF;
        stACK.ucFrameID = 0x03;
        stACK.ucMessageTo = stACK.ucMessageFrom;
        stACK.ucMessageFrom = WIMOS_ID;
        stACK.ucACK = 0x03;
          
        /*Send ACK-NOK.*/
        sendFrame(&stACK,sizeof(stACK));
      
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
    stCommand = prtMessageArray[ucIndexCommand];
    stCommand.ucChecksum = getChecksum(&stCommand, (sizeof(stCommand)-1));
    
    /*Send command*/
    sendFrame(&stCommand, sizeof(stCommand));
    /*Calculate the Commands sent*/
    ucCommandsSent++;
    
    /*Success Command Rate.*/
    if(millis() - ulTimeCommandsRate >= TIME_COMMAND_RATE){
      #ifndef WIMOS_VALIDATION_TEST
        char ucDataOutput[4];
        SERIAL_USB.print("[INFO001]:");
        
        SERIAL_USB.print(" ");
        sprintf(ucDataOutput,"%02x",(uint8_t)((float)((float)ucCommandsResponseReceived / (float)ucCommandsSent)) * 100);
        SERIAL_USB.print(ucDataOutput);
        
        SERIAL_USB.print(" ");
        sprintf(ucDataOutput,"%02x",((uint32_t)((float)((float) TIME_COMMAND_RATE / (float)ucCommandsResponseReceived))));
        SERIAL_USB.print(ucDataOutput);
        
        SERIAL_USB.println(" OK");      
        ulTimeCommandsRate = millis();
        ucCommandsResponseReceived = 0;
        ucCommandsSent = 0;
      #else
        bValidationOnGoing = false;
      #endif
    }
    
    /*Calculate the next command.*/
    ucIndexCommand = (ucIndexCommand + 1) % ucMessageArraySize;

    /*Calculate the ACK value.*/
    ucLastChecksum = (uint8_t)(stCommand.ucChecksum << 1) & 0xFF;
    
    /*Go to wait ACK state.*/
    communicationThread = waitACK;
    
    #ifdef DEBUG_COMM_STATUS
      SERIAL_DEBUG.print(__FUNCTION__);
      SERIAL_DEBUG.println(": Goto WaitACK");
    #endif
    
    return;    
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
      /*Restart ACK timeout value.*/
      ulTimeoutACK = 0;  
      
      /*Build the ACK-NOK message.*/
      stACK.ucBegin = 0xFF;
      stACK.ucFrameID = 0x03;
      
      #ifdef __AVR_ATmega32U4__
        stACK.ucMessageTo = stCommand.ucMessageTo;
      #endif
      #ifdef __SAM3X8E__
        stACK.ucMessageTo = stCommand.ucMessageFrom;
      #endif 
      stACK.ucMessageFrom = WIMOS_ID;
      stACK.ucACK = 0x03;
      
      /*Send ACK-NOK.*/
      sendFrame(&stACK,sizeof(stACK));
      
      #ifdef DEBUG_COMM_STATUS
        SERIAL_DEBUG.print(__FUNCTION__);
        SERIAL_DEBUG.println(": Timeout expired.");
      #endif
      
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
          
          /*ACK-OK*/          
          if( stACK.ucACK == ucLastChecksum){
            DEBUG_OK("ACK checksum matched.");
            
            /*Build the ACK-OK message.*/
            stACK.ucBegin = 0xFF;
            stACK.ucFrameID = 0x03;
            stACK.ucMessageTo = stACK.ucMessageFrom;
            stACK.ucMessageFrom = WIMOS_ID;
            stACK.ucACK = (uint8_t)(( ucLastChecksum << 1) & 0xFF);
            
            /*Go to the next state.*/
            #ifdef __AVR_ATmega32U4__
            
              #ifdef DEBUG_COMM_STATUS
                SERIAL_DEBUG.print(__FUNCTION__);
                SERIAL_DEBUG.println(": Goto RunFunction.");
              #endif
              communicationThread = runFunction;
            
            #endif
            
            #ifdef __SAM3X8E__
            
              #ifdef DEBUG_COMM_STATUS
                SERIAL_DEBUG.print(__FUNCTION__);
                SERIAL_DEBUG.println(": Goto WaitCommand.");
              #endif
              communicationThread = waitCommand;
            #endif 
            
          /*ACK-NOK*/
          }else{
            
            #ifdef DEBUG_COMM_STATUS
              SERIAL_DEBUG.print(__FUNCTION__);
              SERIAL_DEBUG.println(": Checksum Error.");
            #endif
            
            /*Build the ACK-NOK message.*/
            stACK.ucBegin = 0xFF;
            stACK.ucFrameID = 0x03;
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
          
          /*Send the ACK(OK or NOK) message.*/
          sendFrame(&stACK,sizeof(stACK));
          
          return;
          
        }else{
          
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
    stACK.ucFrameID= 0x03;
    stACK.ucMessageFrom = WIMOS_ID;
    
    #ifdef __SAM3X8E__
      stACK.ucMessageTo = stCommand.ucMessageFrom;
    #endif
    #ifdef __AVR_ATmega32U4__
      stACK.ucMessageTo = stCommand.ucMessageTo;    
    #endif 
    stACK.ucACK = ucLastChecksum;
    
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
      
      /*Restart ACK timeout value.*/
      ulTimeoutACK = 0;  
      
      /*Build the ACK-NOK message.*/
      stACK.ucBegin = 0xFF;
      stACK.ucFrameID = 0x03;
      
      #ifdef __AVR_ATmega32U4__
        stACK.ucMessageTo = stCommand.ucMessageTo;
      #endif
      #ifdef __SAM3X8E__
        stACK.ucMessageTo = stCommand.ucMessageFrom;
      #endif 
      stACK.ucMessageFrom = WIMOS_ID;
      stACK.ucACK = 0x03;
      
      /*Send ACK-NOK.*/
      sendFrame(&stACK,sizeof(stACK));
            
      #ifdef DEBUG_COMM_STATUS
        SERIAL_DEBUG.print(__FUNCTION__);
        SERIAL_DEBUG.println(": Timeout Expired.");
      #endif
      
      /*Go to first state.*/
      #ifdef __SAM3X8E__
        communicationThread = waitCommand;
      #endif
      #ifdef __AVR_ATmega32U4__
          SERIAL_USB.println(" ACK-NOK");
          communicationThread = sendCommand;      
      #endif 
            
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
          
          /*The checksum matchs.*/             
          if(stACK.ucACK == ucLastChecksum ){
            #ifdef __AVR_ATmega32U4__
              
              #ifdef DEBUG_COMM_STATUS
                SERIAL_DEBUG.print(__FUNCTION__);
                SERIAL_DEBUG.println(": Goto SendCommand.");
              #endif
              
              /*Success command rate.*/
              ucCommandsResponseReceived++;

              /*Print ACK-OK over USB.*/
              SERIAL_USB.println(" ACK-OK");
              
              /*Go to next state.*/
              communicationThread = sendCommand;
              
            #endif
            
            #ifdef __SAM3X8E__
            
            
              #ifdef DEBUG_COMM_STATUS
                SERIAL_DEBUG.print(__FUNCTION__);
                SERIAL_DEBUG.println(": Goto RunFunction.");
              #endif
              communicationThread = runFunction;
            #endif 
            
            return;
          
          /*The checksum does not match.*/
          }else{
            
            /*Reset the ACK sent state.*/    
            bACKSent = false;
            
            /*Reset the ACK timeout counter.*/
            ulTimeoutACK = 0;
            
            /*Build the ACK-NOK message.*/
            stACK.ucBegin = 0xFF;
            stACK.ucFrameID = 0x03;
            
            #ifdef __AVR_ATmega32U4__
              stACK.ucMessageTo = stCommand.ucMessageTo;
            #endif
            #ifdef __SAM3X8E__
              stACK.ucMessageTo = stCommand.ucMessageFrom;
            #endif 
            stACK.ucMessageFrom = WIMOS_ID;
            stACK.ucACK = 0x03;
            
            /*Send ACK-NOK.*/
            sendFrame(&stACK,sizeof(stACK));
            
            #ifdef DEBUG_COMM_STATUS
              SERIAL_DEBUG.print(__FUNCTION__);
              SERIAL_DEBUG.println(": Checksum Error.");
            #endif
         
            /*Go to the first state.*/
            #ifdef __AVR_ATmega32U4__
              SERIAL_USB.println(" ACK-NOK");
              communicationThread = sendCommand;
            #endif
            #ifdef __SAM3X8E__
              communicationThread = waitCommand;
            #endif 
            
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

    /*It is not the first time of execution.*/
    }
    
    {
      
      /*It is not the first execution.*/
      /*Process the command.*/
      switch(stCommand.ucCommand){
  
        /*The command is a general info request.*/
        case COMMAND_GET_GENERAL_INFO:
          
          /*Build a general info message.*/
          stGlobalWimosInfoMsg.ucBegin = 0xFF;
          stGlobalWimosInfoMsg.ucFrameID = 0x05;
          stGlobalWimosInfoMsg.ucMessageTo = 0x00;
          stGlobalWimosInfoMsg.ucMessageFrom = 0x10; 
          stGlobalWimosInfoMsg.ucChecksum  = getChecksum(&stGlobalWimosInfoMsg, (sizeof(stGlobalWimosInfoMsg)-1));
  
          /*Calculate the checksum.*/
          ucLastChecksum = ((uint8_t)stGlobalWimosInfoMsg.ucChecksum << 1) & 0xFF ;  
  
          /*Send the general info response.*/
          sendFrame(&stGlobalWimosInfoMsg,sizeof(stGlobalWimosInfoMsg));
  
          /*Go to next state.*/
          communicationThread = waitACK;
        
        break;
  
        /*The command is an alert request.*/
        case COMMAND_GET_ALERT_LIST:
          
          /*Build a general info message.*/
          stGlobalWimosAlertMsg.ucBegin = 0xFF;
          stGlobalWimosAlertMsg.ucFrameID = 0x06;
          stGlobalWimosAlertMsg.ucMessageTo = 0x00;
          stGlobalWimosAlertMsg.ucMessageFrom = 0x10; 
          stGlobalWimosAlertMsg.ucChecksum  = getChecksum(&stGlobalWimosAlertMsg, (sizeof(stGlobalWimosAlertMsg)-1));
  
          /*Calculate the checksum.*/
          ucLastChecksum = ((uint8_t)stGlobalWimosAlertMsg.ucChecksum << 1) & 0xFF ;  
  
          /*Send the general info response.*/
          sendFrame(&stGlobalWimosAlertMsg,sizeof(stGlobalWimosAlertMsg));
  
          /*Go to next state.*/
          communicationThread = waitACK;
        
        break;
        /*DON'T ERASE THIS COMMENT.*/
        /*INPUT:#0x0021#*/
        /*The command received is unknown.*/
        default:
          DEBUG_ERROR("Unknown Command received.");
          communicationThread = waitCommand;
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
        stACK.ucFrameID= 0x03;      
        stACK.ucMessageFrom = WIMOS_ID;
        stACK.ucMessageTo = stCommand.ucMessageTo;
        stACK.ucACK = 0x01;
        
        /*Send the ACK-NOK message.*/
        sendFrame(&stACK, sizeof(stACK));     
        
        /*Go to first state.*/
        #ifdef DEBUG_COMM_STATUS
          SERIAL_DEBUG.print(__FUNCTION__);
          SERIAL_DEBUG.println(": Timeout Error");
        #endif
        communicationThread = sendCommand;
        
        return;
      
      }else{
        
        /*Process the command.*/
        switch(stCommand.ucCommand){
          stWimosInfoMessage stWimosTestMsg;
          /*The command is a General Info request.*/
          case COMMAND_GET_GENERAL_INFO:
            /*If it receives a General info message.*/
            if( receiveFrame(&stWimosTestMsg, sizeof(stWimosTestMsg),  5 ) ){ 
              /*If the ACK message is for me and from the Command.*/
              if(stWimosTestMsg.ucMessageTo == WIMOS_ID && stWimosTestMsg.ucMessageFrom == stCommand.ucMessageTo){                
                /* The checksum matched.*/
                if( stWimosTestMsg.ucChecksum  == getChecksum(&stWimosTestMsg, (sizeof(stWimosTestMsg)-1))){ 
                
                  /*Calculate the ACK.*/
                  ucLastChecksum = ((uint8_t)stWimosTestMsg.ucChecksum << 1) & 0xFF ;  
                  
                  /*Frame to USB.*/
                  SERIAL_USB.print("FRAME");
                  SERIAL_USB.print(stCommand.ucCommand);
                  SERIAL_USB.print(":");
                      
                  for(uint8_t i=0; i< sizeof(stWimosTestMsg); i++ ){
                    char ucDataOutput[4];
                    SERIAL_USB.print(" ");
                    sprintf(ucDataOutput,"%02x",((uint8_t*)&stWimosTestMsg)[i]);
                    SERIAL_USB.print(ucDataOutput);
                  }
                  
                  /*Go to next state.*/
                  communicationThread = sendACK;
                  
                  /*Clear first execution value.*/
                  bFirstExec = true;
                  
                  return;
                /*The checksum not matched.*/
                }else{
                                  
                  /*Build the ACK error message.*/
                  stACK.ucBegin = 0xFF;
                  stACK.ucFrameID= 0x03;                
                  stACK.ucMessageFrom = WIMOS_ID;
                  stACK.ucMessageTo = stCommand.ucMessageTo;
                  stACK.ucACK = 0x01;
  
                  /*Send ACK-NOK.*/
                  sendFrame(&stACK, sizeof(stACK));
                  #ifdef DEBUG_COMM_STATUS
                    SERIAL_DEBUG.print(__FUNCTION__);
                    SERIAL_DEBUG.println(": Checksum Error");
                  #endif             
  
                  /*Go to the first state.*/
                  communicationThread = sendCommand;          
                  
                  return;
                }
              }else{
                #ifdef DEBUG_COMM_STATUS
                  SERIAL_DEBUG.print(__FUNCTION__);
                  SERIAL_DEBUG.println(": Address Error");
                #endif
              }
            }
          break;
          
          case COMMAND_GET_ALERT_LIST: /*If it receives a General info message.*/
            stAlertMessage stLocalWimosAlertMsg;
            
            if( receiveFrame(&stLocalWimosAlertMsg, sizeof(stAlertMessage),  6 ) ){ 
              /*If the ACK message is for me and from the Command.*/
              if(stLocalWimosAlertMsg.ucMessageTo == WIMOS_ID && stLocalWimosAlertMsg.ucMessageFrom == stCommand.ucMessageTo){                
                /* The checksum matched.*/
                if( stLocalWimosAlertMsg.ucChecksum  == getChecksum(&stLocalWimosAlertMsg, (sizeof(stLocalWimosAlertMsg)-1))){ 
                
                  /*Calculate the ACK.*/
                  ucLastChecksum = ((uint8_t)stLocalWimosAlertMsg.ucChecksum << 1) & 0xFF ;  
                  
                  /*Frame to USB.*/
                  SERIAL_USB.print("FRAME");
                  SERIAL_USB.print(stCommand.ucCommand);
                  SERIAL_USB.print(":");
                      
                  for(uint8_t i=0; i< sizeof(stLocalWimosAlertMsg); i++ ){
                    char ucDataOutput[4];
                    SERIAL_USB.print(" ");
                    sprintf(ucDataOutput,"%02x",((uint8_t*)&stLocalWimosAlertMsg)[i]);
                    SERIAL_USB.print(ucDataOutput);
                  }
                  
                  /*Go to next state.*/
                  communicationThread = sendACK;
                  
                  /*Clear first execution value.*/
                  bFirstExec = true;
                  
                  return;
                /*The checksum not matched.*/
                }else{
                                  
                  /*Build the ACK error message.*/
                  stACK.ucBegin = 0xFF;
                  stACK.ucFrameID= 0x03;                
                  stACK.ucMessageFrom = WIMOS_ID;
                  stACK.ucMessageTo = stCommand.ucMessageTo;
                  stACK.ucACK = 0x01;
  
                  /*Send ACK-NOK.*/
                  sendFrame(&stACK, sizeof(stACK));
                  #ifdef DEBUG_COMM_STATUS
                    SERIAL_DEBUG.print(__FUNCTION__);
                    SERIAL_DEBUG.println(": Checksum Error");
                  #endif             
  
                  /*Go to the first state.*/
                  communicationThread = sendCommand;          
                  
                  return;
                }
              }else{
                #ifdef DEBUG_COMM_STATUS
                  SERIAL_DEBUG.print(__FUNCTION__);
                  SERIAL_DEBUG.println(": Address Error");
                #endif
              }
            }          
          break;
          /*DON'T ERASE THIS COMMENT.*/
          /*INPUT:#0x0020#*/
          default:
            DEBUG_ERROR("Unknow Command received.");
            communicationThread = sendCommand;
          break; 
        }
      }
    }
  #endif
}

void noOperation(void){
  
}





/**
 * @brief Clear Frame buffer function.
 *
 * This function clears the buffer RF.
 * @verbatim like this@endverbatim 
 * @param <None>
 * @return none.
 */
uint8_t getChecksum(void* ptrDataInput, uint8_t ucDataInputSize){
  return 10;
  uint8_t ucRetValue = 0;
  if(ucDataInputSize > 0){
    ucRetValue = ((uint8_t*)ptrDataInput)[0];
    uint8_t i;
    for(i = 1; i < ucDataInputSize; i++){
      /*XOR operation for each byte.*/    
      ucRetValue ^= ((uint8_t*)ptrDataInput)[i];
    }
  }
  return ucRetValue;
}




/**
 * @brief Clear Frame buffer function.
 *
 * This function clears the buffer RF.
 * @verbatim like this@endverbatim 
 * @param <None>
 * @return none.
 */
void clearBufferRF(void){
  uint16_t usCurrentBufferStatus = 0x00;  
  updateBufferRF(0);
  
  #ifndef WIMOS_UNIT_TEST
    while(SERIAL_RF.available()){
      SERIAL_RF.read();
    }
  
    do{
      usCurrentBufferStatus = SERIAL_RF.available();
      delay(5);
    }while(usCurrentBufferStatus != SERIAL_RF.available());
  
    while(SERIAL_RF.available()){
      SERIAL_RF.read();
    }
  #else
    ucUnitTestInputSize = ucUnitTestInputIndex = 0;
  #endif
  
  return;  
}


/**
 * @brief Update Frame buffer function.
 *
 * This function update the buffer RF.
 * @verbatim like this@endverbatim 
 * @param ucSize Data length for read.
 * @return none.
 */
bool updateBufferRF(uint8_t ucSize){
  static bool bFound = false;
  uint8_t ucValueRF = 0x00;
  uint16_t usCurrentBufferStatus = 0x00;

  if(ucSize == 0){
    bFound = false;
    return false;
    
  }else{
    
    if(bFound == false){
      
      #ifndef WIMOS_UNIT_TEST
        /*Wait the data entering.*/
        do{
          usCurrentBufferStatus = SERIAL_RF.available();
          delay(5);
        }while(usCurrentBufferStatus != SERIAL_RF.available());
      #endif

      #ifdef WIMOS_UNIT_TEST
        /*Read RF buffer until buffer empty or receive the 0xFF token.*/
        while(ucUnitTestInputIndex < ucUnitTestInputSize &&  ucValueRF != 0xFF ){
          ucValueRF = ucUnitTestInput[ucUnitTestInputIndex++];
        }
      #else
        /*Read RF buffer until buffer empty or receive the 0xFF token.*/
        while(SERIAL_RF.available() &&  ucValueRF != 0xFF ){
          ucValueRF = SERIAL_RF.read();
        }
      #endif
          
      /*The token was not received.*/
      if(ucValueRF != 0xFF){
        bFound = false;
        return false;
      /*The token was received.*/
      }else{
        bFound = true;   
        return false;   
      }    
    }else{    
      /*If Serial buffer available equal than ucSize.*/
      /* (ucSize - 1) =  Data without token.*/
      #ifdef WIMOS_UNIT_TEST
        if( (ucUnitTestInputSize - (ucUnitTestInputIndex - 1)) >= (ucSize - 1) ){
      #else
        if( SERIAL_RF.available() >= (ucSize - 1) ){
      #endif
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
      }
  }
}

/**
 * @brief Receive Frame function.
 *
 * This function receive the frame byte to byte.
 * @verbatim like this@endverbatim 
 * @param pData Pointer to data for sending.
 * @param ucSize Data length for sending.
 * @param ucFrameSize ID of message expected.
 * @return none.
 */
bool receiveFrame( void* pData, uint8_t ucSize, uint8_t ucFrameSize){
  uint8_t i=0;
  bool bFound = false;
    
  bFound = updateBufferRF(ucSize);
  
  if(bFound){
        
    ((uint8_t*)pData)[0] =  0xFF;
    #ifdef WIMOS_UNIT_TEST
      ((uint8_t*)pData)[1] =   ucUnitTestInput[ucUnitTestInputIndex++];
    #else
      ((uint8_t*)pData)[1] =   SERIAL_RF.read();
    #endif
    if( ((uint8_t*)pData)[1] != ucFrameSize  || SERIAL_RF.available() < (ucSize - 2)){
      return false;
    }else{
      #ifdef DEBUG_COMM_RXTX
        SERIAL_DEBUG.print("RX: ");
        SERIAL_DEBUG.print(((uint8_t*)pData)[0],HEX);
        SERIAL_DEBUG.print(" ");
        SERIAL_DEBUG.print(((uint8_t*)pData)[1],HEX);
        SERIAL_DEBUG.print(" ");
      #endif
      for(i=2; i < ucSize; i++){
        #ifdef WIMOS_UNIT_TEST
          ((uint8_t*)pData)[i] =  ucUnitTestInput[ucUnitTestInputIndex++];
        #else
          ((uint8_t*)pData)[i] =  SERIAL_RF.read();
        #endif
        
      #ifdef DEBUG_COMM_RXTX
        SERIAL_DEBUG.print(((uint8_t*)pData)[i],HEX);
        SERIAL_DEBUG.print(" ");
      #endif
      }
      clearBufferRF();
      #ifdef DEBUG_COMM_RXTX
        SERIAL_DEBUG.println();
      #endif
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
    clearBufferRF();
    #ifdef DEBUG_COMM_RXTX
      SERIAL_DEBUG.print("TX: ");
    #endif
    for(i=0; i<ucSize/PACKAGE_SIZE_RF; i++){
      for(uint8_t j=0; j<PACKAGE_SIZE_RF; j++){
      DEBUG_DATA("Send Byte = %d.", ((uint8_t*)pData)[i]);
      #ifdef WIMOS_UNIT_TEST
        ucUnitTestOutput[i] = ((uint8_t*)pData)[(i*PACKAGE_SIZE_RF)+j];
      #else
        #ifdef DEBUG_COMM_RXTX
          SERIAL_DEBUG.print(((uint8_t*)pData)[(i*PACKAGE_SIZE_RF)+j],HEX);
          SERIAL_DEBUG.print(" ");
        #endif
        SERIAL_RF.write(((uint8_t*)pData)[(i*PACKAGE_SIZE_RF)+j]);
        /*Go to the first state.*/
        #ifdef __AVR_ATmega32U4__
          delayMicroseconds(250);        
        #endif
        #ifdef __SAM3X8E__
          delayMicroseconds(250);
        #endif 
      }
      delay(10);
    }
    if(ucSize % PACKAGE_SIZE_RF > 0){
      for(i=0; i< (ucSize % PACKAGE_SIZE_RF); i++){
        #ifdef DEBUG_COMM_RXTX
          SERIAL_DEBUG.print(((uint8_t*)pData)[((ucSize / PACKAGE_SIZE_RF)*PACKAGE_SIZE_RF)+i],HEX);
          SERIAL_DEBUG.print(" ");
        #endif
        SERIAL_RF.write(((uint8_t*)pData)[((ucSize / PACKAGE_SIZE_RF)*PACKAGE_SIZE_RF)+i]);
        /*Go to the first state.*/
        #ifdef __AVR_ATmega32U4__
          delayMicroseconds(250);        
        #endif
        #ifdef __SAM3X8E__
          delayMicroseconds(250);
        #endif   
      }
      for(i=0; i< PACKAGE_SIZE_RF - (ucSize % PACKAGE_SIZE_RF); i++){
        #ifdef DEBUG_COMM_RXTX
          SERIAL_DEBUG.print(0x00 ,HEX);
          SERIAL_DEBUG.print(" ");
        #endif
        SERIAL_RF.write((uint8_t)0x00);
        /*Go to the first state.*/
        #ifdef __AVR_ATmega32U4__
          delayMicroseconds(250);        
        #endif
        #ifdef __SAM3X8E__
          delayMicroseconds(250);
        #endif 
      #endif  
      }
      delay(10);
      
    }
    
    #ifdef DEBUG_COMM_RXTX
      SERIAL_DEBUG.println();
    #endif
  #endif
  return;
}


#ifdef WIMOS_DEBUG
  #ifdef WIMOS_UNIT_TEST
      
  /**
   * @brief Wimos test n3.UT20.
   *
   * Unit test n3.UT20 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n3UT20 (void){
    const char* testName = "n3.UT20 = %ld";
    stCommandMessage stTestCommand = (stCommandMessage) { .ucBegin = 0xAA,
                                                           .ucFrameID = 0xAB, 
                                                           .ucMessageFrom = 0xAC, 
                                                           .ucMessageTo = 0xAE, 
                                                           .ucCommand = 0xAF, 
                                                           .ucChecksum = 0xB0 };

    
    /*Body_TEST:*/     
    sendFrame(&stTestCommand,sizeof(stTestCommand));
    DEBUG_VALID(testName , 
               (memcmp(&stTestCommand,ucUnitTestOutput,sizeof(stTestCommand))), 
               (memcmp(&stTestCommand,ucUnitTestOutput,sizeof(stTestCommand)) == 0));
    /*End_Body_TEST:*/
  }
      
  /**
   * @brief Wimos test n4.UT01.
   *
   * Unit test n4.UT01 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT01 (void){
    const char* testName = "n4.UT01 = %ld";
    bool bReturnValue = true;
    stCommandMessage stTestCommand = (stCommandMessage) { .ucBegin = 0xFF,
                                                           .ucFrameID = 0x04, 
                                                           .ucMessageFrom = 0xAC, 
                                                           .ucMessageTo = 0xAE, 
                                                           .ucCommand = 0xAF, 
                                                           .ucChecksum = 0xB0 };
    memset(ucUnitTestInput,0,sizeof(stTestCommand));    
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestCommand));
    
    memcpy(ucUnitTestInput,&stTestCommand,sizeof(stTestCommand));
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
        
    /*Body_TEST:*/     
    bReturnValue = updateBufferRF(sizeof(stTestCommand));
    DEBUG_VALID(testName , 
               (ucUnitTestInputIndex), 
               (bReturnValue == true && ucUnitTestInputIndex == 1));
    /*End_Body_TEST:*/
  }
      
  /**
   * @brief Wimos test n4.UT02.
   *
   * Unit test n4.UT02 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT02 (void){
    const char* testName = "n4.UT02 = %ld";
    bool bReturnValue = true;
    stCommandMessage stTestCommand = (stCommandMessage) { .ucBegin = 0x00,
                                                           .ucFrameID = 0x04, 
                                                           .ucMessageFrom = 0xAC, 
                                                           .ucMessageTo = 0xAE, 
                                                           .ucCommand = 0xAF, 
                                                           .ucChecksum = 0xB0 };
    memset(ucUnitTestInput,0,sizeof(stTestCommand));    
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestCommand));
    
    memcpy(ucUnitTestInput,&stTestCommand,sizeof(stTestCommand));
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
        
    /*Body_TEST:*/     
    bReturnValue = updateBufferRF(sizeof(stTestCommand));
    DEBUG_VALID(testName , 
               (ucUnitTestInputIndex), 
               (bReturnValue == false && ucUnitTestInputIndex == 6));
    /*End_Body_TEST:*/
  }
   
      
  /**
   * @brief Wimos test n4.UT03.
   *
   * Unit test n4.UT03 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT03 (void){
    const char* testName = "n4.UT03 = %ld";
    bool bReturnValue = true;
    stCommandMessage stTestCommand = (stCommandMessage) { .ucBegin = 0x00,
                                                           .ucFrameID = 0x04, 
                                                           .ucMessageFrom = 0xAC, 
                                                           .ucMessageTo = 0xAE, 
                                                           .ucCommand = 0xFF, 
                                                           .ucChecksum = 0x0F };
    
    memcpy(ucUnitTestInput,&stTestCommand,sizeof(stTestCommand));    
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
        
    /*Body_TEST:*/     
    bReturnValue = updateBufferRF(sizeof(stTestCommand));
    DEBUG_VALID(testName , 
               (ucUnitTestInputIndex), 
               (bReturnValue == false && ucUnitTestInputIndex == 5));
               
    memset(ucUnitTestInput,0,sizeof(stTestCommand));    
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestCommand));
    /*End_Body_TEST:*/
  }
   
      
  /**
   * @brief Wimos test n4.UT04.
   *
   * Unit test n4.UT04 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT04 (void){
    const char* testName = "n4.UT04 = %ld";
    bool bReturnValue = true;
    stCommandMessage stTestCommand = (stCommandMessage) { .ucBegin = 0xFF,
                                                           .ucFrameID = 0x04, 
                                                           .ucMessageFrom = 0xAC, 
                                                           .ucMessageTo = 0xAE, 
                                                           .ucCommand = 0x01, 
                                                           .ucChecksum = 0x0F };
    
    stCommandMessage stTestCommandAux;
    memcpy(ucUnitTestInput,&stTestCommand,sizeof(stTestCommand));
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
            
    /*Body_TEST:*/     
    bReturnValue = receiveFrame(&stTestCommandAux,sizeof(stTestCommandAux),0x04);
    DEBUG_VALID(testName , 
               (bReturnValue), 
               (bReturnValue == true && memcmp(&stTestCommand, &stTestCommandAux, sizeof(stTestCommand)) == 0));
               
    memset(ucUnitTestInput,0,sizeof(stTestCommand));    
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestCommand));
    /*End_Body_TEST:*/
  }
  
   
      
  /**
   * @brief Wimos test n4.UT05.
   *
   * Unit test n4.UT05 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT05 (void){
    const char* testName = "n4.UT05 = %ld";
    bool bReturnValue = true;
    stCommandMessage stTestCommand = (stCommandMessage) { .ucBegin = 0x00,
                                                           .ucFrameID = 0x00, 
                                                           .ucMessageFrom = 0xAC, 
                                                           .ucMessageTo = 0xAE, 
                                                           .ucCommand = 0xFF, 
                                                           .ucChecksum = 0x0F };
    
    stCommandMessage stTestCommandAux;                                                           
    memcpy(ucUnitTestInput,&stTestCommand,sizeof(stTestCommand));
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
        
    /*Body_TEST:*/     
    bReturnValue = receiveFrame(&stTestCommandAux,sizeof(stTestCommandAux),0x04);
    DEBUG_VALID(testName , 
               (bReturnValue), 
               (bReturnValue == false && memcmp(&stTestCommand, &stTestCommandAux, sizeof(stTestCommand))!= 0));
               
    memset(ucUnitTestInput,0,sizeof(stTestCommand));    
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestCommand));
    /*End_Body_TEST:*/
  }
  
  
   
#ifdef __SAM3X8E__
      
  /**
   * @brief Wimos test n4.UT06.
   *
   * Unit test n4.UT06 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT06 (void){
    const char* testName = "n4.UT06 = %ld";
    bool bReturnValue = true;
    stCommandMessage stTestCommand = (stCommandMessage) { .ucBegin = 0xFF,
                                                           .ucFrameID = 0x04, 
                                                           .ucMessageFrom = 0xAC, 
                                                           .ucMessageTo = WIMOS_ID, 
                                                           .ucCommand = 0x55, 
                                                           .ucChecksum = 0xAC };                                   
    stTestCommand.ucChecksum = getChecksum(&stTestCommand, sizeof(stTestCommand)-1);                                                       
    memcpy(ucUnitTestInput,&stTestCommand,sizeof(stTestCommand));
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
        
    /*Body_TEST:*/     
    waitCommand();
    DEBUG_VALID(testName , 
               (bReturnValue), 
               (communicationThread == sendACK));
               
    memset(ucUnitTestInput,0,sizeof(stTestCommand));    
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestCommand));
    /*End_Body_TEST:*/
  }
  
  
   
      
  /**
   * @brief Wimos test n4.UT07.
   *
   * Unit test n4.UT07 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT07 (void){
    const char* testName = "n4.UT07 = %ld";
    bool bReturnValue = true;
    stCommandMessage stTestCommand = (stCommandMessage) { .ucBegin = 0xFE,
                                                           .ucFrameID = 0x04, 
                                                           .ucMessageFrom = 0xAC, 
                                                           .ucMessageTo = WIMOS_ID, 
                                                           .ucCommand = 0x55, 
                                                           .ucChecksum = 0xAC };                                   
                                       
    stTestCommand.ucChecksum = getChecksum(&stTestCommand, sizeof(stTestCommand)-1);                                                       
    memcpy(ucUnitTestInput,&stTestCommand,sizeof(stTestCommand));
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    communicationThread = waitCommand;
        
    /*Body_TEST:*/     
    waitCommand();
    DEBUG_VALID(testName , 
               (bReturnValue), 
               (communicationThread == waitCommand));
               
    memset(ucUnitTestInput,0,sizeof(stTestCommand));    
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestCommand));
    /*End_Body_TEST:*/
  }
  
  
   
      
  /**
   * @brief Wimos test n4.UT08.
   *
   * Unit test n4.UT08 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT08 (void){
    const char* testName = "n4.UT08 = %ld";
    bool bReturnValue = true;
    stCommandMessage stTestCommand = (stCommandMessage) { .ucBegin = 0xFF,
                                                           .ucFrameID = 0x03, 
                                                           .ucMessageFrom = 0xAC, 
                                                           .ucMessageTo = WIMOS_ID, 
                                                           .ucCommand = 0x55, 
                                                           .ucChecksum = 0xAC };   
                                                                                              
    stTestCommand.ucChecksum = getChecksum(&stTestCommand, sizeof(stTestCommand)-1);
    memcpy(ucUnitTestInput,&stTestCommand,sizeof(stTestCommand));
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    communicationThread = waitCommand;
        
    /*Body_TEST:*/     
    waitCommand();
    DEBUG_VALID(testName , 
               (bReturnValue), 
               (communicationThread == waitCommand));

    memset(ucUnitTestInput,0,sizeof(stTestCommand));    
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestCommand));
    /*End_Body_TEST:*/
  }
  
  
   
      
  /**
   * @brief Wimos test n4.UT09.
   *
   * Unit test n4.UT09 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT09 (void){
    const char* testName = "n4.UT09 = %ld";
    bool bReturnValue = true;
    stCommandMessage stTestCommand = (stCommandMessage) { .ucBegin = 0xFF,
                                                           .ucFrameID = 0x03, 
                                                           .ucMessageFrom = 0xAC, 
                                                           .ucMessageTo = WIMOS_ID+1, 
                                                           .ucCommand = 0x55, 
                                                           .ucChecksum = 0xAC };   
                                                                                              
    stTestCommand.ucChecksum = getChecksum(&stTestCommand, sizeof(stTestCommand)-1);
    memcpy(ucUnitTestInput,&stTestCommand,sizeof(stTestCommand));
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    communicationThread = waitCommand;
        
    /*Body_TEST:*/     
    waitCommand();
    DEBUG_VALID(testName , 
               (bReturnValue), 
               (communicationThread == waitCommand));
               
    memset(ucUnitTestInput,0,sizeof(stTestCommand));    
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestCommand));
    /*End_Body_TEST:*/
  }
  
  
   
      
  /**
   * @brief Wimos test n4.UT10.
   *
   * Unit test n4.UT10 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT10 (void){
    const char* testName = "n4.UT10 = %ld";
    bool bReturnValue = true;
    stCommandMessage stTestCommand = (stCommandMessage) { .ucBegin = 0xFF,
                                                           .ucFrameID = 0x03, 
                                                           .ucMessageFrom = 0xAC, 
                                                           .ucMessageTo = WIMOS_ID, 
                                                           .ucCommand = 0x55, 
                                                           .ucChecksum = 0xAC };   
                                                                                              
    stTestCommand.ucChecksum = getChecksum(&stTestCommand, sizeof(stTestCommand)-1)- 1;
    memcpy(ucUnitTestInput,&stTestCommand,sizeof(stTestCommand));
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    communicationThread = waitCommand;
        
    /*Body_TEST:*/     
    waitCommand();
    DEBUG_VALID(testName , 
               (bReturnValue), 
               (communicationThread == waitCommand));
               
    memset(ucUnitTestInput,0,sizeof(stTestCommand));    
    ucUnitTestInputSize  =  sizeof(stTestCommand);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestCommand));
    /*End_Body_TEST:*/
  }
  #endif
      
  #ifdef __AVR_ATmega32U4__
    /**
     * @brief Wimos test n4.UT11.
     *
     * Unit test n4.UT11 function.
     * @verbatim like this@endverbatim 
     * @param none.
     * @return none.
     */
    extern void _test_n4UT11 (void){
      const char* testName = "n4.UT11 = %ld";
      bool bReturnValue = true;
      communicationThread = sendCommand;
      memset(ucUnitTestOutput,0, sizeof(stCommandMessage));
      /*Body_TEST:*/     
      sendCommand();
      DEBUG_VALID(testName , 
                 (memcmp(prtMessageArray,ucUnitTestOutput,sizeof(stCommandMessage))==0), 
                 (memcmp(prtMessageArray,ucUnitTestOutput,sizeof(stCommandMessage))==0) && 
                 (communicationThread == waitACK));
                 
      /*End_Body_TEST:*/
    }
  
  
  #endif

  /**
   * @brief Wimos test n4.UT12.
   *
   * Unit test n4.UT12 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT12 (void){
    const char* testName = "n4.UT12 = %ld";
    bool bReturnValue = true;    
    stWimosACK stTestACK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = WIMOS_ID, 
                                           .ucMessageTo = stCommand.ucMessageFrom, 
                                           .ucACK = 0x02};  
    
    stCommandMessage stTestACKAux;                                                           
    memcpy(ucUnitTestInput,&stTestACK,sizeof(stTestACK));
    ucUnitTestInputSize  =  sizeof(stTestACK);
    ucUnitTestInputIndex = 0;
        
    /*Body_TEST:*/     
    bReturnValue = receiveFrame(&stTestACKAux,sizeof(stTestACKAux),0x03);
    DEBUG_VALID(testName , 
               (bReturnValue), 
               (bReturnValue == true && memcmp(&stTestACK, &stTestACKAux, sizeof(stTestACK))== 0));
               
    memset(ucUnitTestInput,0,sizeof(stTestACK));    
    ucUnitTestInputSize  =  sizeof(stTestACK);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestACK));
    /*End_Body_TEST:*/
  }


  /**
   * @brief Wimos test n4.UT13.
   *
   * Unit test n4.UT13 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT13 (void){
    const char* testName = "n4.UT13 = %ld";
    bool bReturnValue = true;    
    #ifdef __AVR_ATmega32U4__
    stWimosACK stTestACK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = WIMOS_ID, 
                                           .ucMessageTo = stCommand.ucMessageTo, 
                                           .ucACK = 0x02};  
    #endif
    
    #ifdef __SAM3X8E__
    stWimosACK stTestACK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = WIMOS_ID, 
                                           .ucMessageTo = stCommand.ucMessageFrom, 
                                           .ucACK = 0x02};
    #endif
          
    /*Body_TEST:*/       
    /*Reset SendACK internal state*/
    communicationThread = sendACK;
    
    /*Reset the RF output*/
    memset(&ucUnitTestOutput,0,sizeof(stTestACK));
    /*Force the last Checksum to 2*/
    ucLastChecksum = 0x02;
        
    /*Send the ACK*/
    sendACK();
    
    /*Check the ACK was sent.*/
    bReturnValue = (memcmp(&stTestACK,ucUnitTestOutput,sizeof(stTestACK)) == 0);
    
    DEBUG_VALID(testName , 
               (bReturnValue), 
               (bReturnValue));
               
    memset(ucUnitTestInput,0,sizeof(stTestACK));    
    ucUnitTestInputSize  =  sizeof(stTestACK);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestACK));
    /*End_Body_TEST:*/
  }
  
  /**
   * @brief Wimos test n4.UT14.
   *
   * Unit test n4.UT14 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT14 (void){
    const char* testName = "n4.UT14 = %ld";
    bool bReturnValue = true;    
                                           
    #ifdef __AVR_ATmega32U4__
    stWimosACK stTestACK_OK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = stCommand.ucMessageTo, 
                                           .ucMessageTo = WIMOS_ID, 
                                           .ucACK = 0x04};  
    #endif
    
    #ifdef __SAM3X8E__
    stWimosACK stTestACK_OK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = stCommand.ucMessageFrom, 
                                           .ucMessageTo = WIMOS_ID, 
                                           .ucACK = 0x04};  
    #endif      
          
    /*Body_TEST:*/       
    /*Reset SendACK internal state*/
    communicationThread = sendACK;
    
    /*Reset the RF output*/
    memcpy(ucUnitTestInput,&stTestACK_OK,sizeof(stTestACK_OK));  
    ucUnitTestInputSize  =  sizeof(stTestACK_OK);
    ucUnitTestInputIndex = 0;
    /*Force the last Checksum to 2*/
    ucLastChecksum = 0x04;
        
    /*Send the ACK*/
    sendACK();
    
    #ifdef __SAM3X8E__
    DEBUG_VALID(testName , 
               (communicationThread == runFunction), 
               (communicationThread == runFunction));
    #endif           
    #ifdef __AVR_ATmega32U4__
    DEBUG_VALID(testName , 
               (communicationThread == sendCommand), 
               (communicationThread == sendCommand));    
    #endif
    
    memset(ucUnitTestInput,0,sizeof(stTestACK_OK));    
    ucUnitTestInputSize  =  sizeof(stTestACK_OK);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestACK_OK));
    /*End_Body_TEST:*/
  }


  
  /**
   * @brief Wimos test n4.UT15.
   *
   * Unit test n4.UT15 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT15 (void){
    const char* testName = "n4.UT14 = %ld";
    bool bReturnValue = true;    
                                           
    #ifdef __AVR_ATmega32U4__
    stWimosACK stTestACK_OK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = stCommand.ucMessageTo, 
                                           .ucMessageTo = WIMOS_ID, 
                                           .ucACK = 0x03};  
                                           
    stWimosACK stTestACK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = WIMOS_ID, 
                                           .ucMessageTo = stCommand.ucMessageTo, 
                                           .ucACK = 0x02};  
    #endif
    
    #ifdef __SAM3X8E__
    stWimosACK stTestACK_OK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = stCommand.ucMessageFrom, 
                                           .ucMessageTo = WIMOS_ID, 
                                           .ucACK = 0x05};  
                                           
    stWimosACK stTestACK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = WIMOS_ID, 
                                           .ucMessageTo = stCommand.ucMessageFrom, 
                                           .ucACK = 0x03};
    #endif 
          
    /*Body_TEST:*/       
    /*Reset SendACK internal state*/
    communicationThread = sendACK;
    
    /*Reset the RF output*/
    memset(&ucUnitTestOutput,0,sizeof(stTestACK));
    /*Force the last Checksum to 2*/
    ucLastChecksum = 0x02;
        
    /*Send the ACK*/
    sendACK();
   
    memset(ucUnitTestInput,0,sizeof(stTestACK));    
    ucUnitTestInputSize  =  sizeof(stTestACK);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestACK));
    
    /*Reset SendACK internal state*/
    communicationThread = sendACK;
    
    /*Reset the RF output*/
    memcpy(ucUnitTestInput,&stTestACK_OK,sizeof(stTestACK_OK));  
    ucUnitTestInputSize  =  sizeof(stTestACK_OK);
    ucUnitTestInputIndex = 0;
    /*Force the last Checksum to 2*/
    ucLastChecksum = 0x04;
        
    /*Send the ACK*/
    sendACK();
    
    #ifdef __SAM3X8E__
    DEBUG_VALID(testName , 
               (communicationThread == waitCommand), 
               (communicationThread == waitCommand));
    #endif           
    #ifdef __AVR_ATmega32U4__
    DEBUG_VALID(testName , 
               (communicationThread == sendCommand), 
               (communicationThread == sendCommand));    
    #endif
    
    memset(ucUnitTestInput,0,sizeof(stTestACK_OK));    
    ucUnitTestInputSize  =  sizeof(stTestACK_OK);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestACK_OK));
    /*End_Body_TEST:*/
  }


  /**
   * @brief Wimos test n4.UT16.
   *
   * Unit test n4.UT16 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT16 (void){
    const char* testName = "n4.UT16 = %ld";
    bool bReturnValue = true;    
    #ifdef __AVR_ATmega32U4__
                                           
    stWimosACK stTestACK_OK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = WIMOS_ID, 
                                           .ucMessageTo = stCommand.ucMessageTo, 
                                           .ucACK = 0x04};
                                           
    stWimosACK stTestACK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = stCommand.ucMessageTo, 
                                           .ucMessageTo = WIMOS_ID, 
                                           .ucACK = 0x02};    
    #endif
    
    #ifdef __SAM3X8E__
    stWimosACK stTestACK_OK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = WIMOS_ID, 
                                           .ucMessageTo = stCommand.ucMessageFrom, 
                                           .ucACK = 0x04};  
                                           
    stWimosACK stTestACK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = stCommand.ucMessageFrom, 
                                           .ucMessageTo = WIMOS_ID, 
                                           .ucACK = 0x02};
    #endif
          
    /*Body_TEST:*/       
    /*Reset SendACK internal state*/
    communicationThread = waitACK;
    
    /*Reset the RF output*/
    memcpy(ucUnitTestInput,&stTestACK,sizeof(stTestACK));
    ucUnitTestInputSize = sizeof(stTestACK);
    ucUnitTestInputIndex = 0;
    
    /*Force the last Checksum to 2*/
    ucLastChecksum = 0x02;
        
    /*Send the ACK*/
    waitACK();
    
    /*Check the ACK was sent.*/
    bReturnValue = (memcmp(&stTestACK_OK,ucUnitTestOutput,sizeof(stTestACK_OK)) == 0);

    #ifdef __SAM3X8E__
      bReturnValue &= (communicationThread == waitCommand);
    #endif 
    #ifdef __AVR_ATmega32U4__
      bReturnValue &= (communicationThread == runFunction);    
    #endif
    
    DEBUG_VALID(testName , 
               (bReturnValue), 
               (bReturnValue));
               
    memset(ucUnitTestInput,0,sizeof(stTestACK));    
    ucUnitTestInputSize  =  sizeof(stTestACK);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestACK));
    /*End_Body_TEST:*/
  }
  


  /**
   * @brief Wimos test n4.UT17.
   *
   * Unit test n4.UT17 function.
   * @verbatim like this@endverbatim 
   * @param none.
   * @return none.
   */
  extern void _test_n4UT17 (void){
    const char* testName = "n4.UT17 = %ld";
    bool bReturnValue = true;    
    #ifdef __AVR_ATmega32U4__
                                           
    stWimosACK stTestACK_OK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = WIMOS_ID, 
                                           .ucMessageTo = stCommand.ucMessageTo, 
                                           .ucACK = 0x03};
                                           
    stWimosACK stTestACK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = stCommand.ucMessageTo, 
                                           .ucMessageTo = WIMOS_ID, 
                                           .ucACK = 0x01};    
    #endif
    
    #ifdef __SAM3X8E__
    stWimosACK stTestACK_OK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = WIMOS_ID, 
                                           .ucMessageTo = stCommand.ucMessageFrom, 
                                           .ucACK = 0x03};  
                                           
    stWimosACK stTestACK = (stWimosACK) {  .ucBegin = 0xFF,
                                           .ucFrameID = 0x03, 
                                           .ucMessageFrom = stCommand.ucMessageFrom, 
                                           .ucMessageTo = WIMOS_ID, 
                                           .ucACK = 0x01};
    #endif
          
    /*Body_TEST:*/       
    /*Reset SendACK internal state*/
    communicationThread = waitACK;
    
    /*Reset the RF output*/
    memcpy(ucUnitTestInput,&stTestACK,sizeof(stTestACK));
    ucUnitTestInputSize = sizeof(stTestACK);
    ucUnitTestInputIndex = 0;
    
    /*Force the last Checksum to 2*/
    ucLastChecksum = 0x02;
        
    /*Send the ACK*/
    waitACK();
    
    /*Check the ACK was sent.*/
    bReturnValue = (memcmp(&stTestACK_OK,ucUnitTestOutput,sizeof(stTestACK_OK)) == 0);

    #ifdef __SAM3X8E__
      bReturnValue &= (communicationThread == waitCommand);
    #endif 
    #ifdef __AVR_ATmega32U4__
      bReturnValue &= (communicationThread == sendCommand);    
    #endif
    
    DEBUG_VALID(testName , 
               (bReturnValue), 
               (bReturnValue));
               
    memset(ucUnitTestInput,0,sizeof(stTestACK));    
    ucUnitTestInputSize  =  sizeof(stTestACK);
    ucUnitTestInputIndex = 0;
    updateBufferRF(sizeof(stTestACK));
    /*End_Body_TEST:*/
  }
  
  
  #ifdef __SAM3X8E__
   /**
    * @brief Wimos test n4.UT18.
    *
    * Unit test n4.UT18 function.
    * @verbatim like this@endverbatim 
    * @param none.
    * @return none.
    */
    extern void _test_n4UT18 (void){
      const char* testName = "n4.UT18 = %ld";
      bool bReturnValue = true;    
      
      /*Body_TEST:*/       
      /*Reset SendACK internal state*/
      communicationThread = runFunction;
      stCommand.ucCommand = COMMAND_GET_GENERAL_INFO;
      stGlobalWimosInfoMsg.ucChecksum = getChecksum(&stGlobalWimosInfoMsg, sizeof(stGlobalWimosInfoMsg)-1);
      /*Send the ACK*/
      runFunction();
      
      /*Check the ACK was sent.*/
      bReturnValue = (memcmp(&stGlobalWimosInfoMsg,ucUnitTestOutput,sizeof(stGlobalWimosInfoMsg)) == 0);
  
      bReturnValue &= (communicationThread == waitACK);
      
      DEBUG_VALID(testName , 
                 (bReturnValue), 
                 (bReturnValue));
                 
      /*End_Body_TEST:*/
    }
  #endif

  
  #ifdef __AVR_ATmega32U4__
   /**
    * @brief Wimos test n4.UT19.
    *
    * Unit test n4.UT19 function.
    * @verbatim like this@endverbatim 
    * @param none.
    * @return none.
    */
    extern void _test_n4UT19 (void){
      const char* testName = "\nn4.UT19 = %ld";
      bool bReturnValue = true;    
      
      /*Body_TEST:*/       
      /*Reset SendACK internal state*/
      communicationThread = runFunction;
      
      stCommand.ucCommand = COMMAND_GET_GENERAL_INFO;
      
      stGlobalWimosInfoMsg.ucBegin = 0xFF;
      stGlobalWimosInfoMsg.ucFrameID = 0x05;
      stGlobalWimosInfoMsg.ucMessageFrom = stCommand.ucMessageTo;
      stGlobalWimosInfoMsg.ucMessageTo = WIMOS_ID;
      stGlobalWimosInfoMsg.ucChecksum = getChecksum(&stGlobalWimosInfoMsg, sizeof(stGlobalWimosInfoMsg)-1);
    
      memcpy(ucUnitTestInput,&stGlobalWimosInfoMsg,sizeof(stGlobalWimosInfoMsg));
      ucUnitTestInputSize = sizeof(stGlobalWimosInfoMsg);
      ucUnitTestInputIndex = 0;
      /*Send the ACK*/
      runFunction();
      
      /*Check the ACK was sent.*/
      bReturnValue = (communicationThread == sendACK);
      
      DEBUG_VALID(testName , 
                 (bReturnValue), 
                 (bReturnValue));
                 
      /*End_Body_TEST:*/
    }
  #endif


  
  #ifdef __AVR_ATmega32U4__
   /**
    * @brief Wimos test n4.UT20.
    *
    * Unit test n4.UT20 function.
    * @verbatim like this@endverbatim 
    * @param none.
    * @return none.
    */
    extern void _test_n4UT20 (void){
      const char* testName = "n4.UT20 = %ld";
      bool bReturnValue = true;    
      
      /*Body_TEST:*/       
      /*Reset SendACK internal state*/
      communicationThread = runFunction;
      stCommand.ucCommand = COMMAND_GET_GENERAL_INFO;
      
      stGlobalWimosInfoMsg.ucBegin = 0xFF;
      stGlobalWimosInfoMsg.ucFrameID = 0x05;
      stGlobalWimosInfoMsg.ucMessageFrom = stCommand.ucMessageTo;
      stGlobalWimosInfoMsg.ucMessageTo = WIMOS_ID;
      stGlobalWimosInfoMsg.ucChecksum = getChecksum(&stGlobalWimosInfoMsg, sizeof(stGlobalWimosInfoMsg)-1)+1;
    
    
      memcpy(ucUnitTestInput,&stGlobalWimosInfoMsg,sizeof(stGlobalWimosInfoMsg));
      ucUnitTestInputSize = sizeof(stGlobalWimosInfoMsg);
      ucUnitTestInputIndex = 0;
      /*Send the ACK*/
      runFunction();
      
      /*Check the ACK was sent.*/
      bReturnValue = (communicationThread == sendCommand);
      
      DEBUG_VALID(testName , 
                 (bReturnValue), 
                 (bReturnValue));
                 
      /*End_Body_TEST:*/
    }
  #endif




   
  #endif

  #ifdef WIMOS_VALIDATION_TEST
  
      #ifdef __AVR_ATmega32U4__
        /**
         * @brief Wimos test n3.VT03.
         *
         * Unit test n4.VT01 function.
         * @verbatim like this@endverbatim 
         * @param none.
         * @return none.
         */
        extern void _test_n4VT01 (void){
          const char* testName = "n4.VT01 = %ld";     
          
          ulTimeCommandsRate = millis();     
          communicationThread = sendCommand;
          /*Body_TEST:*/    
          do{
            communicationThread();
          }while(bValidationOnGoing == false);
          DEBUG_VALID(testName , 
                     ((uint8_t)((float)((float)ucCommandsResponseReceived / (float)ucCommandsSent)) * 100), 
                     ((((uint8_t)((float)((float)ucCommandsResponseReceived / (float)ucCommandsSent)) * 100)>= 85 ) &&
                     (((uint32_t)((float)((float) TIME_COMMAND_RATE / (float)ucCommandsResponseReceived))) <= 333 ) ));
          /*End_Body_TEST:*/
        } 
        
    #endif
  #endif
#endif
