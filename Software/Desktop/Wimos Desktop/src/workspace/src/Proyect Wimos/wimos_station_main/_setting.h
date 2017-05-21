#define WIMOS_ID 0
#ifndef __AVR_ATmega32U4__
	#error "You should select the Center Board"
#endif
#define WIMOS_UNIT_TEST
#undef DEBUG_COMM_RXTX
#undef DEBUG_COMM_STATUS
#undef DEBUG_ANALOG_A1
#undef DEBUG_ANALOG_A2
#undef DEBUG_ANALOG_A3
#undef DEBUG_ANALOG_A4
#undef DEBUG_ANALOG_A5
#undef WIMOS_DEBUG
#define _EN_WIMOS_RF 
#define TIME_COMMAND_RATE 60000 
#define _WIMOS_COMMAND_LIST_SIZE 1 
#define _WIMOS_COMMAND_LIST {\ 
(stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 0, .ucCommand = 0, .ucChecksum = 0x00 } \ 
}