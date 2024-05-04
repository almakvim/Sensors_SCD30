/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SCD30_I2C_H__
#define __SCD30_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define SCD30_ADDR_READ       (0x61 << 1) | 0x01
#define SCD30_ADDR_WRITE      (0x61 << 1)
    
#define MEM_ADDRESS           0x0036
#define START_CONT_MEAS       0x1000
#define STOP_CONT_MEAS        0x0401
#define SET_SOFT_RESET        0x04d3
#define GET_MEAS_INTERV       0x0046
#define GET_DATA_READY        0x0202
#define GET_ASC_STATUS        0x0653
#define GET_FRC_ARGUMENT      0x0452
#define GET_TEMP_OFFSET       0x0354
#define GET_ALTIT_COMPEN      0x0251
#define READ_MEASUREMENT      0x0003

uint16_t scd30_set_stop_measurements(void);
uint16_t scd30_set_start_continuous(void);
uint16_t scd30_get_ready_status(void);
uint16_t scd30_read_measurement(void);
uint16_t scd30_set_ASC(uint8_t asc);
uint16_t scd30_get_ASC(void);
uint16_t scd30_set_FRC(uint16_t ppm);
uint16_t scd30_get_FRC(void);
uint16_t scd30_set_temp_offset(uint16_t offset);
uint16_t scd30_get_temp_offset(void);
uint16_t scd30_set_altitude_comp(uint16_t comp);
uint16_t scd30_get_altitude_comp(void);
uint16_t scd30_set_soft_reset(void);
uint16_t scd30_set_interval(uint16_t inter);
uint16_t scd30_get_interval(void);

    
#ifdef __cplusplus
}
#endif

#endif /* __SCD30_I2C_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
