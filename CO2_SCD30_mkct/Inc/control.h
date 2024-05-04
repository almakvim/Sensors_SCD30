
#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"
#include "system.h"
#include "USART.h"

// ���������
#define VARR_STATE      0      // R: ���������
//--------------------------------------------------
//#define CMD_PAR         20  // 0x14: �������� ��������� (����� �� ������)
#define CMD_SET_PAR     21  // 0x15: ��������� ���������
#define CMD_GET_PAR     22  // 0x16: ������ �������� ���������

#define CMD_RESET		0		// ����� �������� CMD
//--------------------------------------------------
#define STATUS_NULL  	    0x0000	// Null
#define STATUS_DEV_OK    	0x0001	// ������ � ������� ���������
#define STATUS_CAN_MESS 	0x0002	// �������� ����������� �� CAN
#define STATUS_CALC_PAR  	0x0004	// �������� �������������
#define STATUS_EN_OK      	0x0008	// ������ �����

#define STATUS_SNTC3   	    0x0010	// �������� SNTC3
//--------------------------------------------------
#define PAR_STATE  		0x0001	//0 ������ ��������� 0


// ���������� ��������
enum CONTROL_VAR
{
	CONTROL_CO2,			// 
	CONTROL_TEMP,			// 
	CONTROL_HUMID,			// 
	CONTROL_ASC,			// 
	CONTROL_FRC,			// 
	CONTROL_OFFSET, 		// 
	CONTROL_ALTITU, 		// 
	CONTROL_INTERV, 		// 
	CONTROL_SOFRST, 		// 

	CONTROL_VAR_NUM    		// ���������� ����������
};
// ���������� ��������
enum PARAM_VAR
{
	PARAM_SFT_TEMP,			// Bright LED
	PARAM_SFT_HUMID,		// Bright LED

	PARAM_VAR_NUM    		// ���������� ����������
};

#pragma pack (1)
typedef struct
{
    u16 start1;
    u16 start2;
	u16	set_ASC;
	u16	set_FRC;
	u16	set_offset;
	u16	set_altitude;
	u16	set_interval;
	u16 soft_rst;   
	u16 co2;   
	s16 temp;   
	s16 hum;   
	u32 realtime;   
	u32 count_pkt_in;   
	u32 count_pkt_out;   
	u32 count_pkt_err;
}
VAR_PAR;
#pragma pack(4)

extern VAR_PAR dev_var;

//----------------------------------------------------------------
uint8_t Control_prop(uint8_t par, uint8_t * name, uint8_t * prop, uint8_t * nbyte);
void Control_get(uint8_t par, void * value);
void Control_set(uint8_t par, void * value);
//----------------------------------------------------------------
uint8_t Params_prop(uint8_t par, uint8_t * name, uint8_t * prop, uint8_t * nbyte);
void Params_get(uint8_t par, void * value);
void Params_set(uint8_t par, void * value);
//----------------------------------------------------------------

u8 calc_CS(u8 * addr, u16 size);
u16 conv_baud_uint(u16 baud);

// ������� ���������
void CONTROL_Proc(void);

#endif // __CONTROL_H
