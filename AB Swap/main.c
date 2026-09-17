/*==================================================================================================
*  Project              : WCT-PPAUTOS2B
*  Platform             : ARM
*  Peripheral           :
*  Dependencies         : MWCT2XXXS
*
*  Copyright 2023-2024 NXP
*  NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used
*  strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading,
*  installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree
*  to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms,
*  then you may not retain, install, activate or otherwise use the software.
==================================================================================================*/


#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                                        INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 ==================================================================================================*/
#include "Wct_SystemHWInit.h"
#include "Wct_SystemTasker.h"

#include "Wct.h"
#include "Wct_DrvLpuart.h"

// Include for test purpose only
#include "wct_SystemPrint.h"
#include "Wct_DrvDcdc.h"
#include "Wct_HalCoil.h"
#include "Wct_DrvCmp.h"
#include "Wct_HalDdm.h"
#include "Wct_HalCoil.h"
#include "Wct_HalTiming.h"
#include "Wct_HalMisc.h"
#include "Wct_Lib.h"

#include "APP_Demo.h"
#include "TP.h"
#include "uds_app.h"
#include "MemAcc.h"
#include "fls_app.h"
#include "CRC_hal.h"
#include "crypto_hal.h"

#if PROPRIETARY_PROTOCOL_SUPPORTED
#include "Wct_HalPp.h"
#endif

#if (FREEMASTER_SUPPORTED == TRUE)
#include "freemaster.h"
#endif

/*==================================================================================================
 *                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 ==================================================================================================*/

/*==================================================================================================
 *                                       LOCAL MACROS
 ==================================================================================================*/

/*==================================================================================================
 *                                      LOCAL CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                      LOCAL VARIABLES
 ==================================================================================================*/
volatile Std_ReturnType LibStatus = E_OK;
/*==================================================================================================
 *                                      GLOBAL CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                      GLOBAL VARIABLES
 ==================================================================================================*/

/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 ==================================================================================================*/

/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 ==================================================================================================*/

/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 ==================================================================================================*/

/**
 * @brief        Main function of the example
 * @details      wireless charging demo main function example code
 */

int main(void)
{
	SampleAppInitTask();
    APP_Demo_Init();
	
	const uint8 *myKey = "myKey0123456789A";

	CRYPTO_HAL_SetKey(myKey, AES_KEY_LEN);

	/*
	=========================================TEST START=========================================
	const uint8 *myKey = "myKey0123456789A";
	const uint8 *myData = "myData";
	uint8 outMAC[16];

	CRYPTO_HAL_SetKey(myKey, AES_KEY_LEN);

	CRYPTO_HAL_GetMAC(myData, 6, &outMAC);

	for(uint8 i=0; i<16; ++i)
	{
		SPRT_PrintWordValueInHex(outMAC[i]);
	}
	SPRT_PrintString("\r\n");

	uint8 testData[8] = {0xA5, 0x5A, 0xA5, 0x5A, 0x01, 0x02, 0x03, 0x04};
	uint16 res = 0;
	CRC_HAL_CreatHardwareCrc(&testData, 8, &res);
	for(uint8 i=0; i<2; i++) SPRT_PrintHexChar(res>>(8*i));
	SPRT_PrintString("\r\n");

    HSE_FirmwareUpdate(0);
	HSE_ActivatePassiveBlock(0);

	MemAcc_Erase(MEMACC_ADDRESS_AREA_1_ID, 0, 0x2000);
	while (MEMACC_JOB_IDLE != MemAcc_GetJobStatus(MEMACC_ADDRESS_AREA_1_ID))
	{
	MemAcc_MainFunction();
	}

	uint8 flashData[8] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};
	MemAcc_Write(MEMACC_ADDRESS_AREA_1_ID, 0, &flashData, 8);
	while (MEMACC_JOB_IDLE != MemAcc_GetJobStatus(MEMACC_ADDRESS_AREA_1_ID))
	{
	MemAcc_MainFunction();
	}
	
	MemAcc_Erase(MEMACC_ADDRESS_AREA_0_ID, 0x4000, 0x2000);
	while (MEMACC_JOB_IDLE != MemAcc_GetJobStatus(MEMACC_ADDRESS_AREA_0_ID))
	{
	MemAcc_MainFunction();
	}

	uint8 flashData[8] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};
	MemAcc_Write(MEMACC_ADDRESS_AREA_0_ID, 0x4000, &flashData, 8);
	while (MEMACC_JOB_IDLE != MemAcc_GetJobStatus(MEMACC_ADDRESS_AREA_0_ID))
	{
	MemAcc_MainFunction();
	}
	 =========================================TEST END=========================================
	 */

	 /* comment up for test only!
	if (Wct_CheckFirstInit() != TRUE)
	{
		uint8 byDeviceId;

		for(byDeviceId = 0U; byDeviceId < WCT_NUM_DEVICES; byDeviceId++)
		{
			WCT_Stop(byDeviceId);
		}

		App_SetErr(APP_NVM_ERROR);

		while(1)
		{
#if (FREEMASTER_SUPPORTED == TRUE)
			FMSTR_Poll();
#endif
			HAL_KickWatchdog();
#if (CONSOLE_SUPPORTED == TRUE)
		    Wct_Lpuart_UartConsSendData();
#endif
		}
	}
	else
	{
		LibStatus = Wct_Start();
	}
*/
	/* Main loop*/
	while (1)
	{
	    SampleAppTaskScheduler();
        TP_MainFun();
 		UDS_MainFun();
        SendMsgMainFun();
		Flash_OperateMainFunction();
		HAL_KickWatchdog();
#if (CONSOLE_SUPPORTED == TRUE)
		Wct_Lpuart_UartConsSendData();
#endif

#if (FREEMASTER_SUPPORTED == TRUE)
        FMSTR_Poll();
#endif
	}

	return 0U;
}




#ifdef __cplusplus
}
#endif

/** @} */
