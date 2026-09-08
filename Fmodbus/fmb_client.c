/******************** (C) COPYRIGHT 2010 Zidetech *****************************
* File Name          : fmb_client.c
* Author             : xxx
* Version            : V1.0
* Date               : 2010.06.22
* Description        : 通讯协议客户端函数实现
********************************************************************************
* Log :
2009.06.22 xxx, initial version
*******************************************************************************/
#include "fmb.h"
#include "stdlib.h"
#include "string.h"


/* ----------------------- Type definitions ---------------------------------*/
typedef  enum
{
	STATE_CLIENT_IDLE,            //< 空闲状态 >
	STATE_CLIENT_RESPONDING,      //< 等待应答状态 >
	STATE_CLIENT_DEALING,         //< 等待处理状态 >

} eMBClientState;

pFuncCommType pfResp= NULL;
static s32            cnt_err= 0;                     //出错计数

/*------------------------ Static variables ----------------------------------*/
static eMBClientState emb_client_state;                 //客户端通讯状态机

static u8 emb_server_for;                               //通讯服务器正在为谁服务 


/*---------------------------- Functions -------------------------------------*/

/*******************************************************************************
* Function Name  : eMBInitClient
* Description    : 初始化Modbus客户端
* Input          : None
* Output         : None
* Return         : errcode
*******************************************************************************/
eMBErrorCode eMBInitClient( void )
{
	eMBErrorCode    eStatus = MB_ENOERR;

	eStatus = eMBRTUInit(  );

	if( eStatus == MB_ENOERR )
	{
		init_ev_queue(  );

		emb_client_state = STATE_CLIENT_IDLE;
	}

	return eStatus;
}

void FreeScpiProc(void);
/*******************************************************************************
* Function Name  : eMBClientPoll
* Description    : Modbus客户端轮询函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void eMBClientPoll( void )
{
	UCHAR    event = 0;            //事件
	UCHAR   *ucMBFrame;
	u16 gRecvLen= 0;

	switch( emb_client_state ) 
	{
	case STATE_CLIENT_RESPONDING://等待应答状态
		/* begin--检查是否有事件发生 */
		if( get_port_event( &event ) == TRUE )
		{
			switch( event )
			{
				/* 收到帧事件 */  
			case EVENT_PORT_1_RECEIVE:
				if( eMBClientReceive( &ucMBFrame ) == MB_ENOERR )
				{ 				
					emb_client_state = STATE_CLIENT_DEALING;//等待处理状态

					if (pfResp!= NULL)
					{
						gRecvLen= GetRecvBuffLen();
						pfResp(ucMBFrame, gRecvLen, MB_ENOERR);
					}

					/* 清除超时计时器 */
					SysTick->CTRL &= ((u32)0xFFFFFFFD);  //关SysTick中断
					ticks_modbus_timeout = 0;
					SysTick->CTRL |= ((u32)0x00000002);  //开SysTick中断   
			
				}
				else//收到误码帧，按照超时处理
				{
					if( pfResp != NULL )
					{
						pfResp( ucMBFrame, 0, MB_ETIMEDOUT );
					}
					FreeScpiProc();
					emb_client_state = STATE_CLIENT_IDLE;
				}

				break;

				/* 超时事件 */
			case EVENT_TIMEOUT:
				if( cnt_err < RETRY_SEND_TIMES )
				{					
					eMBClientReSend( );         //重新发送					
					cnt_err ++;
				}
				else
				{
					if( pfResp != NULL )
					{
						pfResp( ucMBFrame, 0, MB_ETIMEDOUT );
					}
					FreeScpiProc();
					emb_client_state = STATE_CLIENT_IDLE;	
				}				
				break;

			}
		}/* 检查是否有事件发生--end */
		break;

	case STATE_CLIENT_IDLE:
		break;

	case STATE_CLIENT_DEALING:
		break;
	}

}

