#define WIMOS_ID 0
#ifndef __AVR_ATmega32U4__
	#error "You should select the Center Board"
#endif
#define WIMOS_DEBUG
#undef WIMOS_UNIT_TEST
#undef WIMOS_VALIDATION_TEST
#define _EN_WIMOS_RF 
#define TIME_COMMAND_RATE 60000 
#define _WIMOS_COMMAND_LIST_SIZE 5 
#define _WIMOS_COMMAND_LIST {\ 
(stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 1, .ucCommand = 162, .ucChecksum = 0x00 },\ 
(stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 1, .ucCommand = 162, .ucChecksum = 0x00 },\ 
(stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 1, .ucCommand = 162, .ucChecksum = 0x00 },\ 
(stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 1, .ucCommand = 162, .ucChecksum = 0x00 },\ 
(stCommandMessage) { .ucBegin = COMMAND_BEGIN_BYTE_CONST, .ucFrameID = COMMAND_SIZE_BYTE_CONST, .ucMessageFrom = WIMOS_ID, .ucMessageTo = 1, .ucCommand = 161, .ucChecksum = 0x00 } \ 
}