/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!
 * @file APP_Demo.c
 * 
 * @brief: Add your description here for this file.
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section Rule_11-4 Rule: 11.4 (Advisory)
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and integer type.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 * @section Rule_11-6 Rule: 11.6 (Required)
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 * @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  Tomlin Tang  2020-12-29 09:17:00  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
 */

#include "APP_Demo.h"
//#include "includes.h"
#include "user_config.h"
#include "standardTypes.h"
#include "UDS_app.h"
#include "TP.h"
#include "FlexCan_Ip.h"
#include "FlexCAN_Ip_Sa_PBCfg.h"
#include "IntCtrl_Ip.h"
//#include "bootloader_debug.h"
//#include "timer_hal.h" replace by system task API
#include "watchdog_hal.h"
#include "fls_app.h"
//#include "boot.h"
/*******************************************************************************
 * User Include
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* User includes (#include below this line is not maintained by Processor Expert) */
#define CAN_MSG_TYPE (CAN_MSG_ID_STD)
#define RX_MAILBOX_ID (1u)
#define TX_MAILBOX_ID (2u)
Flexcan_Ip_MsgBuffType g_RXCANMsg;

extern void CAN0_ORED_0_31_MB_IRQHandler(void);
extern void Swt_Ip_Swt0_Isr(void);

extern void CAN0_ORED_0_31_MB_IRQHandler(void);
extern void Swt_Ip_Swt0_Isr(void);
const Flexcan_Ip_DataInfoType RXCANMsgConfig = 
{
    .msg_id_type = FLEXCAN_MSG_ID_STD,
    .data_length = 8u,
    .is_polling = FALSE,
    .is_remote = FALSE
};

Flexcan_Ip_DataInfoType TXCANMsgConfig = 
{
    .msg_id_type = FLEXCAN_MSG_ID_STD,
    .data_length = 8u,
    .is_polling = FALSE,
    .is_remote = FALSE
};

/**
 * @brief 
 * 
 * @param instance 
 * @param eventType 
 * @param buffIdx 
 * @param flexcanState 
 */
void flexcan0_Callback(uint8 instance,Flexcan_Ip_EventType eventType, 
                      uint32 buffIdx,const Flexcan_Ip_StateType * flexcanState)
{
    
    if(FLEXCAN_EVENT_RX_COMPLETE == eventType)
    {   
        TP_DriverWriteDataInTP(g_RXCANMsg.msgId, g_RXCANMsg.dataLen, g_RXCANMsg.data);
        FlexCAN_Ip_Receive(INST_FLEXCAN_0, RX_MAILBOX_ID, &g_RXCANMsg, FALSE);
    }
    else if(FLEXCAN_EVENT_TX_COMPLETE == eventType)
    {
        TP_DoTxMsgSuccesfulCallback();
    }
    else
    {}
}

static void BSP_Init(void);
void SendMsgMainFun(void);

/*FUNCTION**********************************************************************
 *
 * Function Name : APP_Demo_Init
 * Description   : APP Demo Init
 *END**************************************************************************/
void APP_Demo_Init(void)
{
	BSP_Init();
	//BOOTLOADER_DEBUG_Init(); use systemprint API
	//TIMER_HAL_Init(); replace by system task API
	UDS_Init();
	TP_Init();
    FLASH_APP_Init();

	//Boot_CheckDownlaodAPPStatus(); no use

	APP_DebugPrintf("Welcome enter S32K312 CAN/CAN FD(Nomial:500K, data:500K) APP demo!\r\n");

}

/*FUNCTION**********************************************************************
 *
 * Function Name : APP_Demo_MainFun
 * Description   : APP main function called by main; replace by system task API
 *END**************************************************************************/
// void APP_Demo_MainFun(void)
// {
// 	static uint16 ledCnt = 0u;
	
// 	if(TRUE == TIMER_HAL_Is1msTickTimeout())
// 	{
// 		TP_SystemTickCtl();

// 		UDS_SystemTickCtl();

// 		ledCnt++;
// 	}

// 	if(ledCnt >= 100u)
// 	{
// 		Debug_ToggleDebugIO();
		
// 		ledCnt = 0u;
// 	}

