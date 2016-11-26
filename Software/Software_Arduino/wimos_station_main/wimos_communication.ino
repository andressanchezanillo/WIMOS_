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
void sendFrame(void* ptrData, uint8_t ucSize);

extern void (*communicationThread)(void);
static uint8_t lastCMD = NONE;

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
    /*TODO: Añadir inicialización del serial.*/
    /*communicationThread = waitCommand;*/
    SERIAL_RF.begin(BAUDRATE_RF);
    DEBUG_OK("Communication RF initialized.");
  #else
    /*communicationThread = noOperation;*/
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
  /** Process RF input **/
  if(COMMAND && IS_FOR_ME){
    /** Process RF command **/
    /** cmd is valid **/
    if(QUERY_ID){
      communicationThread = runFunction;
      return;
    }else{
      communicationThread = sendACK;
      return;
    }
  }
  if(BUFFER_FULL){
      
    communicationThread = clearBuffer;
    return;
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
  
  if(ACK_RECEIVED){
    /*Send ACK confirmation.*/
    ACK_SENT = false;
    ACK_ATTEMPTS = 0;
    ACK_RECEIVED = false;
    communicationThread = waitCommand;
  }else{
    if(TIMEOUT){
      ACK_SENT = false;
      ACK_ATTEMPTS = 0;
      ACK_RECEIVED = false;
      communicationThread = runFunction;
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
  if(! ACK_SENT){
    ACK_SENT = !ACK_SENT;
    ACK_ATTEMPTS = 0;
    sendFrame(&ACK_SENT,1);
  }else{
    if(ACK_RECEIVED){
      ACK_SENT = false;
      ACK_ATTEMPTS = 0;
      ACK_RECEIVED = false;
      communicationThread = runFunction;
    }else{
      if(ACK_ATTEMPTS < 3){
        ACK_ATTEMPTS++;
      }else{
        ACK_SENT = false;
        ACK_ATTEMPTS = 0;
        ACK_RECEIVED = false;
        communicationThread = waitCommand;
      }       
    }
  }
  return;
}

/**
 * @brief Clear buffer State.
 *
 * This is a state of communication state machine, it clears the buffer.
 * @verbatim like this@endverbatim 
 * @param none.
 * @return none.
 */
void clearBuffer(void){
  /* Clear Buffer */
  communicationThread = waitCommand;
  return;
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
  /*TODO*/
  return;
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
void sendFrame(void* pData, uint8_t ucSize){
  /** Send the frame over RF **/
  uint8_t i = 0;
  for(i=0; i<ucSize; i++)
    SERIAL_RF.write(((uint8_t*)pData)[i]);
    
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
