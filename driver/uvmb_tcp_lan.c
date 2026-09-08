/******************** (C) COPYRIGHT 2015 Zidetech *****************************
* File Name          : uvmb_lan.c
* Author             : xxx
* Version            : V1.0
* Date               : 2015.7.30
* Description        : 
********************************************************************************
* Log :

*******************************************************************************/
#include "include.h"
#include "socket_app.h"

#define MB_SER_PDU_SIZE_MIN     4       
#define MB_SER_PDU_SIZE_CRC     2     


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static char  rcv_buff_lan[SIZE_OF_LAN_BUFFER];            //网口接收缓存
static u16   rcv_fram_lan;                                  //收到帧计数
//static u8    state_rcv_lan = STATE_RX_IDLE;                 //LAN模仿接收状态
static u16   rcv_posi_lan = 0;                              //LAN模仿接收索引

static char  tran_buff_lan[SIZE_OF_LAN_BUFFER+3];         //网口发送缓存


static u8    tcp_mb_map[7];

static unTcpFramelen   tcp_fram_valid_len;



/*******************************************************************************
* Function Name  : TcpLanMbDetectFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void TcpLanMbDetectFrame( void )
{
	u16 i;

	rcv_posi_lan = SocketTcpReadData( (u8*)rcv_buff_lan, SIZE_OF_LAN_BUFFER );

	if( gDevice_CommProtocol == PROTOCOL_MODBUS )//MODBUS
	{		
		if( rcv_posi_lan > 0 )
		{
			rcv_fram_lan = 1;
		}
	}
	else
	{
		for (i= 0; i< rcv_posi_lan; i++)
		{
			// Read one byte from the receive data register 
			if (remote_info.tcplan.rx_full== FALSE)
			{				
				if(((remote_info.tcplan.rx_wp+1)%COM_BUF_SIZE) != remote_info.tcplan.rx_rp)
				{
					remote_info.tcplan.rx_buf[remote_info.tcplan.rx_wp++]= rcv_buff_lan[i]; //加缓冲区是否满判断    

					remote_info.tcplan.rx_wp%=COM_BUF_SIZE;	
				}
				else
				{
					if (remote_info.tcplan.rx_full== FALSE)//输入溢出
					{
						remote_info.tcplan.rx_full= TRUE;
						remote_info.tcplan.rx_err++;//输入缓冲溢出错误计数
					}				
				}
			}
			else
				break;//输入溢出抛弃数据
		}
	}
	
}

/*******************************************************************************
* Function Name  : TcpLanMbReadFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool TcpLanMbReadFrame( char *pdst, u16 length, u16* pcnt )
{
	//u16 i,cnt = 0;

	//读走接收帧
	rcv_fram_lan = 0; 

	if ((rcv_buff_lan[2]== 0)&&(rcv_buff_lan[3]== 0))//协议判断
	{
		memcpy(tcp_mb_map, rcv_buff_lan, 7);
		tcp_fram_valid_len.len_buff[0]= tcp_mb_map[5];
		tcp_fram_valid_len.len_buff[1]= tcp_mb_map[4];
		if ( (tcp_fram_valid_len.valid_len+6)==  rcv_posi_lan)//包数据长度判断
		{
			pdst[0]= gDeviceAddr;
			memcpy(pdst+1, rcv_buff_lan+7, tcp_fram_valid_len.valid_len-1);
			*pcnt= tcp_fram_valid_len.valid_len;
			return TRUE;
		}			
	}
	return FALSE;

}

/*******************************************************************************
* Function Name  : TcpLanMbDoPlus
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void TcpLanMbDoPlus( void )
{
	return;			
}

/*******************************************************************************
* Function Name  : LanMbIsThereFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool TcpLanMbIsThereFrame( void )
{
	if( rcv_fram_lan > 0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*******************************************************************************
* Function Name  : Rs232IsSendFree
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool TcpLanMbIsTransmitFree( void )
{
	//假定能发送
	if( 1 ) 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*******************************************************************************
* Function Name  : LanMbTransmitFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool TcpLanMbTransmitFrame( char * pframe, u16 len )
{
	bool status = TRUE;

	tcp_fram_valid_len.valid_len= len-1+1;//去掉设备地址，增加单元标识符
	//有效字符长度
	tcp_mb_map[4]= tcp_fram_valid_len.len_buff[1];
	tcp_mb_map[5]= tcp_fram_valid_len.len_buff[0];

	memcpy(tran_buff_lan, tcp_mb_map, 7);

	memcpy(tran_buff_lan+7, pframe+1, len-1);
	len= tcp_fram_valid_len.valid_len+6;

	if( SocketTcpWriteData((u8*)tran_buff_lan, len) != len )
	{
		status = FALSE;
	}
	return status;
}

