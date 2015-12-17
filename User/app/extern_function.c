#include "includes.h"


enum IRIS_CTL_MODE_TYPE iris_ctl_mode = IRIS_CAM_IRIS;


void shutter_threshold_set(void)
{
	eeprom_byte_write(EEPROM_SHUTTER_THREASHOLD_ADDRESS,shutter_val_auto_iris_level);

}

void shutter_threshold_get(void)
{
	shutter_val_auto_iris_level = eeprom_byte_read(EEPROM_SHUTTER_THREASHOLD_ADDRESS);

	if(shutter_val_auto_iris_level > 3)
		shutter_val_auto_iris_level = 1;

}


u8 ttst,ttst2;
void save_domeID(u8 val)
{
    eeprom_byte_write(EEPROM_DOME_ID_ADDRESS,val);

    ttst = eeprom_byte_read(EEPROM_DOME_ID_ADDRESS);
    
}

void get_domeID(void)
{
    u8 tmp;

    if((eeprom_byte_read(EEPROM_FACTORY_FLAG_ADDRESS) != EEPROM_FACTORY_FLAG_VAL)||(eeprom_byte_read(EEPROM_FACTORY_FLAG2_ADDRESS) != EEPROM_FACTORY_FLAG2_VAL))
    {
        eeprom_byte_write(EEPROM_DOME_ID_ADDRESS,1);
        eeprom_byte_write(EEPROM_BAUDRATE_ADDRESS,2);
        eeprom_byte_write(EEPROM_FACTORY_FLAG_ADDRESS,EEPROM_FACTORY_FLAG_VAL);
        eeprom_byte_write(EEPROM_FACTORY_FLAG2_ADDRESS,EEPROM_FACTORY_FLAG2_VAL);
        
        ttst = eeprom_byte_read(EEPROM_FACTORY_FLAG_ADDRESS);
        ttst2 = eeprom_byte_read(EEPROM_FACTORY_FLAG2_ADDRESS);
    }
    tmp = eeprom_byte_read(EEPROM_DOME_ID_ADDRESS);
    domeNo = tmp;
}

void save_baudrate(u8 val)
{
    Protocol_No = 0;
    
	uart0_init();
    
    eeprom_byte_write(EEPROM_BAUDRATE_ADDRESS,val);
}

void get_baudrate(void)
{
    u8 tmp;
    tmp = eeprom_byte_read(EEPROM_BAUDRATE_ADDRESS);
    if(tmp==0 ||tmp > 7)
        tmp = 2;
    Baud_rate = tmp;
    
}


u8 get_syspara_from_eeporm(u8 mode)
{
    u16 i;
    u16 len;
    u8 mode_tmp;
    
    mode_tmp = mode;
    if(!mode_tmp || mode_tmp > 16)
        mode_tmp = 1;

    len = (mode_tmp-1)*sizeof(system_para.system_para)+EEPROM_SYSTEM_PARA_START_ADDRESS;
    if(eeprom_byte_read(EEPROM_SYSTEM_PARA_FLAG_ADDRESS) != EEPROM_SYSTEM_PARA_FLAG_VAL)
        return 0;

    if(eeprom_byte_read(len) != 0x12 || eeprom_byte_read(len+1) != 0x34)
        return 0;

    
    for(i=0;i<sizeof(system_para.system_para);i++)
    {
        system_para.system_para_arr[i] = eeprom_byte_read(len+i);

    }

    return 1;
}

u8 set_syspara_to_eeporm(u8 mode)
{
    u16 i;
    u16 len;

    u8 mode_tmp;

    mode_tmp = mode;
    if(!mode_tmp || mode_tmp > 16)
        mode_tmp = 1;
    //len = sizeof(system_para.system_para_arr);


    len = (mode_tmp-1)*sizeof(system_para.system_para)+EEPROM_SYSTEM_PARA_START_ADDRESS;

    
    for(i=0;i<sizeof(system_para.system_para);i++)
    {
        eeprom_byte_write(len+i,system_para.system_para_arr[i]);

    }

    for(i=0;i<sizeof(system_para.system_para);i++)
    {
        if(eeprom_byte_read(len+i) != system_para.system_para_arr[i])
        {
            eeprom_byte_write(len, 0);
            break;
        }

    }
    eeprom_byte_write(EEPROM_SYSTEM_PARA_FLAG_ADDRESS, EEPROM_SYSTEM_PARA_FLAG_VAL);
    eeprom_byte_write(EEPROM_SYSTEM_PARA_FLAG_ADDRESS+1, mode_tmp);
    
    return 1;
}

