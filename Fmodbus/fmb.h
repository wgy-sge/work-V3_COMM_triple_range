/******************** (C) COPYRIGHT 2010 Zidetech *****************************
* File Name          : fmb.h
* Author             : xxx
* Version            : V0.1
* Date               : 04/26/2010
* Description        : fmodbus head
********************************************************************************/

#ifndef __FMB__H
#define __FMB__H

#include "port.h"

#include "fmb_proto.h"
#include "fmb_frame.h"
#include "fmb_crc.h"
#include "fmb_port.h"
#include "fmb_global.h"
#include "fmb_rtu.h"


#define    MODBUS_BAUD                       230400//115200
#define    MAIN_BOARD_DEVICE_ADDRESS         0xA0         //主机板设备地址 


#define  MODBUS_TIMEOUT_TICKS           2000     //通讯超时2000ms
#define  MODBUS_EVENT_FIFO_LENGTH          2     //事件队列长度
//#define  MODBUS_BAUD                   19200     //通讯速率


typedef bool (*pFuncCommType)( UCHAR *fram, USHORT len, eMBErrorCode err );
////////////////////////////////////////////////////////////////////////////////
//


eMBErrorCode eMBInitClient( void );

void eMBClientPoll( void );
BOOL request_comm_service( u8 applicant );
void requ_write_an_reg( UCHAR *preg,USHORT usRegAddress );
void requ_read_regs( USHORT usRegAddress,UCHAR uNRegs );
void requ_read_bound_regs( USHORT usRegAddress );
void requ_write_version_reg( UCHAR *preg, u8 len, u8 usRegAddress);
BOOL can_use( u8 applicant );
void back_idle( void );

void InnerTrpa_Trsm(char * pframe, u16 len,	pFuncCommType pftrsp );
void RequestWriteBlockData( UCHAR devi,	USHORT addr, UCHAR *src, u16 bytes );

void client_send(char *pbuff, u16 buff_len );
/*******************************************************************************
* Function Name  : RequestReadRegiBound
* Description    : 请求读寄存器参数范围
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RequestReadBlockData( UCHAR devi, 	USHORT addr );
#endif