/*******************************************************************************
* Function Name  : request_comm_service
* Description    : 请求通讯服务
* Input          : applicant--申请者
* Output         : None
* Return         : TRUE or FALSE
*******************************************************************************/
BOOL request_comm_service( u8 applicant )
{
	if( emb_client_state == STATE_CLIENT_IDLE )
	{
		emb_server_for = applicant;
		return TRUE; 
	}
	else
	{
		return FALSE; 
	}
}




/*******************************************************************************
* Function Name  : requ_write_an_reg
* Description    : 请求写一个寄存器
* Input          : preg--寄存器数值的地址
usRegAddress--寄存器的地址 
* Output         : None
* Return         : None
*******************************************************************************/
void requ_write_an_reg( UCHAR *preg,USHORT usRegAddress )
{
	//客户端地址
	client_snd_buf[MB_ADU_ADDR_OFF] = MAIN_BOARD_DEVICE_ADDRESS;
	//功能码
	client_snd_buf[MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_OFF] = MB_FUNC_WRITE_REGISTER;

	//寄存器首地址
	client_snd_buf [MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_WRITE_ADDR_OFF] =
	(usRegAddress<< 1)  >> 8;

	client_snd_buf [MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_WRITE_ADDR_OFF+1] =
	(usRegAddress<< 1) & 0x00ff;

	client_snd_buf [MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_WRITE_ADDR_OFF+2] = 0;
	client_snd_buf [MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_WRITE_ADDR_OFF+3] = 2;
	client_snd_buf [MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_WRITE_ADDR_OFF+4] = 4;	

	//寄存器数值
	client_snd_buf[ MB_SER_PDU_PDU_OFF+ MB_PDU_FUNC_WRITE_VALUE_OFF] = preg[3];
	client_snd_buf[ MB_SER_PDU_PDU_OFF+ MB_PDU_FUNC_WRITE_VALUE_OFF+1] = preg[2];
	client_snd_buf[ MB_SER_PDU_PDU_OFF+ MB_PDU_FUNC_WRITE_VALUE_OFF+2] = preg[1];
	client_snd_buf[ MB_SER_PDU_PDU_OFF+ MB_PDU_FUNC_WRITE_VALUE_OFF+3] = preg[0];

	//发送
	eMBClientSend( 11 );
	emb_client_state = STATE_CLIENT_RESPONDING;
}
/*
void requ_write_version_reg( UCHAR *preg, u8 len, u8 usRegAddress)
{
	//客户端地址
	client_snd_buf[MB_ADU_ADDR_OFF] = MAIN_BOARD_DEVICE_ADDRESS;//0
	//功能码
	client_snd_buf[MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_OFF] = MB_FUNC_WRITE_VERSION_REGISTER;//1
	//寄存器首地址
	client_snd_buf[MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_WRITE_ADDR_OFF] = usRegAddress;//2
	client_snd_buf[MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_WRITE_ADDR_OFF+1] = len;//3
	//寄存器数值
	memcpy(client_snd_buf+MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_WRITE_VALUE_OFF, preg, len);//4

	//发送
	eMBClientSend( 4+ len );
	emb_client_state = STATE_CLIENT_RESPONDING;
}*/



