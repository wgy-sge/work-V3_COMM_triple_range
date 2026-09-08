/******************** (C) COPYRIGHT 2010 Zidetech *****************************
* File Name          :  fmbrtu.h
* Author             : xxx
* Version            : V0.1
* Date               : 04/26/2010
* Description        :
********************************************************************************/

#ifndef __FMBRTU__H
#define __FMBRTU__H


eMBErrorCode eMBRTUInit( void );

BOOL set_dispose_envi( UCHAR port );
void            eMBRTUStart( void );
void            eMBRTUStop( void );

eMBErrorCode    eMBClientReceive( UCHAR ** pucFrame );
eMBErrorCode    eMBClientSend( USHORT usLength );
eMBErrorCode    eMBClientReSend( void );

void port1_receive_fsm( void );
void port1_transmit_fsm( void );
void port1_t35_expired( void );

//获取接收内容长度
u16 GetRecvBuffLen(void);



#endif
