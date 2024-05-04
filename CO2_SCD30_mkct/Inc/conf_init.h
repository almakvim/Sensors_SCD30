
#ifndef __conf_init_H
#define __conf_init_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "system.h"
#include "control.h"

typedef uint8_t (*func_prop)(uint8_t par, uint8_t * name, uint8_t * prop, uint8_t * nbyte);
typedef void (*func_cmd)(uint8_t par, void * value);

enum UNIT_NUM   // Parent enum
{
	MAIN_UNIT,			
	SYSTEM_UNIT,				
	CONTROL_UNIT,			
	PARAM_UNIT,				
	N_UNIT,     // Number of units (nodes)				
};

#pragma pack (1)
//-------------------------------------------------------
const char *namesUnit[] =	
{
	"CO2_SCD30_103",        // name_unit
	"System",               // name_unit
	"Control",              // name_unit
	"Parameters",           // name_unit
};
//--------------- Variant 1 ------------------------------
const u8 unitProp[][2] =	// Node tree structure 
{
    {MAIN_UNIT, 0},                 //  {parent, num_params },
    {MAIN_UNIT, SYSTEM_VAR_NUM},    //  {parent, num_params },
    {MAIN_UNIT, CONTROL_VAR_NUM},   //  {parent, num_params },
    {MAIN_UNIT, PARAM_VAR_NUM},     //  {parent, num_params },
};
// Tree structure:
// CO2_SCD30_103-|
//               |-System
//               |-Control
//               |-Parameters

//--------------- Variant 2 ------------------------------
//const u8 unitProp[][2] =	// Node tree structure 
//{
//    {MAIN_UNIT, 0},                  //  {parent, num_params },
//    {MAIN_UNIT, SYSTEM_VAR_NUM},     //  {parent, num_params },
//    {SYSTEM_UNIT, CONTROL_VAR_NUM},  //  {parent, num_params },
//    {SYSTEM_UNIT, PARAM_VAR_NUM},    //  {parent, num_params },
//};
// Tree structure:
// CO2_SCD30_103-|
//               |-System-|
//                        |-Control
//                        |-Parameters
//-------------------------------------------------------
func_prop Func_prop[] =		
{
    0,
	System_prop,	
	Control_prop,
	Params_prop	
};
//-------------------------------------------------------
func_cmd Func_get[] =		
{
    0,
	System_get,	
	Control_get,
	Params_get	
};
//-------------------------------------------------------
func_cmd Func_set[] =		
{
    0,
	System_set,	
	Control_set,
	Params_set	
};
//-------------------------------------------------------

#pragma pack (4)

#ifdef __cplusplus
}
#endif
#endif /*__conf_init_H */