// 	TP_MainFun();

// 	UDS_MainFun();

// 	if(TRUE == TIMER_HAL_Is100msTickTimeout())
// 	{
// 		WATCHDOG_HAL_Fed();
// 	}

// 	SendMsgMainFun();
// }

/*Send msg main function*/
void SendMsgMainFun(void)
{
	Flexcan_Ip_MsgBuffType txMsg;
	Flexcan_Ip_StatusType status = FLEXCAN_STATUS_SUCCESS;
    uint32 dataLen;
    status = FlexCAN_Ip_GetTransferStatus(INST_FLEXCAN_0, TX_MAILBOX_ID);
	if(FLEXCAN_STATUS_SUCCESS == status)
	{
		if(TRUE == TP_DriverReadDataFromTP(sizeof(txMsg.data), txMsg.data, &(txMsg.msgId), &(dataLen)))
		{
            TXCANMsgConfig.data_length = dataLen;
            FlexCAN_Ip_Send(INST_FLEXCAN_0, TX_MAILBOX_ID, &TXCANMsgConfig, txMsg.msgId, txMsg.data);
		}
	}
}

static void BSP_Init(void)
{
#if 0
    Clock_Ip_Init(&Mcu_aClockConfigPB[0]);
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0,g_pin_mux_InitConfigArr0);
    IntCtrl_Ip_Init(&IntCtrlConfig_0);
    IntCtrl_Ip_ConfigIrqRouting(&intRouteConfig);
    IntCtrl_Ip_EnableIrq(FlexCAN0_1_IRQn);
	IntCtrl_Ip_InstallHandler(FlexCAN0_1_IRQn, CAN0_ORED_0_31_MB_IRQHandler, NULL_PTR);
    IntCtrl_Ip_EnableIrq(LPUART0_IRQn);
    IntCtrl_Ip_InstallHandler(LPUART0_IRQn, LPUART_UART_IP_0_IRQHandler, NULL_PTR);
    IntCtrl_Ip_InstallHandler(SWT0_IRQn, Swt_Ip_Swt0_Isr, NULL_PTR);
	FlexCAN_Ip_Init(INST_FLEXCAN_0, &FlexCAN_State0, &FlexCAN_Config0);
	FlexCAN_Ip_SetStartMode(INST_FLEXCAN_0);
	FlexCAN_Ip_ConfigRxMb(INST_FLEXCAN_0, RX_MAILBOX_ID, &RXCANMsgConfig, RX_PHY_ID);
	FlexCAN_Ip_Receive(INST_FLEXCAN_0, RX_MAILBOX_ID, &g_RXCANMsg, FALSE);
#else
    //Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    //Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0,g_pin_mux_InitConfigArr0);
//    IntCtrl_Ip_Init(&IntCtrlConfig_0);
//    IntCtrl_Ip_ConfigIrqRouting(&intRouteConfig);
    IntCtrl_Ip_EnableIrq(FlexCAN0_1_IRQn);
	IntCtrl_Ip_InstallHandler(FlexCAN0_1_IRQn, CAN0_ORED_0_31_MB_IRQHandler, NULL_PTR);
//    IntCtrl_Ip_EnableIrq(LPUART6_IRQn);
//    IntCtrl_Ip_InstallHandler(LPUART6_IRQn, LPUART_UART_IP_6_IRQHandler, NULL_PTR);
    IntCtrl_Ip_InstallHandler(SWT0_IRQn, Swt_Ip_Swt0_Isr, NULL_PTR);
	FlexCAN_Ip_Init(INST_FLEXCAN_0, &FlexCAN_State0, &FlexCAN_Config0);
	FlexCAN_Ip_SetStartMode(INST_FLEXCAN_0);
	FlexCAN_Ip_ConfigRxMb(INST_FLEXCAN_0, RX_MAILBOX_ID, &RXCANMsgConfig, RX_PHY_ID);
	FlexCAN_Ip_Receive(INST_FLEXCAN_0, RX_MAILBOX_ID, &g_RXCANMsg, FALSE);

#endif
	WATCHDOG_HAL_Init();
}

/******************************************************************************
 * EOF
 *****************************************************************************/