//开机时调用，进行初始化
void load_system_para(void)
{
    u8 i;
    
    get_domeID();	
    get_baudrate();

    //i = eeprom_byte_read(EEPROM_SYSTEM_PARA_FLAG_ADDRESS+1);
    //if(i>16)
    //    i=1;
    
	//delay_X1ms(9000);
    //if(get_syspara_from_eeporm(i))
    if(get_syspara_from_eeporm(1))    
    {
        delay_X1ms(100);
        set_cam_para_all();
        delay_X1ms(200);
        if(system_para.system_para.para_ex_io_1_mode > 4)
            system_para.system_para.para_ex_io_1_mode = 1;

        if(system_para.system_para.para_ex_io_2_mode > 1)
            system_para.system_para.para_ex_io_2_mode = 1;
        if(system_para.system_para.para_ex_io_3_mode > 1)
            system_para.system_para.para_ex_io_3_mode = 1;
        
        extern_io_output(system_para.system_para.para_ex_io_1_mode-1);
        extern_io2_output(system_para.system_para.para_ex_io_2_mode);
        extern_io3_output(system_para.system_para.para_ex_io_3_mode);
        motor_lens_voltage_set(system_para.system_para.para_ex_lens_vol_mode);

		shutter_threshold_get();
    }
    else
    {
        system_para.system_para.para_ex_io_1_mode = 1;
        system_para.system_para.para_ex_io_2_mode = 1;
        system_para.system_para.para_ex_io_3_mode = 1;


    }
}

void load_system_para_by_mode(u8 mode)
{
    u8 i;
    
    get_domeID();	
    get_baudrate();
    
    if(get_syspara_from_eeporm(mode))
    {

                if(system_para.system_para.para_ex_io_2_mode > 1)
            system_para.system_para.para_ex_io_2_mode = 1;
        if(system_para.system_para.para_ex_io_3_mode > 1)
            system_para.system_para.para_ex_io_3_mode = 1;
        
        if(system_para.system_para.para_ex_io_1_mode > 4)
            system_para.system_para.para_ex_io_1_mode = 1;

        set_cam_para_all();
        extern_io_output(system_para.system_para.para_ex_io_1_mode-1);
        extern_io2_output(system_para.system_para.para_ex_io_2_mode);
        extern_io3_output(system_para.system_para.para_ex_io_3_mode);

        motor_lens_voltage_set(system_para.system_para.para_ex_lens_vol_mode);
		shutter_threshold_get();
    }
}



void save_system_para(u8 mode)
{
    u8 i;

    i = mode;
    if(!i || i>16)
        i=1;

    get_cam_para_all();
    set_syspara_to_eeporm(i);

}

//#define	
void extern_io_output(u8 mode)
{
	BitAction tmp1 = Bit_RESET,tmp2 = Bit_RESET;

    if(mode>8)
        return ;
	if(mode == 1 || mode == 3)
		tmp1 = Bit_SET;

	if(mode == 3 || mode == 2)
		tmp2 = Bit_SET;

	GPIO_WriteBit(GPIOB,GPIO_Pin_12,tmp1);
	GPIO_WriteBit(GPIOB,GPIO_Pin_13,tmp2);

    system_para.system_para.para_ex_io_1_mode = mode+1;
}

void extern_io2_output(u8 mode)
{

    if(mode==1 )
	{
        GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_SET);

    }
    else
       GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_RESET); 

    system_para.system_para.para_ex_io_2_mode = mode;

}

