/**
  ******************************************************************************
  * @file    scd30_i2c.c
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"
#include "system.h"
#include "control.h"
#include "setup.h"
#include "scd30_i2c.h"

//==============================================================================
static uint8_t crc8_table[] = {

	0,   49,  98,  83,  196, 245, 166, 151, 185, 136, 219, 234, 125, 76,  31,  46,
	67,  114, 33,  16,  135, 182, 229, 212, 250, 203, 152, 169, 62,  15,  92,  109,
	134, 183, 228, 213, 66,  115, 32,  17,  63,  14,  93,  108, 251, 202, 153, 168,
	197, 244, 167, 150, 1,   48,  99,  82,  124, 77,  30,  47,  184, 137, 218, 235,
	61,  12,  95,  110, 249, 200, 155, 170, 132, 181, 230, 215, 64,  113, 34,  19,
	126, 79,  28,  45,  186, 139, 216, 233, 199, 246, 165, 148, 3,   50,  97,  80,
	187, 138, 217, 232, 127, 78,  29,  44,  2,   51,  96,  81,  198, 247, 164, 149,
	248, 201, 154, 171, 60,  13,  94,  111, 65,  112, 35,  18,  133, 180, 231, 214,
	122, 75,  24,  41,  190, 143, 220, 237, 195, 242, 161, 144, 7,   54,  101, 84,
	57,  8,   91,  106, 253, 204, 159, 174, 128, 177, 226, 211, 68,  117, 38,  23,
	252, 205, 158, 175, 56,  9,   90,  107, 69,  116, 39,  22,  129, 176, 227, 210,
	191, 142, 221, 236, 123, 74,  25,  40,  6,   55,  100, 85,  194, 243, 160, 145,
	71,  118, 37,  20,  131, 178, 225, 208, 254, 207, 156, 173, 58,  11,  88,  105,
	4,   53,  102, 87,  192, 241, 162, 147, 189, 140, 223, 238, 121, 72,  27,  42,
	193, 240, 163, 146, 5,   52,  103, 86,  120, 73,  26,  43,  188, 141, 222, 239,
	130, 179, 224, 209, 70,  119, 36,  21,  59,  10,  89,  104, 255, 206, 157, 172
};
//==============================================================================
uint8_t crc_8( uint8_t *data, uint16_t len ) {

	size_t a;
	uint8_t crc;
	uint8_t *ptr;

	crc = 0xff;
	ptr = data;

	if ( ptr != NULL ) for (a=0; a<len; a++) {

		crc = crc8_table[(*ptr++) ^ crc];
	}

	return crc;

}  /* crc_8 */
//==============================================================================
float convert_data_to_float(uint8_t * data)
{
    uint32_t tmp = (uint32_t)((uint32_t)data[0]<<24 | 
                            (uint32_t)data[1]<<16 | 
                             (uint32_t)data[3]<<8 | 
                              (uint32_t)data[4]);
    return *(float*)&tmp;
}
//=========================================================================
s32 aver_vdev(float arg)
{
    s32 sing = 1;
    if(arg<0)
    {
        arg *= -1.0f;
        sing = -1;
    }
	u32 tmp1 = (u32)arg;
	u32 tmp10 = (u32)(arg*10.0f);
	u32 tmp100 = (u32)(arg*100.0f);
	u32 tmp1000 = (u32)(arg*1000.0f);
	u32 tmp = tmp1000 - (tmp100*10);

	if(tmp>=5) tmp100++;
	tmp = tmp100 - (tmp10*10);
	if(tmp>=5) tmp10++;
	tmp = tmp10 - (tmp1*10);
	if(tmp>=5) tmp1++;
    

	return tmp1*sing;
}
//==============================================================================
uint16_t scd30_set_stop_measurements(void)
{
    uint16_t command = STOP_CONT_MEAS;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, (uint8_t*)&command, 2, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }

    return 1;
}
//==============================================================================
uint16_t scd30_set_soft_reset(void)
{
    uint16_t command = SET_SOFT_RESET;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, (uint8_t*)&command, 2, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }

    return 1;
}
//==============================================================================
uint16_t scd30_set_interval(uint16_t inter)
{
    uint8_t inter_1 = inter;
    uint8_t inter_2 = inter>>8;

    uint8_t data[5] = { 0x46, 0x00, inter_2, inter_1, 0x00 };
    
    uint8_t crc = crc_8(&data[2], 2);
    data[4] = crc;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, data, 5, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }

    return 1;
}
//==============================================================================
uint16_t scd30_get_interval(void)
{
    uint8_t data[3];
    uint16_t command = GET_MEAS_INTERV;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, (uint8_t*)&command, 2, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }
    res = HAL_I2C_Master_Receive(&hi2c1, SCD30_ADDR_READ, (uint8_t*)data, 3, HAL_MAX_DELAY);
    if (res != HAL_OK) 
    {
        return 0;
    }

    uint16_t tmp = (uint16_t)((uint16_t)data[0]<<8 | (uint16_t)data[1]);

    return tmp;
}
//==============================================================================
uint16_t scd30_set_altitude_comp(uint16_t comp)
{
    uint8_t comp_1 = comp;
    uint8_t comp_2 = comp>>8;

    uint8_t data[5] = { 0x51, 0x02, comp_2, comp_1, 0x00 };
    
    uint8_t crc = crc_8(&data[2], 2);
    data[4] = crc;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, data, 5, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }

    return 1;
}
//==============================================================================
uint16_t scd30_get_altitude_comp(void)
{
    uint8_t data[3];
    uint16_t command = GET_ALTIT_COMPEN;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, (uint8_t*)&command, 2, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }
    res = HAL_I2C_Master_Receive(&hi2c1, SCD30_ADDR_READ, (uint8_t*)data, 3, HAL_MAX_DELAY);
    if (res != HAL_OK) 
    {
        return 0;
    }

    uint16_t tmp = (uint16_t)((uint16_t)data[0]<<8 | (uint16_t)data[1]);

    return tmp;
}
//==============================================================================
uint16_t scd30_set_temp_offset(uint16_t offset)
{
    uint8_t offs_1 = offset;
    uint8_t offs_2 = offset>>8;

    uint8_t data[5] = { 0x54, 0x03, offs_2, offs_1, 0x00 };
    
    uint8_t crc = crc_8(&data[2], 2);
    data[4] = crc;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, data, 5, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }

    return 1;
}
//==============================================================================
uint16_t scd30_get_temp_offset(void)
{
    uint8_t data[3];
    uint16_t command = GET_TEMP_OFFSET;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, (uint8_t*)&command, 2, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }
    res = HAL_I2C_Master_Receive(&hi2c1, SCD30_ADDR_READ, (uint8_t*)data, 3, HAL_MAX_DELAY);
    if (res != HAL_OK) 
    {
        return 0;
    }

    uint16_t tmp = (uint16_t)((uint16_t)data[0]<<8 | (uint16_t)data[1]);

    return tmp;
}
//==============================================================================
uint16_t scd30_set_FRC(uint16_t ppm)
{
    uint8_t frc_1 = ppm;
    uint8_t frc_2 = ppm>>8;

    uint8_t data[5] = { 0x52, 0x04, frc_2, frc_1, 0x00 };
    
    uint8_t crc = crc_8(&data[2], 2);
    data[4] = crc;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, data, 5, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }

    return 1;
}
//==============================================================================
uint16_t scd30_get_FRC(void)
{
    uint8_t data[3];
    uint16_t command = GET_FRC_ARGUMENT;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, (uint8_t*)&command, 2, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }
    res = HAL_I2C_Master_Receive(&hi2c1, SCD30_ADDR_READ, (uint8_t*)data, 3, HAL_MAX_DELAY);
    if (res != HAL_OK) 
    {
        return 0;
    }

    uint16_t tmp = (uint16_t)((uint16_t)data[0]<<8 | (uint16_t)data[1]);

    return tmp;
}
//==============================================================================
uint16_t scd30_set_ASC(uint8_t asc)
{
    uint8_t data[5] = { 0x53, 0x06, 0x00, asc, 0x00 };
    
    uint8_t crc = crc_8(&data[2], 2);
    data[4] = crc;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, data, 5, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }

    return 1;
}
//==============================================================================
uint16_t scd30_get_ASC(void)
{
    uint8_t data[3];
    uint16_t command = GET_ASC_STATUS;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, (uint8_t*)&command, 2, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }
    res = HAL_I2C_Master_Receive(&hi2c1, SCD30_ADDR_READ, (uint8_t*)data, 3, HAL_MAX_DELAY);
    if (res != HAL_OK) 
    {
        return 0;
    }

    uint16_t tmp = (uint16_t)((uint16_t)data[0]<<8 | (uint16_t)data[1]);

    return tmp;
}
//==============================================================================
uint16_t scd30_set_start_continuous(void)
{
    uint8_t data[5] = { 0x00, 0x10, 0x00, 0x00, 0x00 };

    uint8_t crc = crc_8(&data[2], 2);
    data[4] = crc;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, data, 5, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }

    return 1;
}
//==============================================================================
uint16_t scd30_get_ready_status(void)
{
    uint8_t data[3];
    uint16_t command = GET_DATA_READY;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, (uint8_t*)&command, 2, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }
    res = HAL_I2C_Master_Receive(&hi2c1, SCD30_ADDR_READ, (uint8_t*)data, 3, HAL_MAX_DELAY);
    if (res != HAL_OK) 
    {
        return 0;
    }

    uint16_t tmp = (uint16_t)((uint16_t)data[0]<<8 | (uint16_t)data[1]);

    return tmp;
}
//==============================================================================
uint16_t scd30_read_measurement(void)
{
    uint8_t data[18];
    uint16_t command = READ_MEASUREMENT;

    uint32_t res = HAL_I2C_Master_Transmit(&hi2c1, SCD30_ADDR_WRITE, (uint8_t*)&command, 2, HAL_MAX_DELAY);

    if (res != HAL_OK) 
    {
        return 0;
    }

    res = HAL_I2C_Master_Receive(&hi2c1, SCD30_ADDR_READ, (uint8_t*)data, 18, HAL_MAX_DELAY);
    if (res != HAL_OK) 
    {
        return 0;
    }
    float arg = convert_data_to_float(&data[0]);
    dev_var.co2 = aver_vdev(arg);
    arg = convert_data_to_float(&data[6]);
    dev_var.temp = aver_vdev(arg) - Setup.sift_temp;
    arg = convert_data_to_float(&data[12]);
    dev_var.hum = aver_vdev(arg) - Setup.sift_humd;
    
    return 1;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
