/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2011
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
* ============================================================================
 Example to show the usage and performance of SRIO on KeyStone DSP
* =============================================================================
 *  Revision History
 *  ===============
 *  June 13, 2011 Brighton Feng   File Created
 * ============================================================================
 */
#ifndef _SRIO_TEST_H_
#define _SRIO_TEST_H_

#include "KeyStone_SRIO_init_drv.h"

#define DSP0_SRIO_BASE_ID	0xA0
#define DSP1_SRIO_BASE_ID	0xB0
#define DSP2_SRIO_BASE_ID	0xC0
#define DSP3_SRIO_BASE_ID	0xD0

#define SRIO_DEFAULT_TEST_SPEED	1.0

// #define SRIOForwardDataAddrDSP0 0xA0000000
// #define SRIOForwardDataAddrDSP1 (SRIOForwardDataAddrDSP0+0x00100000)
// #define SRIOForwardDataAddrDSP2 (SRIOForwardDataAddrDSP1+0x00100000)
// #define SRIOForwardDataAddrDSP3 (SRIOForwardDataAddrDSP2+0x00100000)
// #define SRIOPackDataAddr 	(SRIOForwardDataAddrDSP3+0x00100000)

#define SRIOForwardDataAddr 0xA0000000
#define SRIOPackDataAddr 	(SRIOForwardDataAddr+0x00300000)
#define SRIOTestDDRAddr		0xB0000000
#define SRIOTestSramAddr	0x0C100000

/*define the message convey by doorbell for this test*/
typedef enum
{
	SRIO_DOORBELL_MSG_LATENCY_TEST= 0,
	SRIO_DOORBELL_MSG_NREAD_DONE    ,
	SRIO_DOORBELL_MSG_NWRITE_DONE   ,
	SRIO_DOORBELL_MSG_NWRITE_R_DONE ,
	SRIO_DOORBELL_MSG_SWRITE_DONE   
}SRIO_Doorbell_Message;



#define DSP0 	(0u)
#define DSP1	(1u)
#define DSP2 	(2u)
#define DSP3	(3u)

//当前程序运行设备ID
#define DeviceId  DSP3
//#define door   0x8D0003



typedef struct 
{
	SRIO_Packet_Type packet_type;

	/*for directIO test, this is source address in memory;
	for message test, this is source queue number*/
	Uint32 source; 	

	/*for directIO test, this is destination address in memory;
	for message test, this is dest device ID, different dest ID map to
	different flow to different memory in this test*/
	Uint32 dest;

	Uint32 byteCount;
	
}SRIO_Transfer_Param;

typedef struct 
{
	SRIO_1x2x4x_Path_Control multiple_port_path;
	/*test parameters for 4 ports, set 0 for unused port*/
	SRIO_Transfer_Param transfer_param[4]; 	
}SRIO_Multiple_Test_Config;



typedef struct{
	uint32_t DevicesID[3];
	uint32_t SRIO_BASE_ID[3];
//	uint32_t InitStatus;
	uint32_t ForwardDataAddr[3];
	uint32_t LsuNum;	//0-7
	uint32_t RecvLocalPackNum;
	uint32_t SendDix;
}struct_SRIOInstance;


extern SerdesSetup_4links serdes_cfg;
extern SerdesLinkSetup serdesLinkSetup;
extern SRIO_Config srio_cfg;
extern volatile Uint32 uiDoorbell_TSC; 	//timestamp when doorbell interrupt happens

/* identify used ports lanes according to 1x 2x 4x path configuration*/
extern void srio_identify_used_ports_lanes(
	SRIO_1x2x4x_Path_Control srio_1x2x4x_path_control);

extern char * get_packet_type_string(SRIO_Packet_Type packetType);

/*fill data before test*/
extern void InitDataBuffer(Uint32 * uipSrc, Uint32 * uipDst, Uint32 uiByteCount);

/*verify data after test*/
extern void VerifyData(Uint32 * uipSrc, Uint32 * uipDst, Uint32 uiByteCount);

extern void InitLsuTransfer(SRIO_LSU_Transfer * lsuTransfer, 
	SRIO_Transfer_Param * transferParam, Uint32 uiLsuNum, 
	Uint32 uiDstID, Uint32 uiSrcIDMap);
	
extern int SrioSendData(	uint32_t LocalDeviceID,
					uint32_t DstDeviceId,
					uint32_t LocalAddr,
					uint32_t DstAddr,
					uint32_t DataLen		);

extern void SRIOInit(uint32_t DevId);

#endif