void extern_io3_output(u8 mode)
{

    if(mode==1 )
	{
        GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);

    }
    else
       GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET); 

    system_para.system_para.para_ex_io_3_mode = mode;

}

//设置镜头电机的驱动电流，电压
void motor_lens_voltage_set(u8 mode)
{
    switch(mode)
    {
    case 1:
        GPIO_ResetBits(GPIOB,VOLTAGE_VL3);	
        GPIO_ResetBits(GPIOB,VOLTAGE_VL2);	
    	GPIO_ResetBits(GPIOB,VOLTAGE_VL1);
        break;
    case 2:
        GPIO_ResetBits(GPIOB,VOLTAGE_VL3);	
        GPIO_ResetBits(GPIOB,VOLTAGE_VL2);	
    	GPIO_SetBits(GPIOB,VOLTAGE_VL1);
        break;
        
    case 3:
        GPIO_ResetBits(GPIOB,VOLTAGE_VL3);	
        GPIO_SetBits(GPIOB,VOLTAGE_VL2);	
    	GPIO_ResetBits(GPIOB,VOLTAGE_VL1);
        break;
        
    case 4:
        GPIO_ResetBits(GPIOB,VOLTAGE_VL3);	
        GPIO_SetBits(GPIOB,VOLTAGE_VL2);	
    	GPIO_SetBits(GPIOB,VOLTAGE_VL1);
        break;
        
    case 5:
        GPIO_SetBits(GPIOB,VOLTAGE_VL3);	
        GPIO_ResetBits(GPIOB,VOLTAGE_VL2);	
    	GPIO_ResetBits(GPIOB,VOLTAGE_VL1);
        break;
        
    case 6:
        GPIO_SetBits(GPIOB,VOLTAGE_VL3);	
        GPIO_ResetBits(GPIOB,VOLTAGE_VL2);	
    	GPIO_SetBits(GPIOB,VOLTAGE_VL1);
        break;
        
    case 7:
        GPIO_SetBits(GPIOB,VOLTAGE_VL3);	
        GPIO_SetBits(GPIOB,VOLTAGE_VL2);	
    	GPIO_ResetBits(GPIOB,VOLTAGE_VL1);
        break;
        
    case 8:
        GPIO_SetBits(GPIOB,VOLTAGE_VL3);	
        GPIO_SetBits(GPIOB,VOLTAGE_VL2);	
    	GPIO_SetBits(GPIOB,VOLTAGE_VL1);
        break;
        
    default:
        break;
        
    }
}


extern u8 iris_auto_manual_state;




