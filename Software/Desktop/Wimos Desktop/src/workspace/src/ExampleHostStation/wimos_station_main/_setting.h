#define WIMOS_ID 1
#ifndef __SAM3X8E__
	#error "You should select the Host Board"
#endif
#define WIMOS_DEBUG
#undef WIMOS_UNIT_TEST
#undef WIMOS_VALIDATION_TEST
#define _EN_WIMOS_GPS
#undef _EN_WIMOS_PORT_A1 
#define _WIMOS_1A_OFFSET_1 	 - VCC_LOGIC / 2
#define _WIMOS_1A_COEFICIENT_1 	 GRAVITY_MM_S2/(VCC_LOGIC/5)
#define _WIMOS_1A_OFFSET_2 	 (uint8_t)(0)
#define _WIMOS_1A_COEFICIENT_2 	 (uint8_t)(1)
#define _WIMOS_1A_OPERATOR_JOINT(x) 	 (x)
#define _WIMOS_1A_AVERAGE_SIZE 	 1
#define _WIMOS_1A_AVERAGE_OFFSET  	 0
#define _WIMOS_1A_OFFSET_MAX_SIZE 	 0
#define _WIMOS_1A_DETECTION(inputValue) 	 detectionA1Default(inputValue)
#undef _EN_WIMOS_PORT_A2 
#define _WIMOS_2A_OFFSET_1 	 - VCC_LOGIC / 2
#define _WIMOS_2A_COEFICIENT_1 	 GRAVITY_MM_S2/(VCC_LOGIC/5)
#define _WIMOS_2A_OFFSET_2 	 (uint8_t)(0)
#define _WIMOS_2A_COEFICIENT_2 	 (uint8_t)(1)
#define _WIMOS_2A_OPERATOR_JOINT(x) 	 (x)
#define _WIMOS_2A_AVERAGE_SIZE 	 1
#define _WIMOS_2A_AVERAGE_OFFSET  	 0
#define _WIMOS_2A_OFFSET_MAX_SIZE 	 0
#define _WIMOS_2A_DETECTION(inputValue) 	 detectionA2Default(inputValue)
#undef _EN_WIMOS_PORT_A3 
#define _WIMOS_3A_OFFSET_1 	 - VCC_LOGIC / 2
#define _WIMOS_3A_COEFICIENT_1 	 GRAVITY_MM_S2/(VCC_LOGIC/5)
#define _WIMOS_3A_OFFSET_2 	 (uint8_t)(0)
#define _WIMOS_3A_COEFICIENT_2 	 (uint8_t)(1)
#define _WIMOS_3A_OPERATOR_JOINT(x) 	 (x)
#define _WIMOS_3A_AVERAGE_SIZE 	 1
#define _WIMOS_3A_AVERAGE_OFFSET  	 0
#define _WIMOS_3A_OFFSET_MAX_SIZE 	 0
#define _WIMOS_3A_DETECTION(inputValue) 	 detectionA3Default(inputValue)
#undef _EN_WIMOS_PORT_A4 
#define _WIMOS_4A1_OFFSET_1 	 - VCC_LOGIC / 2
#define _WIMOS_4A1_COEFICIENT_1 	 GRAVITY_MM_S2/(VCC_LOGIC/5)
#define _WIMOS_4A1_OFFSET_2 	 (uint8_t)(0)
#define _WIMOS_4A1_COEFICIENT_2 	 (uint8_t)(1)
#define _WIMOS_4A2_OFFSET_1 	 - VCC_LOGIC / 2
#define _WIMOS_4A2_COEFICIENT_1 	 GRAVITY_MM_S2/(VCC_LOGIC/5)
#define _WIMOS_4A2_OFFSET_2 	 (uint8_t)(0)
#define _WIMOS_4A2_COEFICIENT_2 	 (uint8_t)(1)
#define _WIMOS_4A_OPERATOR_JOINT(x,y) 	 MATH_VECTOR2D_LENGHT(x,y)
#define _WIMOS_4A_AVERAGE_SIZE 	 1
#define _WIMOS_4A_AVERAGE_OFFSET  	 0
#define _WIMOS_4A_OFFSET_MAX_SIZE 	 0
#define _WIMOS_4A_DETECTION(inputValue) 	 detectionA4Default(inputValue)
#define _EN_WIMOS_PORT_A5 
#define _WIMOS_5A1_OFFSET_1 	 - VCC_LOGIC / 2
#define _WIMOS_5A1_COEFICIENT_1 	 GRAVITY_MM_S2/(VCC_LOGIC/5)
#define _WIMOS_5A1_OFFSET_2 	 (uint8_t)(0)
#define _WIMOS_5A1_COEFICIENT_2 	 (uint8_t)(1)
#define _WIMOS_5A2_OFFSET_1 	 - VCC_LOGIC / 2
#define _WIMOS_5A2_COEFICIENT_1 	 GRAVITY_MM_S2/(VCC_LOGIC/5)
#define _WIMOS_5A2_OFFSET_2 	 (uint8_t)(0)
#define _WIMOS_5A2_COEFICIENT_2 	 (uint8_t)(1)
#define _WIMOS_5A3_OFFSET_1 	 - VCC_LOGIC / 2
#define _WIMOS_5A3_COEFICIENT_1 	 GRAVITY_MM_S2/(VCC_LOGIC/5)
#define _WIMOS_5A3_OFFSET_2 	 (uint8_t)(0)
#define _WIMOS_5A3_COEFICIENT_2 	 (uint8_t)(1)
#define _WIMOS_5A_OPERATOR_JOINT(x,y,z) 	 MATH_VECTOR3D_LENGHT(x,y,z)
#define _WIMOS_5A_AVERAGE_SIZE 	 50
#define _WIMOS_5A_AVERAGE_OFFSET  	 120
#define _WIMOS_5A_OFFSET_MAX_SIZE 	 10
#define _WIMOS_5A_PROCESSOR_DET(lastValue, inputValue, currentIndex) 	 MATH_MOVING_AVERAGE(lastValue, inputValue, currentIndex, _WIMOS_5A_AVERAGE_SIZE)
#define _WIMOS_5A_PROCESSOR(lastValue, inputValue, currentIndex, sizeValue) 	 MATH_MOVING_AVERAGE(lastValue, inputValue, currentIndex, sizeValue)
#define _WIMOS_5A_DETECTION(inputValue) 	 detectionA5Default(inputValue)
#undef _EN_WIMOS_PORT_I2C
#define _WIMOS_PORT_I2C_CONFIG_SIZE 0
#define _WIMOS_PORT_I2C_DATA_SIZE 0
#define _WIMOS_I2C_CONFIG { \ 
 }
#define _WIMOS_I2C_DATA { \ 
 }
