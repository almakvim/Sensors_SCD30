
#include "string.h"
#include "mk_conf_tree.h"
#include "setup.h"
#include "system.h"
#include "USART.h"
#include "scd30_i2c.h"
#include "control.h"

VAR_PAR dev_var;
Uint32 timeout_shift = 0;
Uint32 timeout_type = 0;
//============================================================================
uint8_t Control_prop( uint8_t par, uint8_t * name, uint8_t * prop, uint8_t * nbyte )
{
	char * str;
    uint8_t size = 0;
	if( name ) { switch( par )
        {
        case CONTROL_CO2        :	str ="Концентрация CO2 (ppm)"; break;
        case CONTROL_TEMP       :	str ="Температура (oC)"; break;
        case CONTROL_HUMID      :	str ="Влажность (%)"; break;
        case CONTROL_ASC        :	str ="Автокалибровка уст-на"; break;
        case CONTROL_FRC        :	str ="Быстрая калиб-ка (ppm)"; break;
        case CONTROL_OFFSET     :	str ="Смещение темп-ры (oCx100)"; break;
        case CONTROL_ALTITU     :	str ="Компенсация высоты (м)"; break;
        case CONTROL_INTERV     :	str ="Уст-ть интервал (2..1800)"; break;
        case CONTROL_SOFRST     :	str ="Мягкий сброс"; break;
        default: return 0;
        }
        while( *str ) { *name++ = *str++; size++; } *name = 0; 
    }
	if( prop ) switch( par )
        {
        case CONTROL_SOFRST:
        case CONTROL_ASC:       *prop = BOOL; break;
        case CONTROL_OFFSET:    
        case CONTROL_ALTITU:    
        case CONTROL_INTERV:    
        case CONTROL_FRC:    
        case CONTROL_HUMID:     *prop = UINT; break;
        case CONTROL_TEMP:      *prop = INT; break;
        case CONTROL_CO2:	    *prop = UINT | RO; break;
        default: return 0;
        }
	if( nbyte ) switch( par )
        {
        case CONTROL_SOFRST:
        case CONTROL_ASC:       *nbyte = 1; break;
        case CONTROL_OFFSET:    
        case CONTROL_ALTITU:    
        case CONTROL_INTERV:    
        case CONTROL_FRC:    
        case CONTROL_HUMID:     *nbyte = 2; break;
        case CONTROL_TEMP:      *nbyte = 2; break;
        case CONTROL_CO2:	    *nbyte = 2; break;
        default: return 0;
        }
    return size;
}
//============================================================================
void Control_get(uint8_t par, void * value)
{
    switch( par )
    {
        case CONTROL_CO2:	    *(u16*)value = dev_var.co2;  break;
        case CONTROL_TEMP:	    *(s16*)value = dev_var.temp;  break;
        case CONTROL_HUMID:	    *(u16*)value = dev_var.hum;  break;
        case CONTROL_ASC:	    *(u8*)value = dev_var.set_ASC;  break;
        case CONTROL_FRC:	    *(u16*)value = dev_var.set_FRC;  break;
        case CONTROL_OFFSET:	*(u16*)value = dev_var.set_offset;  break;
        case CONTROL_ALTITU:	*(u16*)value = dev_var.set_altitude;  break;
        case CONTROL_INTERV:	*(u16*)value = dev_var.set_interval;  break;
        case CONTROL_SOFRST:	*(u8*)value = dev_var.soft_rst;  break;
        default:;
    }
}
//============================================================================
void Control_set(uint8_t par, void * value)
{
	int16_t i = *(u16*)value;
	uint16_t u = *(u16*)value;
    
    switch( par )
    {
        case CONTROL_TEMP:	    Setup.sift_temp = dev_var.temp - i; break;
        case CONTROL_HUMID:	    Setup.sift_humd = dev_var.hum - i;  break;
        case CONTROL_ASC:	    scd30_set_ASC(i); break;
        case CONTROL_FRC:       if((u >= 400) && (u <= 2000)){
                                scd30_set_FRC( u );
                                } break;
        case CONTROL_OFFSET:    if(u){
                                scd30_set_temp_offset( u );
                                } break;
        case CONTROL_ALTITU:    if(u){
                                scd30_set_altitude_comp( u );
                                } break;
        case CONTROL_INTERV:    if((u >= 2) && (u <= 1800)){
                                scd30_set_interval( u );
                                } break;
        case CONTROL_SOFRST:    if(u){
                                dev_var.soft_rst = 1;
                                dev_var.start1 = 0;
                                scd30_set_soft_reset();
                                } break;
        default:;
    }
}
//============================================================================
uint8_t Params_prop( uint8_t par, uint8_t * name, uint8_t * prop, uint8_t * nbyte )
{
	char * str;
    uint8_t size = 0;
	if( name ) {switch( par )
        {
        case PARAM_SFT_TEMP   :	str ="Смещение температуры"; break;
        case PARAM_SFT_HUMID  :	str ="Смещение влажности"; break;
        default: return 0;
        }
        while( *str ) { *name++ = *str++; size++; } *name = 0; 
    }
	if( prop ) switch( par )
        {
        case PARAM_SFT_TEMP   :
        case PARAM_SFT_HUMID  :	*prop = INT; break;
        default: return 0;
        }
	if( nbyte ) switch( par )
        {
        case PARAM_SFT_TEMP   :
        case PARAM_SFT_HUMID  :	*nbyte = 2; break;
        default: return 0;
        }
    return size;
}
//============================================================================
void Params_get(uint8_t par, void * value)
{
    switch( par )
    {
		case PARAM_SFT_TEMP :	*(u16*)value = Setup.sift_temp; break;
		case PARAM_SFT_HUMID :	*(u16*)value = Setup.sift_humd; break;

        default:;
    }
}
//============================================================================
void Params_set(uint8_t par, void * value)
{
    s16 var = *(s16*)value;
    switch( par )
    {
        case PARAM_SFT_TEMP:    Setup.sift_temp = var; break;
        case PARAM_SFT_HUMID:   Setup.sift_humd = var; break;

        default:;
    }
}
//=========================================================================
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//    if(GPIO_Pin == RDY_Pin) 
//    {
//        if(HAL_GPIO_ReadPin(RDY_GPIO_Port, RDY_Pin))
//        {
//            if(scd30_get_ready_status())
//            {
//                scd30_read_measurement();
//                dev_var.set_ASC = scd30_get_ASC();
//            }
//        }
//    }
}
//=========================================================================
void sdc30_get_proc(void)
{
    dev_var.set_ASC = scd30_get_ASC();
    dev_var.set_FRC = scd30_get_FRC();
    dev_var.set_interval = scd30_get_interval();
    dev_var.set_offset = scd30_get_temp_offset();
    dev_var.set_altitude = scd30_get_altitude_comp();
}
//=========================================================================
// Функция обработки
void CONTROL_Proc(void)
{
    static uint16_t count_get = 0;
    
    if(dev_var.start1 == 0)
    {
        timeout_shift = 500;
        timeout_type = HAL_GetTick();

        HAL_Delay(1000);

        dev_var.start1 = 0;
        
        if(scd30_set_start_continuous())
        {
            dev_var.start1 = 1;
            dev_var.soft_rst = 0;
            sdc30_get_proc();
            scd30_read_measurement();
        }
        return;
    }
        
	if(timeout_type+timeout_shift <= HAL_GetTick())
	{
        if(HAL_GPIO_ReadPin(RDY_GPIO_Port, RDY_Pin))
        {
            if(scd30_get_ready_status())
            {
                scd30_read_measurement();
                if(count_get > 20){
                    count_get = 0;
                    sdc30_get_proc();
                } else count_get++;
            }
        }
        timeout_shift = 500;
        timeout_type = HAL_GetTick();
        
	}
}
//=========================================================================