void dome_func_control(uchar action,uchar prePoint)
{
    uchar k;

    switch (prePoint)
    {
    case 41:
	case 42:
	case 43:
	case 44:
		cam_zoom_data_set((prePoint-40)*100);

		break;
	case 50://关闭激光器
		if(action == 0x11)
		{
            cam_cross_line_mode_set(ON_MODE);
        }
        else
            cam_cross_line_mode_set(OFF_MODE);

		break;
	case 51://
		if(action == 0x11)
		{
            cam_nega_posi_mode_set(ON_MODE);
        }
        else
            cam_nega_posi_mode_set(OFF_MODE);

			break;	
	case 52://
    	if(action == 0x11)
		{
            cam_picture_reverse_set(PIC_MIRROR);
         }
        else
        {
            cam_picture_reverse_set(PIC_NORMAL);
        }
		break;
	case 53:
		if(action == 0x11)
		{
		cam_picture_reverse_set(PIC_VFLIP);
			}
		else
			cam_picture_reverse_set(PIC_NORMAL);
		break;
	case 54:
		if(action == 0x11)
		{
		cam_freeze_set(ON_MODE);
		}
		else
			{
			cam_freeze_set(OFF_MODE);
		}

        break;
        
    case 55:
		if(action == 0x11)
		{
		cam_color_bar_mode_set(ON_MODE);
			}
		else
			cam_color_bar_mode_set(OFF_MODE);
		break;
        
    case 60:
        if(action == 0x11)
		{
		cam_freeze_set(ON_MODE);
		}
		else
			{
			cam_freeze_set(OFF_MODE);
		}

        break;
	case 80:
	case 81:
	case 82:
	case 83:
		extern_io_output(prePoint-80);
		break;
		
	case 84:
        if(action == 0x11)
        {
            extern_io2_output(1);
        }
        else
            extern_io2_output(0);
        break;
	case 85:
        if(action == 0x11)
        {
            extern_io3_output(1);
        }
        else
            extern_io3_output(0);
        break;
	case 86:
	case 87:
		break;
    case 90:
    case 95:
		
		if(action == 0x11)
        {
        iris_ctl_mode = IRIS_CAM_OSD_ENTER;
        	osdSwitchFlag = 1;
        	//cam_osd_control(1);
			camera_osd_switch(0);
		}
		else
		{
			iris_ctl_mode = IRIS_CAM_IRIS;
			osdSwitchFlag = 0;
						//cam_osd_control(1);
			camera_osd_switch(1);


		}
        break;

	case 127:
		if(action == 0x11) // shutter manual, iris auto
		{
			lens_exposure_mode=1; //0,auto;1,shutter prior;2,iris prior
			iris_auto_manual_state = 0;

			/*下面设置快门的模式为手动，并返回当前的快门值			*/
			cam_shutter_mode_set(MANUAL_MODE);//auto,manual

			my_proto_call_preset(127,0,domeNo);

		}
		else
		{
			lens_exposure_mode=0; //0,auto;1,shutter prior;2,iris prior
			iris_auto_manual_state = 0;

			/*下面设置快门的模式为自动，		*/

			shutter_val_auto_iris_level = 2;
			shutter_threshold_set();
		}
		break;
	
	case 128:
		if(action == 0x11) // iris manual, shutter auto
		{
			lens_exposure_mode=2; //0,auto;1,shutter prior;2,iris prior
			iris_auto_manual_state = 1;
			
			
			
			cam_shutter_mode_set(AUTO_MODE);//auto,manual

			my_proto_call_preset(128,0,domeNo);
			
			/*下面设置快门的模式为自动，并返回当前的光圈值			*/
		}
		else
		{
			lens_exposure_mode=0; //0,auto;1,shutter prior;2,iris prior
			iris_auto_manual_state = 0;

			/*下面设置快门的模式为自动，		*/
			
			shutter_val_auto_iris_level = 1;
			shutter_threshold_set();
		}
		break;
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
    case 106:
    case 107:
    case 108:
        motor_lens_voltage_set(prePoint-100);
        break;

	case 111:
		cam_framerate_set(CAM_FRAMERATE_720P30);
	
		break;
	case 112:
		cam_framerate_set(CAM_FRAMERATE_1080P30);
		break;
	case 113:
			cam_framerate_set(CAM_FRAMERATE_720P60);
			break;
	case 114:
		cam_framerate_set(CAM_FRAMERATE_1080I60);
		break;
	case 115:
		cam_framerate_set(CAM_FRAMERATE_720P25);
	
		break;
	case 116:
		cam_framerate_set(CAM_FRAMERATE_1080P25);
		break;
	case 117:
			cam_framerate_set(CAM_FRAMERATE_720P50);
			break;
	case 118:
		cam_framerate_set(CAM_FRAMERATE_1080I50);
		break;
	case 110:
			cam_framerate_set(CAM_CVBS_NTSC);
			break;
	case 119:
		cam_framerate_set(CAM_CVBS_PAL);
		break;
    case 120:
        camera_sensor_format_set_with_long_key();
        break;
	//case 124:
	//case 125:
	//case 126:
	//		shutter_val_auto_iris_level = prePoint - 124;
	//		shutter_threshold_set();
	//	break;
    default:
        break;

    }

    if(prePoint>=201 && prePoint<=208)  
    {
        if(action == 0x11)
        {
            load_system_para_by_mode(prePoint-200);
            camera_power_on_off(0);
            delay_half_second(200);
            camera_power_on_off(1);
        }
        else
        {
            //save_system_para(prePoint-200);
        }
    }
    
    delay_half_ms(20);
    
}