/*******************************************************************************
* Function Name  : RequestWriteBlockData
* Description    : 请求读寄存器参数范围
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RequestWriteBlockData( UCHAR devi,						 
	USHORT addr,
	UCHAR *src, u16 bytes )
{
	client_snd_buf[0] = devi;
	client_snd_buf[1] = MB_FUNC_WRITE_BLOCK_DATA;
	client_snd_buf[2] = addr >> 8;
	client_snd_buf[3] = addr & 0x00FF;
	client_snd_buf[4] = bytes>>8;
	client_snd_buf[5] = bytes;
	UCHAR i;
	for( i=0; i< bytes; i++ )
	{
		client_snd_buf[6+i] = *src++;
	}
	eMBClientSend( 6+bytes );
	emb_client_state = STATE_CLIENT_RESPONDING;
}




/*******************************************************************************
* Function Name  : RequestReadBlockData
* Description    : 请求读寄存器参数范围
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RequestReadBlockData( UCHAR devi, 	USHORT addr )
{
	client_snd_buf[0] = devi;
	client_snd_buf[1] = MB_FUNC_READ_BLOCK_DATA;
	client_snd_buf[2] = addr >> 8;
	client_snd_buf[3] = addr & 0x00FF;	

	eMBClientSend( 4 );	

	emb_client_state = STATE_CLIENT_RESPONDING;

}


/*******************************************************************************
* Function Name  : requ_read_regs
* Description    : 请求读寄存器
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void requ_read_regs( USHORT usRegAddress,UCHAR uNRegs )
{
	//客户端地址
	client_snd_buf[MB_ADU_ADDR_OFF] = MAIN_BOARD_DEVICE_ADDRESS;

	//功能码
	client_snd_buf[MB_SER_PDU_PDU_OFF+ MB_PDU_FUNC_OFF] = MB_FUNC_READ_REGISTERS;

	//寄存器首地址
	client_snd_buf [MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_READ_ADDR_OFF] =
	(usRegAddress<<1)  >> 8;

	client_snd_buf [MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_READ_ADDR_OFF+1] =
	(usRegAddress<<1) & 0x00ff;
	
	//寄存器个数
	client_snd_buf [MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_READ_REGCNT_OFF] =
		(uNRegs<<1)>> 7;

	client_snd_buf [MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_READ_REGCNT_OFF+1] =
		(uNRegs<<1) & 0x00FF;

	//发送
	eMBClientSend( 6);
	emb_client_state = STATE_CLIENT_RESPONDING;
}



/*******************************************************************************
* Function Name  : InnerMbTransmitFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void InnerTrpa_Trsm(char * pframe, u16 len,	pFuncCommType pftrsp )
{
	pfResp = pftrsp;
	memcpy(client_snd_buf+1, pframe+1, len-1);
	client_snd_buf[0]= MAIN_BOARD_DEVICE_ADDRESS;
	eMBClientSend(len-1);
	emb_client_state = STATE_CLIENT_RESPONDING;

}


/*******************************************************************************
* Function Name  : requ_read_bound_regs
* Description    : 请求读寄存器范围
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void requ_read_bound_regs( USHORT usRegAddress )
{
	//客户端地址
	client_snd_buf[MB_ADU_ADDR_OFF] = MAIN_BOARD_DEVICE_ADDRESS;
	//功能码
	client_snd_buf[MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_OFF] = MB_FUNC_READ_BOUND_REGISTERS;
	//寄存器首地址
	client_snd_buf [MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_READ_ADDR_OFF] =
		usRegAddress >> 7;
		
	client_snd_buf [MB_SER_PDU_PDU_OFF+MB_PDU_FUNC_READ_ADDR_OFF+1] =
		(usRegAddress<<1) & 0x00ff;
	//发送
	eMBClientSend( 4 );
	emb_client_state = STATE_CLIENT_RESPONDING;
}


void client_send(char *pbuff, u16 buff_len )
{
	memcpy(client_snd_buf, pbuff, buff_len);
	eMBClientSend( buff_len);
	emb_client_state = STATE_CLIENT_RESPONDING;
}

/*******************************************************************************
* Function Name  : can_use
* Description    : 查询应答消息是否可用
* Input          : applicant--申请者
* Output         : None
* Return         : BOOL
*******************************************************************************/
BOOL can_use( u8 applicant )
{
	if( (emb_client_state == STATE_CLIENT_DEALING)
		&&(emb_server_for == applicant) )
	{
		return TRUE;
	}
	else
	{
		return FALSE; 
	}
}


/*******************************************************************************
* Function Name  : back_idle
* Description    : 置客户端状态机到空闲状态
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void back_idle( void )
{
	emb_client_state = STATE_CLIENT_IDLE; 
}