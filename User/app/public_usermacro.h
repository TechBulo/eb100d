

#ifndef		__PUBLIC_USER__

#define	__PUBLIC_USER__
#define     APP_MOTOR_CHANGE_SPEED_MACRO_ENABLE  1

#define	AUTO_PROTOCOL	0
#define PELCO_P     2 
#define PELCO_D     3      
#define PROTOCOL_CAM_LINK   4



#define	PELCO_D_FRAME_HEADE		0xFF
#define	PELCO_P_FRAME_HEADE		0xA0

#define	DOME_BAUDRATE_1200	1
#define	DOME_BAUDRATE_2400	2
#define	DOME_BAUDRATE_4800	3
#define	DOME_BAUDRATE_9600	4
#define	DOME_BAUDRATE_14400	5
#define	DOME_BAUDRATE_19200	6

#define	DOME_BAUDRATE_115200	7

#define	COMBUFF_LEFT	0X02
#define	COMBUFF_RIGHT	0X04
#define	COMBUFF_UP	0X03
#define	COMBUFF_DOWN	0X05

#define	ZOOM_TELE_CMD		COMBUFF_LEFT
#define	ZOOM_WIDE_CMD		COMBUFF_RIGHT
#define	FOCUS_FAR_CMD		COMBUFF_UP
#define	FOCUS_NEAR_CMD		COMBUFF_DOWN

#define	UART_SEND_OVERTIME		50000

enum ON_OFF_TYPE
{
    OFF_MODE,
    ON_MODE,
	AUTO_MODE,
	MANUAL_MODE,
};



#define VOLTAGE_VL1     GPIO_Pin_6
#define VOLTAGE_VL2     GPIO_Pin_5
#define VOLTAGE_VL3     GPIO_Pin_4

#define EEPROM_SYSTEM_PARA_FLAG_VAL    0x88

#define EEPROM_FACTORY_FLAG_VAL     0x58
#define EEPROM_FACTORY_FLAG2_VAL     0x85
#define EEPROM_FACTORY_FLAG_ADDRESS 100
#define EEPROM_FACTORY_FLAG2_ADDRESS (EEPROM_FACTORY_FLAG_ADDRESS+1)

#define EEPROM_DOME_ID_ADDRESS  (EEPROM_FACTORY_FLAG2_ADDRESS+1)
#define EEPROM_BAUDRATE_ADDRESS  (EEPROM_DOME_ID_ADDRESS+1)

#define EEPROM_SYSTEM_PARA_FLAG_ADDRESS    (EEPROM_BAUDRATE_ADDRESS+10)
#define EEPROM_SYSTEM_PARA_START_ADDRESS    (EEPROM_SYSTEM_PARA_FLAG_ADDRESS+10)

#define	EEPROM_SHUTTER_THREASHOLD_ADDRESS	1

#define	IRIS_LENS_STEPS_MAX		100

#endif
