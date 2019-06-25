/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
******************************************************************************
* File Name    : R_PG_Timer_CMT_U1.h
* Version      : 1.00
* Description  : 
******************************************************************************
* History : 13.06.2019 Version Description
*         :   
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_pdl_cmt.h"
#include "r_pdl_cmt_RX62Txx.h"
#include "R_PG_IntFuncsExtern.h"

bool R_PG_Timer_Start_CMT_U1_C2(void);
bool R_PG_Timer_HaltCount_CMT_U1_C2(void);
bool R_PG_Timer_ResumeCount_CMT_U1_C2(void);
bool R_PG_Timer_GetCounterValue_CMT_U1_C2(uint16_t * counter_val);
bool R_PG_Timer_SetCounterValue_CMT_U1_C2(uint16_t counter_val);
bool R_PG_Timer_Start_CMT_U1_C3(void);
bool R_PG_Timer_HaltCount_CMT_U1_C3(void);
bool R_PG_Timer_ResumeCount_CMT_U1_C3(void);
bool R_PG_Timer_GetCounterValue_CMT_U1_C3(uint16_t * counter_val);
bool R_PG_Timer_SetCounterValue_CMT_U1_C3(uint16_t counter_val);
bool R_PG_Timer_StopModule_CMT_U1(void);



